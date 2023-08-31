/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "popupplasmawindow.h"

#include <QQuickItem>

#include <plasmaquick/plasmaquick_export.h>

namespace PlasmaQuick
{

class AppletQuickItem;

class PLASMAQUICK_EXPORT AppletPopup : public PopupPlasmaWindow
{
    Q_OBJECT
    /**
     * This property holds a pointer to the AppletInterface used by
     */
    Q_PROPERTY(QQuickItem *appletInterface READ appletInterface WRITE setAppletInterface NOTIFY appletInterfaceChanged)

    /**
     * Whether the dialog should be hidden when the dialog loses focus.
     *
     * The default value is @c false.
     **/
    Q_PROPERTY(bool hideOnWindowDeactivate READ hideOnWindowDeactivate WRITE setHideOnWindowDeactivate NOTIFY hideOnWindowDeactivateChanged)

public:
    AppletPopup();
    QQuickItem *appletInterface() const;
    void setAppletInterface(QQuickItem *appletInterface);

    bool hideOnWindowDeactivate() const;
    void setHideOnWindowDeactivate(bool hideOnWindowDeactivate);

Q_SIGNALS:
    void appletInterfaceChanged();
    void hideOnWindowDeactivateChanged();

protected:
    void hideEvent(QHideEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    void onMainItemChanged();
    void updateSize();

    QPointer<AppletQuickItem> m_appletInterface;
    QPointer<QQuickItem> m_lastMainItem;

    bool m_hideOnWindowDeactivate = false;
};

}
