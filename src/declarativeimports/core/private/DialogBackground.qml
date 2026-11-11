/*
 *  SPDX-FileCopyrightText: 2023 Marco Martin <notmart@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick
import QtQuick.Effects
import org.kde.ksvg as KSvg

/*!
 * Internal type used by DialogBackground in plasmaquick to load the actual
 * background SVG.
 */
KSvg.FrameSvgItem {
    id: background

    // PlasmaWindow::backgroundCutout, in this item's coordinates: left out of the background.
    property rect cutout
    readonly property bool hasCutout: cutout.width > 0 && cutout.height > 0

    anchors.fill: parent
    imagePath: "widgets/background"

    // Only with a cutout: otherwise drawn as before, without a layer.
    layer.enabled: hasCutout
    layer.effect: MultiEffect {
        maskEnabled: true
        maskInverted: true
        maskSource: cutoutMask
    }

    Item {
        id: cutoutMask
        anchors.fill: parent
        visible: false
        layer.enabled: background.hasCutout

        Rectangle {
            x: background.cutout.x
            y: background.cutout.y
            width: background.cutout.width
            height: background.cutout.height
            color: "white"
        }
    }
}
