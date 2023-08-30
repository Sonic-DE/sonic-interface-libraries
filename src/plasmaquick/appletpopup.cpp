/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appletpopup.h"

#include "appletquickitem.h"

// TODO queue:
//  Sliding (in superclass)

// min/max hint propagation from mainItem
// close on Focus out
// resize handles + save restore
// mouse press fitt's law handling

using namespace PlasmaQuick;

AppletPopup::AppletPopup()
{
    setAnimated(true);
    connect(this, &PlasmaQuick::PlasmaWindow::mainItemChanged, this, [this]() {
        if (m_lastMainItem) {
            disconnect(m_lastMainItem, &QQuickItem::implicitWidthChanged, this, &AppletPopup::updateSize);
            disconnect(m_lastMainItem, &QQuickItem::implicitHeightChanged, this, &AppletPopup::updateSize);
        }
        m_lastMainItem = mainItem();

        if (!mainItem()) {
            return;
        }
        connect(mainItem(), &QQuickItem::implicitWidthChanged, this, &AppletPopup::updateSize);
        connect(mainItem(), &QQuickItem::implicitHeightChanged, this, &AppletPopup::updateSize);
        updateSize();
    });
}

QQuickItem *AppletPopup::appletInterface() const
{
    return m_appletInterface.data();
}

void AppletPopup::setAppletInterface(QQuickItem *appletInterface)
{
    if (appletInterface == m_appletInterface) {
        return;
    }

    m_appletInterface = qobject_cast<AppletQuickItem *>(appletInterface);
    ;
    Q_EMIT appletInterfaceChanged();
}

bool AppletPopup::hideOnWindowDeactivate() const
{
    return m_hideOnWindowDeactivate;
}

void AppletPopup::setHideOnWindowDeactivate(bool hideOnWindowDeactivate)
{
    if (hideOnWindowDeactivate == m_hideOnWindowDeactivate) {
        return;
    }
    m_hideOnWindowDeactivate = hideOnWindowDeactivate;
    Q_EMIT hideOnWindowDeactivateChanged();
}

void AppletPopup::updateSize()
{
    QSize popupSize = QSize(mainItem()->implicitWidth(), mainItem()->implicitHeight());

    if (!popupSize.isValid()) {
        qWarning() << "Applet implicitSize is broken";
        popupSize = QSize(500, 500);
    }

    popupSize = popupSize.grownBy(margins());
    resize(popupSize);
}

// void Dialog::focusOutEvent(QFocusEvent *ev)
//{
//     if (d->hideOnWindowDeactivate) {
//         bool parentHasFocus = false;

//        QWindow *parentWindow = transientParent();

//        while (parentWindow) {
//            if (parentWindow->isActive() && !(parentWindow->flags() & Qt::WindowDoesNotAcceptFocus)) {
//                parentHasFocus = true;

//                break;
//            }

//            parentWindow = parentWindow->transientParent();
//        }

//        const QWindow *focusWindow = QGuiApplication::focusWindow();
//        bool childHasFocus = focusWindow && ((focusWindow->isActive() && isAncestorOf(focusWindow)) || (focusWindow->type() & Qt::Popup) == Qt::Popup);

//        const bool viewClicked = qobject_cast<const PlasmaQuick::SharedQmlEngine *>(focusWindow) || qobject_cast<const ConfigView *>(focusWindow);

//        if (viewClicked || (!parentHasFocus && !childHasFocus)) {
//            setVisible(false);
//            Q_EMIT windowDeactivated();
//        }
//    }

//    QQuickWindow::focusOutEvent(ev);
//}
