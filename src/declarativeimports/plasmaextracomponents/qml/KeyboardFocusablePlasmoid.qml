/*
    SPDX-FileCopyrightText: 2021 Benjamin Port <benjamin.port@enioka.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.15

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    activeFocusOnTab: {
        let item = this
        while (item.parent) {
            item = item.parent
            if (item.disableKeyboardFocusable) {
                return false
            }
        }
        return true
    }
    anchors.fill: parent
    Keys.onPressed: {
        switch (event.key) {
            case Qt.Key_Space:
            case Qt.Key_Enter:
            case Qt.Key_Return:
                parent.Plasmoid.onActivated()
                break;
        }
    }
    property var appletContainer: {
        let item = this
        while (item.parent) {
            item = item.parent
            if (item.isAppletContainer) {
                return item
            }
        }
        return undefined;
    }

    PlasmaCore.FrameSvgItem {
        x: parent.appletContainer ? parent.x - parent.appletContainer.getMargins('left') : parent.x
        y: parent.appletContainer ? parent.y - parent.appletContainer.getMargins('top') : parent.y
        width: parent.appletContainer ? parent.width + parent.appletContainer.getMargins('left') + parent.appletContainer.getMargins('right') : parent.witdth
        height: parent.appletContainer ? parent.height + parent.appletContainer.getMargins('top') + parent.appletContainer.getMargins('bottom') : parent.height

        z: -1 // always draw behind icons
        opacity: {
            return parent.activeFocus ? 1 : 0
        }

        imagePath: "widgets/tabbar"
        prefix: {
            var prefix = ""
            switch (plasmoid.location) {
                case PlasmaCore.Types.LeftEdge:
                    prefix = "west-active-tab";
                    break;
                case PlasmaCore.Types.TopEdge:
                    prefix = "north-active-tab";
                    break;
                case PlasmaCore.Types.RightEdge:
                    prefix = "east-active-tab";
                    break;
                default:
                    prefix = "south-active-tab";
            }
            if (!hasElementPrefix(prefix)) {
                prefix = "active-tab";
            }
            return prefix;
        }
    }
}
