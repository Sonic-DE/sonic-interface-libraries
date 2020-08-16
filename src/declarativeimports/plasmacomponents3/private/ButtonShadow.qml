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
    id: main
    // TODO: mark this required when our minimum Qt version is new enough
    property bool showShadow

    //used to tell apart this implementation with the touch components one
    property bool hasOverState: true
    property alias enabledBorders: shadow.enabledBorders

    PlasmaCore.FrameSvgItem {
        id: shadow
        anchors {
            fill: parent
            leftMargin: -margins.left
            topMargin: -margins.top
            rightMargin: -margins.right
            bottomMargin: -margins.bottom
        }
        imagePath: "widgets/button"
        prefix: "shadow"
        opacity: showShadow ? 1 : 0
        Behavior on opacity {
            OpacityAnimator {
                easing.type: Easing.OutQuad
                // Same duration as pressed and hovered state durations
                duration: PlasmaCore.Units.shortDuration/2
            }
        }
    }
}
