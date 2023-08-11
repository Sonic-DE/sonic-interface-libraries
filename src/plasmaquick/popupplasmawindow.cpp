/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "popupplasmawindow.h"
#include <kwindowsystem.h>

#include "transientplacementhint_p.h"

void PlasmaQuick::PopupPlasmaWindow::setVisualParent(QQuickItem *item)
{
    if (item == m_visualParent) {
        return;
    }

    m_visualParent = item;
    Q_EMIT visualParentChanged();
    if (isExposed()) {
        updatePosition(); // Dave, we should make this on the next frame on X11?
    }
}

QQuickItem *PlasmaQuick::PopupPlasmaWindow::visualParent() const
{
    return m_visualParent;
}

Plasma::Types::Location PlasmaQuick::PopupPlasmaWindow::location() const
{
    return m_location;
}

void PlasmaQuick::PopupPlasmaWindow::setLocation(Plasma::Types::Location location)
{
    if (location == m_location) {
        return;
    }
    m_location = location;

    if (isExposed()) {
        qWarning() << "location should be set before showing popup window";
    }

    Qt::Edges borders = Qt::LeftEdge | Qt::RightEdge | Qt::TopEdge | Qt::BottomEdge;

    switch (m_location) {
    case Plasma::Types::LeftEdge:
        borders.setFlag(Qt::LeftEdge, false);
        break;
    case Plasma::Types::RightEdge:
        borders.setFlag(Qt::RightEdge, false);
        break;
    case Plasma::Types::TopEdge:
        borders.setFlag(Qt::TopEdge, false);
        break;
    case Plasma::Types::BottomEdge:
        borders.setFlag(Qt::BottomEdge, false);
        break;
    default:
        break;
    }
    setBorders(borders);

    Q_EMIT locationChanged();
}

void PlasmaQuick::PopupPlasmaWindow::showEvent(QShowEvent *event)
{
    updatePosition();
    PlasmaWindow::showEvent(event);
}

void PlasmaQuick::PopupPlasmaWindow::updatePosition()
{
    if (KWindowSystem::isPlatformX11()) {
        updatePositionX11();
    } else if (KWindowSystem::isPlatformWayland()) {
        //        updatePositionWayland();
    }
}

void PlasmaQuick::PopupPlasmaWindow::updatePositionX11()
{
    if (!m_visualParent) {
        qWarning() << "Exposed with no visual parent. This is bad....";
        return;
    }
    setTransientParent(m_visualParent->window());
    TransientPlacementHint placementHint;
    QRectF parentAnchorRect = QRectF(m_visualParent->mapToScene(QPointF(0, 0)), m_visualParent->size());
    placementHint.setParentAnchorArea(parentAnchorRect.toRect());
    placementHint.setParentAnchor(Qt::TopEdge);
    placementHint.setPopupAnchor(Qt::BottomEdge);

    QRect popupPosition = TransientPlacementHelper::popupRect(this, placementHint);
    setGeometry(popupPosition);
}
