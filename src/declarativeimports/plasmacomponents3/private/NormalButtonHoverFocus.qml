/*
    SPDX-FileCopyrightText: 2011 Daker Fernandes Pinheiro <dakerfp@gmail.com>
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2020 Noah Davis <noahadvs@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

/**Documented API
Inherits:
        Item

Imports:
        QtQuick 2.1
        org.kde.plasma.core

Description:
TODO i need more info here


Properties:
**/

import QtQuick 2.12
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    id: root
    // TODO: mark these required when our minimum Qt version is new enough
    property bool showFocus
    property bool showHover

    //used to tell apart this implementation with the touch components one
    property bool hasOverState: true
    property alias enabledBorders: hoverEffect.enabledBorders

    PlasmaCore.FrameSvgItem {
        id: hoverEffect
        anchors {
            fill: parent
            leftMargin: -margins.left
            topMargin: -margins.top
            rightMargin: -margins.right
            bottomMargin: -margins.bottom
        }
        opacity: 0
        imagePath: "widgets/button"
        prefix: "hover"
    }
    PlasmaCore.FrameSvgItem {
        id: focusEffect
        anchors {
            fill: parent
            leftMargin: -margins.left
            topMargin: -margins.top
            rightMargin: -margins.right
            bottomMargin: -margins.bottom
        }
        opacity: 0
        imagePath: "widgets/button"
        prefix: "focus"
    }

    state: {
        if (root.showFocus) {
            return "focused";
        } else if (root.showHover) {
            return "hovered";
        } else {
            return "hidden";
        }
    }

    states: [
        State {
            name: "hovered"
            PropertyChanges {
                target: hoverEffect
                opacity: 1
            }
            PropertyChanges {
                target: focusEffect
                opacity: 0
            }
        },
        State {
            name: "focused"
            PropertyChanges {
                target: hoverEffect
                opacity: 0
            }
            PropertyChanges {
                target: focusEffect
                opacity: 1
            }
        },
        State {
            name: "hidden"
            PropertyChanges {
                target: hoverEffect
                opacity: 0
            }
            PropertyChanges {
                target: focusEffect
                opacity: 0
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "hidden"
            NumberAnimation {
                duration: units.shortDuration
                easing.type: Easing.OutQuad
            }
        },
        Transition {
            from: "*"
            to: "hovered"
            NumberAnimation {
                // Using a shorter duration here makes things feel more responsive.
                duration: units.shortDuration/2
                easing.type: Easing.OutQuad
            }
        },
        Transition {
            from: "*"
            to: "focused"
            NumberAnimation {
                duration: units.shortDuration
                easing.type: Easing.OutQuad
            }
        }
    ]
}
