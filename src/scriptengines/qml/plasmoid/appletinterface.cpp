/*
 *   Copyright 2008-2013 Aaron Seigo <aseigo@kde.org>
 *   Copyright 2010-2013 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "appletinterface.h"

#include <QAction>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlExpression>
#include <QQmlProperty>
#include <QQmlComponent>
#include <QSignalMapper>
#include <QTimer>

#include <kactioncollection.h>
#include <QDebug>
#include <kservice.h>
#include <kservicetypetrader.h>
#include <klocalizedstring.h>

#include <Plasma/Plasma>
#include <Plasma/Applet>
#include <Plasma/Corona>
#include <Plasma/Package>
#include <Plasma/PluginLoader>

#include "containmentinterface.h"
#include <kdeclarative/configpropertymap.h>
#include <kdeclarative/qmlobject.h>

#include <packageurlinterceptor.h>


Q_DECLARE_METATYPE(AppletInterface*)

AppletInterface::AppletInterface(QQuickItem *parent)
    : AppletLoader(parent),
      m_actionSignals(0),
      m_backgroundHints(Plasma::Types::StandardBackground),
      m_busy(false),
      m_expanded(false),
      m_hideOnDeactivate(true)
{
}

AppletInterface::~AppletInterface()
{
}

void AppletInterface::componentComplete()
{
    AppletLoader::componentComplete();

    m_configuration = new KDeclarative::ConfigPropertyMap(applet()->configScheme(), this);

    connect(this, &AppletInterface::configNeedsSaving,
            applet(), &Plasma::Applet::configNeedsSaving);
    connect(applet(), &Plasma::Applet::immutabilityChanged,
            this, &AppletInterface::immutableChanged);
    connect(applet(), &Plasma::Applet::userConfiguringChanged,
            this, &AppletInterface::userConfiguringChanged);

    connect(applet(), &Plasma::Applet::statusChanged,
            this, &AppletInterface::statusChanged);

    connect(applet(), &Plasma::Applet::formFactorChanged,
            this, &AppletInterface::formFactorChanged);
    connect(applet(), &Plasma::Applet::locationChanged,
            this, &AppletInterface::locationChanged);

    if (applet()->containment()) {
        connect(applet()->containment(), &Plasma::Containment::screenChanged,
                this, &ContainmentInterface::screenChanged);
    }

    emit busyChanged();
    applet()->updateConstraints(Plasma::Types::UiReadyConstraint);
}

Plasma::Types::FormFactor AppletInterface::formFactor() const
{
    return applet()->formFactor();
}

Plasma::Types::Location AppletInterface::location() const
{
    return applet()->location();
}

QString AppletInterface::currentActivity() const
{
    return applet()->containment()->activity();
}

QObject* AppletInterface::configuration() const
{
    return m_configuration;
}

uint AppletInterface::id() const
{
    return applet()->id();
}

QString AppletInterface::icon() const
{
    return applet()->icon();
}

void AppletInterface::setIcon(const QString &icon)
{
    if (applet()->icon() == icon) {
        return;
    }

    applet()->setIcon(icon);
    emit iconChanged();
}

QString AppletInterface::title() const
{
    return applet()->title();
}

void AppletInterface::setTitle(const QString &title)
{
    if (applet()->title() == title) {
        return;
    }

    applet()->setTitle(title);
    emit titleChanged();
}

bool AppletInterface::isBusy() const
{
    return m_busy;
}

void AppletInterface::setBusy(bool busy)
{
    if (m_busy == busy) {
        return;
    }

    m_busy = busy;
    emit busyChanged();
}

bool AppletInterface::isExpanded() const
{
    return m_expanded;
}

void AppletInterface::setExpanded(bool expanded)
{
    //if there is no compact representation it means it's always expanded
    //Containnments are always expanded
    if (qobject_cast<ContainmentInterface *>(this) || m_expanded == expanded) {
        return;
    }

    m_expanded = expanded;
    emit expandedChanged();
}

Plasma::Types::BackgroundHints AppletInterface::backgroundHints() const
{
    return m_backgroundHints;
}

void AppletInterface::setBackgroundHints(Plasma::Types::BackgroundHints hint)
{
    if (m_backgroundHints == hint) {
        return;
    }

    m_backgroundHints = hint;
    emit backgroundHintsChanged();
}

void AppletInterface::setConfigurationRequired(bool needsConfiguring, const QString &reason)
{
    appletScript()->setConfigurationRequired(needsConfiguring, reason);
}

QString AppletInterface::activeConfig() const
{
    return m_currentConfig.isEmpty() ? "main" : m_currentConfig;
}

void AppletInterface::setActiveConfig(const QString &name)
{
    if (name == "main") {
        m_currentConfig.clear();
        return;
    }

    Plasma::ConfigLoader *loader = m_configs.value(name, 0);

    if (!loader) {
        QString path = appletScript()->package().filePath("config", name + ".xml");
        if (path.isEmpty()) {
            return;
        }

        QFile f(path);
        KConfigGroup cg = applet()->config();
        loader = new Plasma::ConfigLoader(&cg, &f, this);
        m_configs.insert(name, loader);
    }

    m_currentConfig = name;
}

void AppletInterface::writeConfig(const QString &entry, const QVariant &value)
{
    Plasma::ConfigLoader *config = 0;
    if (m_currentConfig.isEmpty()) {
        config = applet()->configScheme();
    } else {
        config = m_configs.value(m_currentConfig, 0);
    }

    if (config) {
        KConfigSkeletonItem *item = config->findItemByName(entry);
        if (item) {
            item->setProperty(value);
            config->blockSignals(true);
            config->writeConfig();
            config->blockSignals(false);
            appletScript()->configNeedsSaving();
        }
    } else
        qWarning() << "Couldn't find a configuration entry";
}

QVariant AppletInterface::readConfig(const QString &entry) const
{
    Plasma::ConfigLoader *config = 0;
    QVariant result;

    if (m_currentConfig.isEmpty()) {
        config = applet()->configScheme();
    } else {
        config = m_configs.value(m_currentConfig, 0);
    }

    if (config) {
        result = config->property(entry);
    }

    return result;
}

QString AppletInterface::file(const QString &fileType)
{
    return appletScript()->package().filePath(fileType.toLocal8Bit().constData(), QString());
}

QString AppletInterface::file(const QString &fileType, const QString &filePath)
{
    return appletScript()->package().filePath(fileType.toLocal8Bit().constData(), filePath);
}

QList<QAction*> AppletInterface::contextualActions() const
{
    QList<QAction*> actions;
    Plasma::Applet *a = applet();
    if (a->failedToLaunch()) {
        return actions;
    }

    foreach (const QString &name, m_actions) {
        QAction *action = a->actions()->action(name);

        if (action) {
            actions << action;
        }
    }

    return actions;
}

void AppletInterface::setActionSeparator(const QString &name)
{
    Plasma::Applet *a = applet();
    QAction *action = a->actions()->action(name);

    if (action) {
        action->setSeparator(true);
    } else {
        action = new QAction(this);
        action->setSeparator(true);
        a->actions()->addAction(name, action);
        m_actions.append(name);
    }
}

void AppletInterface::setAction(const QString &name, const QString &text, const QString &icon, const QString &shortcut)
{
    Plasma::Applet *a = applet();
    QAction *action = a->actions()->action(name);

    if (action) {
        action->setText(text);
    } else {
        action = new QAction(text, this);
        a->actions()->addAction(name, action);

        Q_ASSERT(!m_actions.contains(name));
        m_actions.append(name);

        if (!m_actionSignals) {
            m_actionSignals = new QSignalMapper(this);
            connect(m_actionSignals, SIGNAL(mapped(QString)),
                    this, SLOT(executeAction(QString)));
        }

        connect(action, SIGNAL(triggered()), m_actionSignals, SLOT(map()));
        m_actionSignals->setMapping(action, name);
    }

    if (!icon.isEmpty()) {
        action->setIcon(QIcon::fromTheme(icon));
    }

    if (!shortcut.isEmpty()) {
        action->setShortcut(shortcut);
    }

    action->setObjectName(name);
}

void AppletInterface::removeAction(const QString &name)
{
    Plasma::Applet *a = applet();
    QAction *action = a->actions()->action(name);

    if (action) {
        if (m_actionSignals) {
            m_actionSignals->removeMappings(action);
        }

        delete action;
    }

    m_actions.removeAll(name);
}

QAction *AppletInterface::action(QString name) const
{
    return applet()->actions()->action(name);
}

void AppletInterface::executeAction(const QString &name)
{
    QMetaObject::invokeMethod(this, QString("action_" + name).toLatin1(), Qt::DirectConnection);
}


bool AppletInterface::immutable() const
{
    return applet()->immutability() != Plasma::Types::Mutable;
}

bool AppletInterface::userConfiguring() const
{
    return applet()->isUserConfiguring();
}

int AppletInterface::apiVersion() const
{
    const QString constraint("[X-Plasma-API] == 'declarative' and 'Applet' in [X-Plasma-ComponentTypes]");
    KService::List offers = KServiceTypeTrader::self()->query("Plasma/ScriptEngine", constraint);
    if (offers.isEmpty()) {
        return -1;
    }

    return offers.first()->property("X-KDE-PluginInfo-Version", QVariant::Int).toInt();
}

void AppletInterface::setAssociatedApplication(const QString &string)
{
    applet()->setAssociatedApplication(string);
}

QString AppletInterface::associatedApplication() const
{
    return applet()->associatedApplication();
}

void AppletInterface::setStatus(const Plasma::Types::ItemStatus &status)
{
    applet()->setStatus(status);
}

Plasma::Types::ItemStatus AppletInterface::status() const
{
    return applet()->status();
}

int AppletInterface::screen() const
{
    if (applet()->containment()) {
        return applet()->containment()->screen();
    }

    return -1;
}

void AppletInterface::setHideOnWindowDeactivate(bool hide)
{
    if (m_hideOnDeactivate != hide) {
        m_hideOnDeactivate = hide;
        emit hideOnWindowDeactivateChanged();
    }
}

bool AppletInterface::hideOnWindowDeactivate() const
{
    return m_hideOnDeactivate;
}


QString AppletInterface::downloadPath(const QString &file)
{
    const QString downloadDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/Plasma/" + applet()->pluginInfo().pluginName() + '/';

    if (!QFile::exists(downloadDir)) {
        QDir dir(QChar('/'));
        dir.mkpath(downloadDir);
    }

    return downloadDir;
}

QStringList AppletInterface::downloadedFiles() const
{
    const QString downloadDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/Plasma/" + applet()->pluginInfo().pluginName() + '/';
    QDir dir(downloadDir);
    return dir.entryList(QDir::Files | QDir::NoSymLinks | QDir::Readable);
}


#include "moc_appletinterface.cpp"
