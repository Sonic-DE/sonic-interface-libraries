/*
    SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

pragma Singleton

import QtQuick 2.4
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.kirigami 2.16 as Kirigami

Kirigami.BasicThemeDefinition {
    textColor: PlasmaCore.Theme.textColor
    disabledTextColor: Qt.rgba(PlasmaCore.Theme.textColor.r, PlasmaCore.Theme.textColor.g, PlasmaCore.Theme.textColor.b, 0.6)

    highlightColor: PlasmaCore.Theme.highlightColor
    highlightedTextColor: PlasmaCore.Theme.highlightedTextColor
    backgroundColor: PlasmaCore.Theme.backgroundColor
    alternateBackgroundColor: Qt.darker(PlasmaCore.Theme.backgroundColor, 1.05)

    focusColor: PlasmaCore.Theme.highlightColor
    hoverColor: PlasmaCore.Theme.highlightColor

    //TODO: don't make this invisible
    activeTextColor: PlasmaCore.Theme.highlightColor
    linkColor: PlasmaCore.Theme.linkColor
    visitedLinkColor: PlasmaCore.Theme.visitedLinkColor
    negativeTextColor: PlasmaCore.Theme.negativeTextColor
    neutralTextColor: PlasmaCore.Theme.neutralTextColor
    positiveTextColor: PlasmaCore.Theme.positiveTextColor

    buttonTextColor: PlasmaCore.Theme.buttonTextColor
    buttonBackgroundColor: PlasmaCore.Theme.buttonBackgroundColor
    buttonAlternateBackgroundColor: Qt.darker(PlasmaCore.Theme.buttonBackgroundColor, 1.05)
    buttonHoverColor: PlasmaCore.Theme.buttonHoverColor
    buttonFocusColor: PlasmaCore.Theme.buttonFocusColor

    viewTextColor: PlasmaCore.Theme.viewTextColor
    viewBackgroundColor: PlasmaCore.Theme.viewBackgroundColor
    viewAlternateBackgroundColor: Qt.darker(PlasmaCore.Theme.viewBackgroundColor, 1.05)
    viewHoverColor: PlasmaCore.Theme.viewHoverColor
    viewFocusColor: PlasmaCore.Theme.viewFocusColor

    selectionTextColor: PlasmaCore.Theme.highlightedTextColor
    selectionBackgroundColor: PlasmaCore.Theme.highlightColor
    selectionAlternateBackgroundColor: Qt.darker(PlasmaCore.Theme.highlightColor, 1.05)
    selectionHoverColor: PlasmaCore.Theme.buttonHoverColor
    selectionFocusColor: PlasmaCore.Theme.buttonFocusColor

    tooltipTextColor: PlasmaCore.Theme.complementaryTextColor
    tooltipBackgroundColor: PlasmaCore.Theme.complementaryBackgroundColor
    tooltipAlternateBackgroundColor: Qt.darker(PlasmaCore.Theme.complementaryBackgroundColor, 1.05)
    tooltipHoverColor: PlasmaCore.Theme.complementaryHoverColor
    tooltipFocusColor: PlasmaCore.Theme.complementaryFocusColor

    complementaryTextColor: PlasmaCore.Theme.complementaryTextColor
    complementaryBackgroundColor: PlasmaCore.Theme.complementaryBackgroundColor
    complementaryAlternateBackgroundColor: Qt.darker(PlasmaCore.Theme.complementaryBackgroundColor, 1.05)
    complementaryHoverColor: PlasmaCore.Theme.complementaryHoverColor
    complementaryFocusColor: PlasmaCore.Theme.complementaryFocusColor

    headerTextColor: PlasmaCore.Theme.headerTextColor
    headerBackgroundColor: PlasmaCore.Theme.headerBackgroundColor
    headerAlternateBackgroundColor: Qt.darker(PlasmaCore.Theme.headerBackgroundColor, 1.05)
    headerHoverColor: PlasmaCore.Theme.headerHoverColor
    headerFocusColor: PlasmaCore.Theme.headerFocusColor

    defaultFont: PlasmaCore.Theme.defaultFont
    smallFont: PlasmaCore.Theme.smallestFont
}
