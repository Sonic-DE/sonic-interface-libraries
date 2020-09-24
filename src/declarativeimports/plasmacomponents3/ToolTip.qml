/*
    SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2016 The Qt Company Ltd.

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.6
import QtQuick.Templates @QQC2_VERSION@ as T
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.kirigami 2.5 as Kirigami

T.ToolTip {
    id: control

    x: parent ? Math.round((parent.width - implicitWidth) / 2) : 0
    y: -implicitHeight - 3

    visible: parent && (Kirigami.Settings.tabletMode ? parent.pressed : parent.hovered)
    delay: Kirigami.Units.toolTipDelay
    timeout: 5000

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)

    margins: units.gridUnit
    leftPadding: backgroundItem.margins.left
    topPadding: backgroundItem.margins.top
    rightPadding: backgroundItem.margins.right
    bottomPadding: backgroundItem.margins.bottom
    leftInset: -shadowItem.margins.left
    topInset: -shadowItem.margins.top
    rightInset: -shadowItem.margins.right
    bottomInset: -shadowItem.margins.bottom

    closePolicy: T.Popup.CloseOnEscape | T.Popup.CloseOnPressOutsideParent | T.Popup.CloseOnReleaseOutsideParent

    PlasmaCore.ColorScope.colorGroup: PlasmaCore.Theme.ToolTipColorGroup
    PlasmaCore.ColorScope.inherit: false

    contentItem: Label {
        // Strip out ampersands right before non-whitespace characters, i.e.
        // those used to determine the alt key shortcut
        text: control.text.replace(/&(?=\S)/g, "")
        font: control.font
    }

    background: PlasmaCore.FrameSvgItem {
        id: shadowItem
        imagePath: "widgets/tooltip"
        prefix: "shadow"

        PlasmaCore.FrameSvgItem {
            id: backgroundItem
            anchors.fill: parent
            anchors.leftMargin: parent.margins.left
            anchors.rightMargin: parent.margins.right
            anchors.topMargin: parent.margins.top
            anchors.bottomMargin: parent.margins.bottom
            imagePath: "opaque/widgets/tooltip"
        }
    }
}
