/*
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

    property real leftMargin
    property real topMargin
    property real rightMargin
    property real bottomMargin
    // FIXME: I don't think this actually works
    property string usedPrefix

    readonly property bool keyboardFocus: parentControl.activeFocus &&
        (parentControl.focusReason == Qt.TabFocusReason || parentControl.focusReason == Qt.BacktabFocusReason)

    FlatButtonBackground {
        id: flatButtonBackground
        visible: false
        anchors.fill: parent
        parentControl: root.parentControl
        keyboardFocus: root.keyboardFocus
    }

    NormalButtonBackground {
        id: normalButtonBackground
        visible: false
        anchors.fill: parent
        parentControl: root.parentControl
        keyboardFocus: root.keyboardFocus
    }

    state: parentControl.flat ? "flat" : "normal"

    states: [
        State {
            name: "flat"
            PropertyChanges {
                target: root
                leftMargin: flatButtonBackground.leftMargin
                topMargin: flatButtonBackground.topMargin
                rightMargin: flatButtonBackground.rightMargin
                bottomMargin: flatButtonBackground.bottomMargin
                usedPrefix: flatButtonBackground.usedPrefix
            }
            PropertyChanges {
                target: flatButtonBackground
                visible: true
            }
            PropertyChanges {
                target: normalButtonBackground
                visible: false
            }
        },
        State {
            name: "normal"
            PropertyChanges {
                target: root
                leftMargin: normalButtonBackground.leftMargin
                topMargin: normalButtonBackground.topMargin
                rightMargin: normalButtonBackground.rightMargin
                bottomMargin: normalButtonBackground.bottomMargin
                usedPrefix: normalButtonBackground.usedPrefix
            }
            PropertyChanges {
                target: flatButtonBackground
                visible: false
            }
            PropertyChanges {
                target: normalButtonBackground
                visible: true
            }
        }
    ]
}
