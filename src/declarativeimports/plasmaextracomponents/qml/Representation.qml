/*
    SPDX-FileCopyrightText: 2020 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as PlasmaComponents
import org.kde.plasma.components 2.0 as PlasmaExtras

 /**
  * Item to be used as root item for Full representations of plasmoids.
  * It's a QtQuickControls2 Page, and like that one, has an header, a contentItem and a Footer
  * It may go over the plasmoid edges (both on desktop and popups) with the properties applyHorizontalPadding and applyVerticalPadding.
  * When the contentItem is a ScrollView or a Scrollarea, the plasmoid margins will be automatically removed.
  *
  * This code will create a full representation with a listview that will automatically 
  * full the whole area without margins from the plasmoid popup borders
  * @code
  * Plasmoid.FullRepresentation: PlasmaExtras.FullRepresentation {
  *     header: PlasmaExtras.BasicPlasmoidHeading{}
  *     contentItem: PlasmaComponent.ScrollView {
  *         ListView {
  *             ....
  *         }
  *     }
  * }
  * @endcode
  *
  * @since 5.77
  * @inherit QtQuick.Templates.Page
  */
 PlasmaComponents.Page {
    id: control

    /**
     * applyHorizontalPadding: bool
     * if true, this FullRepresentation mainItem's contents will go over the plasmoids horizontal edges
     */
    property bool applyHorizontalPadding: !headingSvg.showingScrollView

    /**
     * applyHorizontalPadding: bool
     * if true, this FullRepresentation mainItem's contents will go over the plasmoids vertical edges
     */
    property bool applyVerticalPadding: !headingSvg.showingScrollView

    leftInset: headingSvg.leftHint
    rightInset: headingSvg.rightHint
    topInset: headingSvg.topHint
    bottomInset: fixedMargins.bottomHint

    leftPadding: applyHorizontalPadding ? 0 : headingSvg.leftHint
    rightPadding: applyHorizontalPadding ? 0 : headingSvg.rightHint
    topPadding: applyVerticalPadding
                    ? (header ? units.gridUnit : 0)
                    : (header ? 0 : headingSvg.topHint)
    bottomPadding: applyVerticalPadding
                    ? (footer ? units.gridUnit : 0)
                    : (footer ? 0 : headingSvg.bottomHint)

    // Used only for the negative margins
    PlasmaCore.FrameSvgItem {
        id: headingSvg

        property bool isDesktop: false
        readonly property bool showingScrollView: (control.contentItem instanceof PlasmaComponents.ScrollView) || (control.contentItem instanceof ScrollArea)

        readonly property real leftHint: isDesktop ? -fixedMargins.left + shadowMargins.left : -fixedMargins.left
        readonly property real rightHint: isDesktop ? -fixedMargins.right + shadowMargins.right : -fixedMargins.right
        readonly property real topHint: isDesktop ? -fixedMargins.top + shadowMargins.top : -fixedMargins.top
        readonly property real bottomHint: isDesktop ? -fixedMargins.bottom + shadowMargins.bottom : -fixedMargins.bottom

        visible: false

        imagePath: {
            if (control.Window.window && (control.Window.window instanceof PlasmaCore.Dialog)) {
                isDesktop = false;
                return "dialogs/background";
            // Perhaps panel case shouldn't be managed at all?
            } else if (typeof plasmoid !== "undefined" && plasmoid.formFactor == PlasmaCore.Types.Horizontal || PlasmaCore.Types.Vertical) {
                isDesktop = false;
                return "widgets/panel-background";
            } else {
                isDesktop = true;
                return "widgets/background";
            }
        }
    }
 }
