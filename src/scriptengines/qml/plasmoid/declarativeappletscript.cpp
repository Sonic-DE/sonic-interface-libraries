/*
 *   Copyright 2009 by Alan Alpert <alan.alpert@nokia.com>
 *   Copyright 2010 by Ménard Alexis <menard@kde.org>

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

#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlExpression>
#include <QQmlProperty>
#include <QFile>
#include <QTimer>
#include <QUiLoader>
#include <QWidget>

#include <kconfiggroup.h>
#include <QDebug>
#include <klocalizedstring.h>

#include <Plasma/Applet>
#include <Plasma/Corona>
#include <Plasma/Package>
#include <Plasma/PluginLoader>
#include <Plasma/Service>

#include "plasmoid/declarativeappletscript.h"

#include "plasmoid/appletinterface.h"
#include "plasmoid/containmentinterface.h"

#include <kdeclarative/qmlobject.h>
#include <kdeclarative/configpropertymap.h>

//HACK?
#include "applet.h"

#include <packageurlinterceptor.h>

K_EXPORT_PLASMA_APPLETSCRIPTENGINE(declarativeappletscript, DeclarativeAppletScript)


DeclarativeAppletScript::DeclarativeAppletScript(QObject *parent, const QVariantList &args)
    : Plasma::AppletScript(parent),
      m_interface(0)
{
    qmlRegisterType<AppletInterface>();
    qmlRegisterType<KDeclarative::ConfigPropertyMap>();
    qmlRegisterType<AppletInterface>();
    qmlRegisterType<QAction>();
    qmlRegisterUncreatableType<SizeHintAttachedType>("org.kde.plasma.shell", 2, 0, "SizeHint",
                                             QLatin1String("Do not create objects of type SizeHint"));
    qmlRegisterType<AppletInterface>("org.kde.plasma.shell", 2, 0, "Applet");
    qmlRegisterType<ContainmentInterface>("org.kde.plasma.shell", 2, 0, "Containment");

    Q_UNUSED(args);
}

DeclarativeAppletScript::~DeclarativeAppletScript()
{
}

bool DeclarativeAppletScript::init()
{
    //make possible to import extensions from the package
    //FIXME: probably to be removed, would make possible to use native code from within the package :/
    //m_interface->qmlObject()->engine()->addImportPath(package()->path()+"/contents/imports");

    Plasma::Applet *a = applet();
    Plasma::Containment *pc = qobject_cast<Plasma::Containment *>(a);

     {
       // m_interface = new AppletInterface(this);
        KDeclarative::QmlObject *qmlObject = new KDeclarative::QmlObject(this);
        QVariantHash initialProperties;
        initialProperties["_plasma_appletscript"] = QVariant::fromValue(this);

        //Hook generic url resolution to the applet package as well
        //TODO: same thing will have to be done for every qqmlengine: PackageUrlInterceptor is material for plasmaquick?
        PackageUrlInterceptor *interceptor = new PackageUrlInterceptor(qmlObject->engine(), a->package());
        interceptor->addAllowedPath(a->containment()->corona()->package().path());
        qmlObject->engine()->setUrlInterceptor(interceptor);

        qmlObject->setInitializationDelayed(true);
        qmlObject->setSource(QUrl::fromLocalFile(mainScript()));
        qmlObject->completeInitialization(initialProperties);

        //error?
        if (!qmlObject->engine() || !qmlObject->engine()->rootContext() || !qmlObject->engine()->rootContext()->isValid() || qmlObject->mainComponent()->isError()) {
            QString reason;
            foreach (QQmlError error, qmlObject->mainComponent()->errors()) {
                reason += error.toString()+'\n';
            }
            reason = i18n("Error loading QML file: %1", reason);
            qWarning() << reason;

            qmlObject->setSource(QUrl::fromLocalFile(applet()->containment()->corona()->package().filePath("appleterror")));
            qmlObject->completeInitialization();


            //even the error message QML may fail
            if (qmlObject->mainComponent()->isError()) {
                return false;
            } else {
                qmlObject->rootObject()->setProperty("reason", reason);
            }

            setLaunchErrorMessage(reason);
        }

        a->setProperty("graphicObject", QVariant::fromValue(qmlObject->rootObject()));
    }


    return true;
}

QString DeclarativeAppletScript::filePath(const QString &type, const QString &file) const
{
    return package().filePath(type.toLocal8Bit().constData(), file);
}

void DeclarativeAppletScript::constraintsEvent(Plasma::Types::Constraints constraints)
{
    if (constraints & Plasma::Types::FormFactorConstraint) {
        emit formFactorChanged();
    }

    if (constraints & Plasma::Types::LocationConstraint) {
        emit locationChanged();
    }

    if (constraints & Plasma::Types::ContextConstraint) {
        emit contextChanged();
    }
}

QList<QAction*> DeclarativeAppletScript::contextualActions()
{
    if (!m_interface) {
        return QList<QAction *>();
    }

    return m_interface->contextualActions();
}


#include "declarativeappletscript.moc"

