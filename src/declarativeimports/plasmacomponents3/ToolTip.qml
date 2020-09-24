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

    implicitWidth: contentItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding

    margins: units.gridUnit
    leftPadding: background.margins.left
    topPadding: background.margins.top
    rightPadding: background.margins.right
    bottomPadding: background.margins.bottom

    closePolicy: T.Popup.CloseOnEscape | T.Popup.CloseOnPressOutsideParent | T.Popup.CloseOnReleaseOutsideParent

    contentItem: Label {
        // Strip out ampersands right before non-whitespace characters, i.e.
        // those used to determine the alt key shortcut
        text: control.text.replace(/&(?=\S)/g, "")
        font: control.font
        color: PlasmaCore.Theme.complementaryTextColor

    }

    background: PlasmaCore.FrameSvgItem {
        colorGroup: PlasmaCore.Theme.ComplementaryColorGroup
        height: Math.max(implicitHeight, control.height) + margins.top + margins.bottom
        width: Math.max(implicitWidth, control.width) + margins.left + margins.right
        imagePath: "widgets/tooltip"
        prefix: "shadow"

        PlasmaCore.FrameSvgItem {
            colorGroup: PlasmaCore.Theme.ComplementaryColorGroup
            anchors.fill: parent
            anchors.leftMargin: parent.margins.left
            anchors.rightMargin: parent.margins.right
            anchors.topMargin: parent.margins.top
            anchors.bottomMargin: parent.margins.bottom
            imagePath: "opaque/widgets/tooltip"
        }
    }
}
