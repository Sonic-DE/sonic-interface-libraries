/*
    SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2022 ivan (@ratijas) tkachenko <me@ratijas.tk>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.15
import QtQuick.Templates @QQC2_VERSION@ as T
import org.kde.plasma.core 2.1 as PlasmaCore
import "private" as Private

Item {
    id: root

    required property T.AbstractButton control

    implicitWidth: inactive.width
    implicitHeight: Math.max(inactive.height, button.implicitHeight)
    opacity: control.enabled ? 1 : 0.6

    PlasmaCore.Svg {
        id: switchSvg
        imagePath: "widgets/switch"
    }

    PlasmaCore.FrameSvgItem {
        id: inactive
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: 1
            rightMargin: 1
            verticalCenter: parent.verticalCenter
        }
        height: switchSvg.hasElement("hint-bar-size")
                ? switchSvg.elementSize("hint-bar-size").height
                : root.implicitHeight
        width: switchSvg.hasElement("hint-bar-size")
                ? switchSvg.elementSize("hint-bar-size").width
                : root.implicitHeight * 2
        imagePath: "widgets/switch"
        prefix: "inactive"
    }
    PlasmaCore.FrameSvgItem {
        anchors.fill: inactive
        imagePath: "widgets/switch"
        prefix: "active"
        opacity: root.control.checked ? 1 : 0
        Behavior on opacity {
            OpacityAnimator {
                duration: PlasmaCore.Units.shortDuration
                easing.type: Easing.InOutQuad
            }
        }
    }
    PlasmaCore.SvgItem {
        id: button
        x: root.control.mirrored ? (root.control.checked ? 0 : parent.width - width) : (root.control.checked ? parent.width - width : 0)
        anchors.verticalCenter: parent.verticalCenter
        svg: switchSvg
        elementId: root.control.hovered ? "handle-hover" : "handle"

        implicitWidth: naturalSize.width
        implicitHeight: naturalSize.height

        Behavior on x {
            XAnimator {
                duration: PlasmaCore.Units.shortDuration
                easing.type: Easing.InOutQuad
            }
        }
    }
}

