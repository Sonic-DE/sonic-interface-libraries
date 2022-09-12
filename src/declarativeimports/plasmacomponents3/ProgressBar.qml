/*
    SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.15
import QtQuick.Templates @QQC2_VERSION@ as T
import org.kde.plasma.core 2.0 as PlasmaCore

T.ProgressBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    hoverEnabled: false

    PlasmaCore.Svg {
        id: barSvg
        imagePath: "widgets/bar_meter_horizontal"
        colorGroup: PlasmaCore.ColorScope.colorGroup
    }

    contentItem: PlasmaCore.FrameSvgItem {
        imagePath: "widgets/bar_meter_horizontal"
        prefix: "bar-inactive"
        colorGroup: PlasmaCore.ColorScope.colorGroup

        implicitWidth: PlasmaCore.Units.gridUnit * 8
        implicitHeight: barSvg.hasElement("hint-bar-size") ? barSvg.elementSize("hint-bar-size").height : fixedMargins.top + fixedMargins.bottom

        width: Math.max(fixedMargins.left + fixedMargins.right, control.availableWidth)
        height: implicitHeight

        x: control.leftPadding
        y: control.topPadding + Math.round((control.availableHeight - height) / 2)

        PlasmaCore.FrameSvgItem {
            id: grooveFill

            readonly property real indeterminateWidth: Math.round(control.availableWidth / 4)
            property real indeterminateProgress: 0

            imagePath: "widgets/bar_meter_horizontal"
            prefix: "bar-active"
            colorGroup: PlasmaCore.ColorScope.colorGroup

            LayoutMirroring.enabled: control.mirrored
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: control.indeterminate ? indeterminateProgress * (control.availableWidth - indeterminateWidth) : 0
            anchors.bottom: parent.bottom

            // unlike Slider, this width is allowed to be less than its minimum (margins) size, in which case it would render as blank.
            width: control.indeterminate ? indeterminateWidth : Math.round(control.position * control.availableWidth)
            visible: width >= fixedMargins.left + fixedMargins.right

            SequentialAnimation on indeterminateProgress {
                id: animation

                loops: Animation.Infinite
                running: control.indeterminate && grooveFill.visible

                NumberAnimation {
                    duration: PlasmaCore.Units.humanMoment / 2
                    easing.type: Easing.InOutSine
                    to: 1
                }
                NumberAnimation {
                    duration: PlasmaCore.Units.humanMoment / 2
                    easing.type: Easing.InOutSine
                    to: 0
                }
            }
        }
    }
}
