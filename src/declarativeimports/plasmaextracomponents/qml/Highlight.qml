// SPDX-FileCopyrightText: 2011 Daker Fernandes Pinheiro <dakerfp@gmail.com>
// SPDX-FileCopyrightText: 2021 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: LGPL-2.0-or-later

import QtQuick 2.15
import org.kde.plasma.core 2.0 as PlasmaCore

/**
 * @brief Highlight for a list or grid item.
 *
 * Highlight provides the highlight used to indicate the active
 * item in a model view. It is typically used in conjunction with
 * the @sa QtQuick.ListView::highlight or the
 * @sa QtQuick.GridView::highlight properties.
 *
 * Provides built-in animation of Behavior on opacity Easing.OutQuad for a
 * duration of 50ms (defined in PlasmaCore.Units.veryShortDuration).
 *
 * @code{.qml}
 * import QtQuick 2.15
 * import org.kde.plasma.extras 2.0 as PlasmaExtras
 *
 * ListView {
 *     highlightFollowsCurrentItem: true
 *     highlight: PlasmaExtras.Highlight { }
 *     highlightMoveDuration: 0
 *     highlightResizeDuration: 0
 *     currentIndex: -1
 * }
 *
 * @endcode
 *
 * @inherit QtQuick.Item
 */
Item {
    id: highlight

    /**
     * This property holds whether the control is hovered.
     */
    property bool hovered: ListView ? true : false
    // in the case we are the highlight of a listview, it follows the mouse, so hover = true

    /**
     * This property holds whether the highlight has a pressed appearance.
     */
    property bool pressed: false

    width: ListView.view ? ListView.view.width : undefined

    Behavior on opacity {
        NumberAnimation {
            duration: PlasmaCore.Units.veryShortDuration
            easing.type: Easing.OutQuad
        }
    }

    PlasmaCore.FrameSvgItem {
        id: background
        imagePath: "widgets/viewitem"

        opacity: if (highlight.ListView && highlight.ListView.view && highlight.ListView.view.currentIndex < 0) {
            return 0;
        } else if (highlight.GridView && highlight.GridView.view && highlight.GridView.view.currentIndex < 0) {
            return 0;
        } else {
            return 1;
        }

        prefix: {
            if (pressed) {
                return hovered ? "selected+hover" : "selected";
            }

            if (hovered) {
                return "hover";
            }

            return "normal";
        }

        Behavior on opacity {
            NumberAnimation {
                duration: PlasmaCore.Units.veryShortDuration
                easing.type: Easing.OutQuad
            }
        }

        anchors.fill: parent
    }
}
