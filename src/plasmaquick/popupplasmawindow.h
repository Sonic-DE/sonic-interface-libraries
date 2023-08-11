/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plasmawindow.h"

#include <Plasma/Plasma>
#include <QQuickItem>
#include <plasmaquick/plasmaquick_export.h>

namespace PlasmaQuick
{
class PopupPlasmaWindowPrivate;

/**
 * @brief The PopupPlasmaWindow class is a styled Plasma window that can be positioned
 * relative to an existing Item on another window. When shown the popup is placed correctly.s
 *
 * On Wayland this is currently an XdgTopLevel with the PlasmaShellSurface used on top.
 * Do not rely on that implementation detail
 */
class PLASMAQUICK_EXPORT PopupPlasmaWindow : public PlasmaWindow
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *visualParent READ visualParent WRITE setVisualParent NOTIFY visualParentChanged)
    Q_PROPERTY(Plasma::Types::Location location READ location WRITE setLocation NOTIFY
                   locationChanged) // Dave, I don't like this, I'd rather be explicit about the popup

public:
    QQuickItem *visualParent() const;
    void setVisualParent(QQuickItem *parent);

    Plasma::Types::Location location() const;
    void setLocation(Plasma::Types::Location location);

    void showEvent(QShowEvent *event) override;

Q_SIGNALS:
    void visualParentChanged();
    void locationChanged();

private:
    void updatePosition();
    void updatePositionX11();
    void updatePositionWayland();
    QPoint calculatePosition();

    QPointer<QQuickItem> m_visualParent;
    Plasma::Types::Location m_location = Plasma::Types::Desktop;
};
}
