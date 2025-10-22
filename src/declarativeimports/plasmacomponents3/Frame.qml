/*
    SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import org.kde.kirigami as Kirigami
import org.kde.ksvg as KSvg
//NOTE: importing PlasmaCore is necessary in order to make KSvg load the current Plasma Theme
import org.kde.plasma.core as PlasmaCore

/**
 * @brief A simple frame with custom styling to better fit the plasma environment.
 * @inherit QtQuick.Controls.Frame
 */
T.Frame {
    id: control

    implicitWidth: contentWidth + leftPadding + rightPadding
    implicitHeight: contentHeight + topPadding + bottomPadding

    contentWidth: contentItem.implicitWidth || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    padding: Kirigami.Units.smallSpacing

    background: KSvg.FrameSvgItem {
        imagePath: "widgets/frame"
        prefix: "plain"
    }
}
