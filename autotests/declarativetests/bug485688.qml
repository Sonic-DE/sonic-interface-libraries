// SPDX-FileCopyrightText: 2024 Fushan Wen <qydwhotmail@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

import QtQuick
import QtQuick.Layouts
import QtTest

import org.kde.plasma.components as PC3
import org.kde.plasma.extras as PlasmaExtras

// Make sure the text field and the button have (almost) the same height
TestCase {
    id: root
    width: 640
    height: 480
    when: windowShown

    RowLayout {
        width: parent.width
        PlasmaExtras.SearchField {
            id: searchField
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.fillWidth: true
        }
        PC3.ToolButton {
            id: pinButton
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            icon.name: "window-pin"
            text: "Keep Open"
            display: PC3.ToolButton.IconOnly
        }
    }

    function test_compareHeight() {
        verify(searchField.implicitHeight > 0);
        fuzzyCompare(searchField.implicitHeight, pinButton.implicitHeight, 1.0 /* For taller placeholder text in some languages */);
    }
}
