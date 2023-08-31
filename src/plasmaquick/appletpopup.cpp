/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appletpopup.h"

#include <QGuiApplication>

#include <KConfigGroup>

#include "applet.h"
#include "appletquickitem.h"
#include "waylandintegration_p.h"

#include "edgeeventforwarder.h"
#include "windowresizehandler.h"

// TODO queue:
// resize handles
// background hints (in PlasmaWindow)

using namespace PlasmaQuick;

AppletPopup::AppletPopup()
{
    setAnimated(true);
    PlasmaWaylandShellIntegration::get(this)->setRole(QtWayland::org_kde_plasma_surface::role::role_appletpopup);

    auto edgeForwarder = new EdgeEventForwarder(this);
    edgeForwarder->setMargins(margins());
    connect(this, &PlasmaWindow::marginsChanged, this, [edgeForwarder, this]() {
        edgeForwarder->setMargins(margins());
    });
    // edges that have a border are not on a screen edge
    // we want to forward on sides touching screen edges
    edgeForwarder->setActiveEdges(~borders());
    connect(this, &PlasmaWindow::bordersChanged, this, [edgeForwarder, this]() {
        edgeForwarder->setActiveEdges(~borders());
    });

    auto windowResizer = new WindowResizeHandler(this);
    windowResizer->setMargins(margins());
    connect(this, &PlasmaWindow::marginsChanged, this, [windowResizer, this]() {
        windowResizer->setMargins(margins());
    });
    windowResizer->setActiveEdges(borders());
    connect(this, &PlasmaWindow::bordersChanged, this, [windowResizer, this]() {
        windowResizer->setActiveEdges(borders());
    });

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

        QSize popupSize = QSize(mainItem()->implicitWidth(), mainItem()->implicitHeight());

        // Dave, this is order dependent, how should we handle this in a way that isn't terrible?
        if (m_appletInterface) {
            KConfigGroup config = m_appletInterface->applet()->config();
            popupSize.rwidth() = config.readEntry("popupWidth", popupSize.width());
            popupSize.rheight() = config.readEntry("popupHeight", popupSize.height());
        }
        resize(popupSize.grownBy(margins()));
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
    resize(popupSize.grownBy(margins()));
}

void AppletPopup::hideEvent(QHideEvent *event)
{
    // Persist the size if this contains an applet
    if (m_appletInterface) {
        KConfigGroup config = m_appletInterface->applet()->config();
        // save size without margins, so we're robust against theme changes
        const QSize popupSize = size().shrunkBy(margins());
        config.writeEntry("popupWidth", popupSize.width());
        config.writeEntry("popupHeight", popupSize.height());
        config.sync();
    }

    QQuickWindow::hideEvent(event);
}

void AppletPopup::focusOutEvent(QFocusEvent *ev)
{
    if (m_hideOnWindowDeactivate) {
        bool parentHasFocus = false;

        QWindow *parentWindow = transientParent();

        // DAVE. We're already using qApp->focusWindow below
        // couldn't it just be focusWindow->isAncestorOf(this) || this->isAncestorOf(focus)
        while (parentWindow) {
            if (parentWindow->isActive() && !(parentWindow->flags() & Qt::WindowDoesNotAcceptFocus)) {
                parentHasFocus = true;
                break;
            }

            parentWindow = parentWindow->transientParent();
        }

        const QWindow *focusWindow = QGuiApplication::focusWindow();
        bool childHasFocus = focusWindow && ((focusWindow->isActive() && isAncestorOf(focusWindow)) || (focusWindow->type() & Qt::Popup) == Qt::Popup);

        // DAVE, what was this doing?
        //        const bool viewClicked = qobject_cast<const PlasmaQuick::SharedQmlEngine *>(focusWindow) || qobject_cast<const ConfigView *>(focusWindow);

        if (/*viewClicked || */ (!parentHasFocus && !childHasFocus)) {
            setVisible(false);
        }
    }

    QQuickWindow::focusOutEvent(ev);
}
