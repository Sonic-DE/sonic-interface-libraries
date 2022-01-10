// SPDX-FileCopyrightText: 2019 Carl-Lucien Schwan <carl@carlschwan.eu>
// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: LGPL-2.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.plasma.components 3.0 as PlasmaComponents3
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.core 2.0 as PlasmaCore

/**
 * This is a standard textfield following KDE HIG. Using Ctrl+F as focus
 * sequence and "Search..." as placeholder text.
 *
 * Example usage for the search field component:
 * @code
 * import org.kde.plasma.extras 2.0 as PlasmaExtras
 *
 * PlasmaExtras.SearchField {
 *     id: searchField
 *     onAccepted: console.log("Search text is " + searchField.text)
 * }
 * @endcode
 *
 * @inherit org::plasmas::extras::ActionTextField
 */
PlasmaExtras.ActionTextField {
    id: root
    /**
     * This properperty determines whether the accepted signal will be fired automatically
     * when the text is changed.
     *
     * Setting this to false will require that the user presses return or enter (the same
     * way a QML.TextInput works).
     *
     * The default value is false
     *
     * @since 5.92
     */
    property bool autoAccept: false

    /**
     * This property defines the delays the automatic acceptance of the input
     * further (by 2.5 seconds).
     *
     * Set this to true if your search is expensive (such as for online
     * operations or in exceptionally slow data sets).
     *
     * \note If you must have immediate feedback (filter-style), use the
     * text property directly instead of accepted()
     *
     * The default value is false
     *
     * @since 5.92
     */
    property bool delaySearch: false

    property int _iconWidth: (activeFocus || root.text.length > 0 ? 0 : searchIcon.width)

    // padding to accommodate search icon nicely
    leftPadding: if (root.mirrored) {
        return _rightActionsRow.width + (__hasBackgroundAndMargins ? background.margins.left : 0);
    } else {
        return _iconWidth + _leftActionsRow.width + (__hasBackgroundAndMargins ? background.margins.left : 0);
    }
    rightPadding: if (root.mirrored) {
        return _iconWidth + _leftActionsRow.width + (__hasBackgroundAndMargins ? background.margins.right : 0);
    } else {
        return _rightActionsRow.width + (__hasBackgroundAndMargins ? background.margins.right : 0);
    }

    PlasmaCore.IconItem {
        id: searchIcon
        opacity: parent.activeFocus || text.length > 0 ? 0 : 1
        anchors.left: parent.left
        anchors.leftMargin: PlasmaCore.Units.smallSpacing * 2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Math.round((parent.implicitHeight - implicitHeight) / 2 + (parent.bottomPadding - parent.topPadding) / 2)
        implicitHeight: PlasmaCore.Units.iconSizes.sizeForLabels
        implicitWidth: PlasmaCore.Units.iconSizes.sizeForLabels
        // TODO placeholder color

        source: "search"

        Behavior on opacity {
            NumberAnimation {
                duration: PlasmaCore.Units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
    }

    placeholderText: i18nd("libplasma5", "Search…")

    Accessible.name: i18nd("libplasma5", "Search")
    Accessible.searchEdit: true

    focusSequence: StandardKey.Find
    inputMethodHints: Qt.ImhNoPredictiveText
    rightActions: QQC2.Action {
        icon.name: root.mirrored ? "edit-clear-locationbar-ltr" : "edit-clear-locationbar-rtl"
        enabled: root.text.length > 0
        onTriggered: {
            root.text = "";
            // Since we are always sending the accepted signal here (whether or not the user has requested
            // that the accepted signal be delayed), stop the delay timer that gets started by the text changing
            // above, so that we don't end up sending two of those in rapid succession.
            fireSearchDelay.stop();
            root.accepted();
        }
    }

    Timer {
        id: fireSearchDelay
        interval: root.delaySearch ? PlasmaCore.Units.humanMoment : PlasmaCore.Units.shortDuration
        running: false
        repeat: false
        onTriggered: root.accepted()
    }
    onAccepted: {
        fireSearchDelay.running = false
    }
    onTextChanged: {
        if (root.autoAccept) {
            fireSearchDelay.restart();
        } else {
            fireSearchDelay.stop();
        }
    }
}
