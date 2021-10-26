/*
    SPDX-FileCopyrightText: 2017 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2017 The Qt Company Ltd.

    SPDX-License-Identifier: LGPL-3.0-only OR GPL-2.0-or-later
*/


import QtQuick 2.15
import QtQuick.Templates @QQC2_VERSION@ as T
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.kirigami 2.20 as Kirigami

T.ScrollView {
    id: controlRoot

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    leftPadding: mirrored && T.ScrollBar.vertical && T.ScrollBar.vertical.visible && !Kirigami.Settings.isMobile ? T.ScrollBar.vertical.width : 0
    rightPadding: !mirrored && T.ScrollBar.vertical && T.ScrollBar.vertical.visible && !Kirigami.Settings.isMobile ? T.ScrollBar.vertical.width : 0
    bottomPadding: T.ScrollBar.horizontal && T.ScrollBar.horizontal.visible && !Kirigami.Settings.isMobile ? T.ScrollBar.horizontal.height : 0

    data: [
        Kirigami.ScrollHandler {
            target: controlRoot.contentItem
            verticalScrollBar: controlRoot.ScrollBar.vertical
            horizontalScrollBar: controlRoot.ScrollBar.horizontal
        }
    ]

    ScrollBar.vertical: ScrollBar {
        readonly property Flickable flickableItem: controlRoot.contentItem
        onFlickableItemChanged: {
            flickableItem.clip = true;
        }
        parent: controlRoot
        x: controlRoot.mirrored ? 0 : controlRoot.width - width
        y: controlRoot.topPadding
        height: controlRoot.availableHeight
        active: controlRoot.ScrollBar.vertical && controlRoot.ScrollBar.vertical.active
    }

    ScrollBar.horizontal: ScrollBar {
        parent: controlRoot
        x: controlRoot.leftPadding
        y: controlRoot.height - height
        width: controlRoot.availableWidth
        active: controlRoot.ScrollBar.horizontal && controlRoot.ScrollBar.horizontal.active
    }
}
