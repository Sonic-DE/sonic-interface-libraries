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
    // TODO: mark this required when our minimum Qt version is new enough
    // This should be either a Button or ToolButton
    property Item parentControl
    property bool keyboardFocus

    property real leftMargin: surfaceHover.margins.left
    property real topMargin: surfaceHover.margins.top
    property real rightMargin: surfaceHover.margins.right
    property real bottomMargin: surfaceHover.margins.bottom
    property string usedPrefix: surfaceHover.usedPrefix

    ButtonShadow {
        anchors.fill: parent
        visible: root.usedPrefix === "normal"
        showShadow: !(parentControl.checked || parentControl.pressed)
    }

    // TODO: Maybe add a way to customize the look of normal state flat buttons with "toolbutton-normal"? 
    // TODO: Maybe add a way to customize the background of focused flat buttons with "toolbutton-focus-background"? 
    // TODO KF6: "flat" would be a more logical name than "toolbutton" since toolbuttons can be non-flat.
    PlasmaCore.FrameSvgItem {
        id: surfaceHover
        anchors.fill: parent
        imagePath: "widgets/button"
        // TODO KF6: making "toolbutton-hover" margins work like "hover" and using "hover" as a fallback would make more sense.
        // The fallback is "normal" to match PC2 behavior. Some 3rd party themes depend on this.
        prefix: ["toolbutton-hover", "normal"]
        opacity: 0
    }

    PlasmaCore.FrameSvgItem {
        id: surfacePressed
        anchors.fill: parent
        imagePath: "widgets/button"
        prefix: ["toolbutton-pressed", "pressed"]
        opacity: 0
    }

    // focusEffect is independent from hovered and pressed states.
    PlasmaCore.FrameSvgItem {
        id: focusEffect
        anchors {
            fill: parent
            leftMargin: -margins.left
            topMargin: -margins.top
            rightMargin: -margins.right
            bottomMargin: -margins.bottom
        }
        imagePath: "widgets/button"
        prefix: ["toolbutton-focus", "focus"]
        opacity: keyboardFocus ? 1 : 0
        Behavior on opacity {
            OpacityAnimator {
                easing.type: Easing.OutQuad
                duration: PlasmaCore.Units.shortDuration
            }
        }
    }

    state: {
        if (parentControl.checked || parentControl.pressed) {
            return "pressed";
        } else if (parentControl.hovered) {
            return "hovered";
        } else {
            return "normal";
        }
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: root
                leftMargin: surfaceHover.margins.left
                topMargin: surfaceHover.margins.top
                rightMargin: surfaceHover.margins.right
                bottomMargin: surfaceHover.margins.bottom
                usedPrefix: surfaceHover.usedPrefix
            }
            PropertyChanges {
                target: surfaceHover
                opacity: 0
            }
            PropertyChanges {
                target: surfacePressed
                opacity: 0
            }
        },
        State {
            name: "hovered"
            PropertyChanges {
                target: root
                leftMargin: surfaceHover.margins.left
                topMargin: surfaceHover.margins.top
                rightMargin: surfaceHover.margins.right
                bottomMargin: surfaceHover.margins.bottom
                usedPrefix: surfaceHover.usedPrefix
            }
            PropertyChanges {
                target: surfaceHover
                opacity: 1
            }
            PropertyChanges {
                target: surfacePressed
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
                target: surfaceHover
                opacity: 0
            }
            PropertyChanges {
                target: surfacePressed
                opacity: 1
            }
        }
    ]

    transitions: [
        //FIXME: For some reason, the hovered state doesn't work with OpacityAnimator.
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
            to: "hovered"
            NumberAnimation {
                property: "opacity"
                easing.type: Easing.OutQuad
                // Using a shorter duration here makes things feel more responsive
                duration: PlasmaCore.Units.shortDuration/2
            }
        },
        Transition {
            from: "*"
            to: "pressed"
            NumberAnimation {
                property: "opacity"
                easing.type: Easing.OutQuad
                // Using a shorter duration here makes things feel more responsive
                duration: PlasmaCore.Units.shortDuration/2
            }
        }
    ]
}
/*
Item {
    ButtonShadow {
        anchors.fill: parent
        visible: (!parentControl.flat || parentControl.hovered) && (!parentControl.pressed || !parentControl.checked)
        state: {
            if (parentControl.pressed) {
                return "hidden"
            } else if (parentControl.hovered) {
                return "hover"
            } else if (parentControl.activeFocus) {
                return "focus"
            } else {
                return "shadow"
            }
        }
    }
    PlasmaCore.FrameSvgItem {
        id: surfaceNormal
        anchors.fill: parent
        imagePath: "widgets/button"
        prefix: parentControl.activeFocus ? ["focus-background", "normal"] : "normal"
        opacity: (!parentControl.flat || parentControl.hovered) && (!parentControl.pressed || !parentControl.checked) ? 1 : 0
        Behavior on opacity {
            OpacityAnimator {
                duration: units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
    }
    PlasmaCore.FrameSvgItem {
        anchors.fill: parent
        imagePath: "widgets/button"
        prefix: "pressed"
        opacity: parentControl.checked || parentControl.pressed ? 1 : 0
        Behavior on opacity {
            OpacityAnimator {
                duration: units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
    }
}
*//*
Item {
    ButtonShadow {
        anchors.fill: parent
        readonly property bool keyboardFocus: parentControl.activeFocus &&
            (parentControl.focusReason == Qt.TabFocusReason || parentControl.focusReason == Qt.BacktabFocusReason)
        visible:  (!parentControl.flat || parentControl.hovered || keyboardFocus) && (!parentControl.pressed || !parentControl.checked)

        state: {
            if (parentControl.pressed) {
                return "hidden"
            } else if (parentControl.hovered) {
                return "hover"
            } else if (keyboardFocus) {
                return "focus"
            } else if (parentControl.flat) {
                return "hidden"
            } else {
                return "shadow"
            }
        }
    }
    PlasmaCore.FrameSvgItem {
        id: surfaceNormal
        anchors.fill: parent
        imagePath: "widgets/button"
        prefix: "normal"
        opacity: !parentControl.flat && (!parentControl.pressed || !parentControl.checked) ? 1 : 0
        Behavior on opacity {
            OpacityAnimator {
                duration: units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
    }
    PlasmaCore.FrameSvgItem {
        id: surfacePressed
        anchors.fill: parent
        imagePath: "widgets/button"
        prefix: parentControl.flat ? "toolbutton-pressed" : "pressed"
        opacity: parentControl.checked || parentControl.pressed ? 1 : 0
        Behavior on opacity {
            OpacityAnimator {
                duration: units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
    }
}
*/
