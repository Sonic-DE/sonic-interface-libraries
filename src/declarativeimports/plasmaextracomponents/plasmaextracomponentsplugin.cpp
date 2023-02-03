/*
    SPDX-FileCopyrightText: 2012 Sebastian Kügler <sebas@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plasmaextracomponentsplugin.h"

#include <QQmlEngine>
#include <QtQml>

// #include <KSharedConfig>
// #include <KConfigGroup>

#include "qmenu.h"
#include "qmenuitem.h"

void PlasmaExtraComponentsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_ASSERT(uri == QByteArray("org.kde.plasma.extras"));
}

void PlasmaExtraComponentsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QByteArray("org.kde.plasma.extras"));
    qmlRegisterType<QMenuProxy>(uri, 2, 0, "Menu");
    qmlRegisterType<QMenuItem>(uri, 2, 0, "MenuItem");
    qmlRegisterUncreatableType<DialogStatus>(uri, 2, 0, "DialogStatus", {});
}
