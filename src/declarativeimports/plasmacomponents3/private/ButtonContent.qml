/*
 * SPDX-FileCopyrightText: 2020 Noah Davis <noahadvs@gmail.com>
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Templates @QQC2_VERSION@ as T
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.kirigami 2.12 as Kirigami
import "../" as PlasmaComponents

GridLayout {
    id: root
    // TODO: mark this required when our minimum Qt version is new enough
    property T.Button parentControl

    readonly property bool useHighlightedTextColor: !parentControl.flat && buttonSvg.hasElement("hint-focus-highlighted-background") && parentControl.visualFocus

    // See states below.
    // The restoreEntryValues property of PropertyChanges is true by default.
    rows: 1
    columns: 1

    rowSpacing: PlasmaCore.Units.smallSpacing
    columnSpacing: PlasmaCore.Units.smallSpacing

    PlasmaCore.Svg {
        id: buttonSvg
        imagePath: "widgets/button"
    }

    PlasmaCore.IconItem {
        id: icon

        implicitWidth: {
            if (parentControl.icon.width > 0) {
                return parentControl.icon.width;
            } else if (parentControl.flat) {
                return PlasmaCore.Units.iconSizes.smallMedium;
            } else {
                return PlasmaCore.Units.iconSizes.small;
            }
        }
        implicitHeight: {
            if (parentControl.icon.height > 0) {
                return parentControl.icon.height;
            } else if (parentControl.flat) {
                return PlasmaCore.Units.iconSizes.smallMedium;
            } else {
                return PlasmaCore.Units.iconSizes.small;
            }
        }

        Layout.alignment: Qt.AlignCenter

        colorGroup: PlasmaCore.ColorScope.colorGroup
        visible: source.length > 0
        source: parentControl.icon ? (parentControl.icon.name || parentControl.icon.source) : ""
        status: root.useHighlightedTextColor ? PlasmaCore.Svg.Selected : PlasmaCore.Svg.Normal
    }

    PlasmaComponents.Label {
        id: label
        Layout.fillWidth: true
        visible: text.length > 0
        text: parentControl.Kirigami.MnemonicData.richTextLabel
        font: parentControl.font
        color: root.useHighlightedTextColor ? PlasmaCore.ColorScope.highlightedTextColor : PlasmaCore.ColorScope.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        Behavior on color {
            ColorAnimation {
                duration: units.shortDuration
                easing.type: Easing.OutQuad
            }
        }
    }
    
    states: [
        State {
            name: "IconOnly"
            when: parentControl.display === T.Button.IconOnly
            PropertyChanges {
                target: root
                implicitWidth: icon.implicitWidth
                implicitHeight: icon.implicitHeight
            }
            PropertyChanges {
                target: label
                visible: false
            }
        },
        State {
            name: "TextOnly"
            when: parentControl.display === T.Button.TextOnly
            PropertyChanges {
                target: root
                implicitWidth: label.implicitWidth
                implicitHeight: label.implicitHeight
            }
            PropertyChanges {
                target: icon
                visible: false
            }
        },
        State {
            name: "TextBesideIcon"
            when: parentControl.display === T.Button.TextBesideIcon
            PropertyChanges {
                target: root
                implicitWidth: icon.implicitWidth + root.columnSpacing + label.implicitWidth
                implicitHeight: Math.max(icon.implicitWidth, label.implicitWidth)
                rows: 1
                columns: 2
            }
            PropertyChanges {
                target: label
                horizontalAlignment: Text.AlignLeft
            }
        },
        State {
            name: "TextUnderIcon"
            when: parentControl.display === T.Button.TextUnderIcon
            PropertyChanges {
                target: root
                implicitWidth: Math.max(icon.implicitWidth, label.implicitWidth)
                implicitHeight: icon.implicitHeight + root.rowSpacing + label.implicitHeight
                rows: 2
                columns: 1
            }
        }
    ]
}
