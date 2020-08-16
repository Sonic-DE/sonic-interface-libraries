/*
    SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2020 Noah Davis <noahadvs@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

/* This file was created as an attempt to reduce the amount of code
 * duplicated between Button.qml and ToolButton.qml.
 */

import QtQuick 2.6
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    id: root
    // TODO: Mark this required when our minimum Qt version is new enough.
    // This should be either a Button or ToolButton.
    property Item parentControl
    property bool keyboardFocus

    property real leftMargin: surfaceNormal.margins.left
    property real topMargin: surfaceNormal.margins.top
    property real rightMargin: surfaceNormal.margins.right
    property real bottomMargin: surfaceNormal.margins.bottom
    property string usedPrefix: surfaceNormal.usedPrefix

    ButtonShadow {
        anchors.fill: parent
        showShadow: !(parentControl.checked || parentControl.pressed)
    }

    PlasmaCore.FrameSvgItem {
        id: surfaceNormal
        anchors.fill: parent
        imagePath: "widgets/button"
        prefix:  "normal"
        opacity: 1
    }

    // Intentionally lower than surfacePressed and surfaceFocused
    NormalButtonHoverFocus {
        visible: !parentControl.pressed
        anchors.fill: parent
        showFocus: root.keyboardFocus
        showHover: parentControl.hovered && !(parentControl.checked || parentControl.pressed)
    }

    PlasmaCore.FrameSvgItem {
        id: surfacePressed
        anchors.fill: parent
        imagePath: "widgets/button"
        prefix: "pressed"
        opacity: 0
    }

    PlasmaCore.FrameSvgItem {
        id: surfaceFocused
        anchors.fill: parent
        imagePath: "widgets/button"
        prefix: ["focus-background", "normal"]
        opacity: 0
    }

    state: {
        if (parentControl.checked || parentControl.pressed) {
            return "pressed";
        } else if (keyboardFocus && surfaceFocused.usedPrefix != "normal") {
            return "focused";
        } else {
            return "normal";
        }
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: root
                leftMargin: surfaceNormal.margins.left
                topMargin: surfaceNormal.margins.top
                rightMargin: surfaceNormal.margins.right
                bottomMargin: surfaceNormal.margins.bottom
                usedPrefix: surfaceNormal.usedPrefix
            }
            PropertyChanges {
                target: surfaceNormal
                opacity: 1
            }
            PropertyChanges {
                target: surfacePressed
                opacity: 0
            }
            PropertyChanges {
                target: surfaceFocused
                opacity: 0
            }
        },
        State {
            name: "pressed"
            PropertyChanges {
                target: root
                leftMargin: surfacePressed.margins.left
                topMargin: surfacePressed.margins.top
                rightMargin: surfacePressed.margins.right
                bottomMargin: surfacePressed.margins.bottom
                usedPrefix: surfacePressed.usedPrefix
            }
            PropertyChanges {
                target: surfaceNormal
                opacity: 0
            }
            PropertyChanges {
                target: surfacePressed
                opacity: 1
            }
            PropertyChanges {
                target: surfaceFocused
                opacity: 0
            }
        },
        State {
            name: "focused"
            PropertyChanges {
                target: root
                leftMargin: surfaceFocused.margins.left
                topMargin: surfaceFocused.margins.top
                rightMargin: surfaceFocused.margins.right
                bottomMargin: surfaceFocused.margins.bottom
                usedPrefix: surfaceFocused.usedPrefix
            }
            PropertyChanges {
                target: surfaceNormal
                opacity: 0
            }
            PropertyChanges {
                target: surfacePressed
                opacity: 0
            }
            PropertyChanges {
                target: surfaceFocused
                opacity: 1
            }
        }
    ]

    transitions: [
        //FIXME: For some reason, the focused state doesn't work with OpacityAnimator.
        // OpacityAnimator would be more efficient.
        Transition {
            from: "*"
            to: "normal"
            NumberAnimation {
                property: "opacity"
                easing.type: Easing.OutQuad
                duration: PlasmaCore.Units.shortDuration
            }
        },
        Transition {
            from: "*"
            to: "pressed"
            NumberAnimation {
                property: "opacity"
                easing.type: Easing.OutQuad
                // Using a shorter duration here makes things feel more responsive.
                duration: PlasmaCore.Units.shortDuration/2
            }
        },
        Transition {
            from: "*"
            to: "focused"
            NumberAnimation {
                property: "opacity"
                easing.type: Easing.OutQuad
                duration: PlasmaCore.Units.shortDuration
            }
        }
    ]
}
