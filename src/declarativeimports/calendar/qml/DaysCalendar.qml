/*
    SPDX-FileCopyrightText: 2013 Heena Mahour <heena393@gmail.com>
    SPDX-FileCopyrightText: 2013 Sebastian Kügler <sebas@kde.org>
    SPDX-FileCopyrightText: 2015, 2016 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1 as QQC1

import org.kde.plasma.calendar 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as Components
import org.kde.plasma.extras 2.0 as PlasmaExtras

Item {
    id: daysCalendar

    signal headerClicked

    signal previous
    signal next

    signal activated(int index, var date, var item)
    // so it forwards it to the delegate which then emits activated with all the necessary data
    signal activateHighlightedItem

    readonly property int gridColumns: showWeekNumbers ? calendarGrid.columns + 1 : calendarGrid.columns

    property alias previousLabel: previousButton.tooltip
    property alias nextLabel: nextButton.tooltip

    property int rows
    property int columns

    property bool showWeekNumbers

    property bool showCustomHeader: false;

    // how precise date matching should be, 3 = day+month+year, 2 = month+year, 1 = just year
    property int dateMatchingPrecision

    property alias headerModel: days.model
    property alias gridModel: repeater.model

    property alias title: heading.text

    // Take the calendar width, subtract the inner and outer spacings and divide by number of columns (==days in week)
    readonly property int cellWidth: Math.floor((stack.width - (daysCalendar.columns + 1) * root.borderWidth) / (daysCalendar.columns + (showWeekNumbers ? 1 : 0)))
    // Take the calendar height, subtract the inner spacings and divide by number of rows (root.weeks + one row for day names)
    readonly property int cellHeight:  Math.floor((stack.height - heading.height - calendarGrid.rows * root.borderWidth) / calendarGrid.rows)

    property real transformScale: 1
    property point transformOrigin: Qt.point(width / 2, height / 2)

    transform: Scale {
        xScale: daysCalendar.transformScale
        yScale: xScale
        origin.x: transformOrigin.x
        origin.y: transformOrigin.y
    }

    Behavior on scale {
        id: scaleBehavior
        ScaleAnimator {
            duration: PlasmaCore.Units.longDuration
        }
    }

    QQC1.Stack.onStatusChanged: {
        if (QQC1.Stack.status === QQC1.Stack.Inactive) {
            daysCalendar.transformScale = 1
            opacity = 1
        }
    }

    RowLayout {
        visible: !showCustomHeader
        spacing: PlasmaCore.Units.smallSpacing

        PlasmaExtras.Heading {
            id: heading

            Layout.fillWidth: true

            level: 2
            elide: Text.ElideRight
            font.capitalization: Font.Capitalize
            //SEE QTBUG-58307
            //try to make all heights an even number, otherwise the layout engine gets confused
            Layout.preferredHeight: implicitHeight + implicitHeight%2

            MouseArea {
                id: monthMouse
                property int previousPixelDelta

                anchors.fill: parent
                onClicked: {
                    if (!stack.busy) {
                        daysCalendar.headerClicked()
                    }
                }
                onExited: previousPixelDelta = 0
                onWheel: {
                    var delta = wheel.angleDelta.y || wheel.angleDelta.x
                    var pixelDelta = wheel.pixelDelta.y || wheel.pixelDelta.x

                    // For high-precision touchpad scrolling, we get a wheel event for basically every slightest
                    // finger movement. To prevent the view from suddenly ending up in the next century, we
                    // cumulate all the pixel deltas until they're larger than the label and then only change
                    // the month. Standard mouse wheel scrolling is unaffected since it's fine.
                    if (pixelDelta) {
                        if (Math.abs(previousPixelDelta) < monthMouse.height) {
                            previousPixelDelta += pixelDelta
                            return
                        }
                    }

                    if (delta >= 15) {
                        daysCalendar.previous()
                    } else if (delta <= -15) {
                        daysCalendar.next()
                    }
                    previousPixelDelta = 0
                }
            }
        }

        Components.ToolButton {
            id: previousButton
            property string tooltip

            icon.name: Qt.application.layoutDirection === Qt.RightToLeft ? "go-next" : "go-previous"
            onClicked: daysCalendar.previous()
            Accessible.name: tooltip
            Components.ToolTip { text: parent.tooltip }
            //SEE QTBUG-58307
            Layout.preferredHeight: implicitHeight + implicitHeight%2
        }

        Components.ToolButton {
            icon.name: "go-jump-today"
            property string tooltip

            onClicked: root.resetToToday()
            tooltip: i18ndc("libplasma5", "Reset calendar to today", "Today")
            Accessible.name: tooltip
            Accessible.description: i18nd("libplasma5", "Reset calendar to today")
            Components.ToolTip { text: parent.tooltip }
            //SEE QTBUG-58307
            Layout.preferredHeight: implicitHeight + implicitHeight%2
        }

        Components.ToolButton {
            id: nextButton
            property string tooltip

            icon.name: Qt.application.layoutDirection === Qt.RightToLeft ? "go-previous" : "go-next"
            onClicked: daysCalendar.next()
            Accessible.name: tooltip
            Components.ToolTip { text: parent.tooltip }
            //SEE QTBUG-58307
            Layout.preferredHeight: implicitHeight + implicitHeight%2
        }
    }

    PlasmaCore.Svg {
        id: calendarSvg
        imagePath: "widgets/calendar"
    }

    Component {
        id: eventsMarkerComponent

        PlasmaCore.SvgItem {
            id: eventsMarker
            svg: calendarSvg
            elementId: "event"
        }
    }

    Column {
        id: weeksColumn
        visible: showWeekNumbers
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
            // The borderWidth needs to be counted twice here because it goes
            // in fact through two lines - the topmost one (the outer edge)
            // and then the one below weekday strings
            topMargin: daysCalendar.cellHeight + root.borderWidth + root.borderWidth
        }
        spacing: root.borderWidth

        Repeater {
            model: showWeekNumbers ? calendarBackend.weeksModel : []

            Components.Label {
                height: daysCalendar.cellHeight
                width: daysCalendar.cellWidth
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: 0.4
                text: modelData
                font.pixelSize: Math.max(PlasmaCore.Theme.smallestFont.pixelSize, daysCalendar.cellHeight / 3)
            }
        }
    }

    Grid {
        id: calendarGrid

        anchors {
            top: parent.top
            right: parent.right
            rightMargin: root.borderWidth
            bottom: parent.bottom
            bottomMargin: root.borderWidth
        }

        columns: daysCalendar.columns
        rows: daysCalendar.rows + (daysCalendar.headerModel ? 1 : 0)

        spacing: root.borderWidth
        property bool containsEventItems: false // FIXME
        property bool containsTodoItems: false // FIXME

        Repeater {
            id: days

            Components.Label {
                width: daysCalendar.cellWidth
                height: daysCalendar.cellHeight
                text: Qt.locale(Qt.locale().uiLanguages[0]).dayName(((calendarBackend.firstDayOfWeek + index) % days.count), Locale.ShortFormat)
                font.pixelSize: Math.max(PlasmaCore.Theme.smallestFont.pixelSize, daysCalendar.cellHeight / 3)
                opacity: 0.4
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                fontSizeMode: Text.HorizontalFit
            }
        }

        Repeater {
            id: repeater

            DayDelegate {
                id: delegate
                width: daysCalendar.cellWidth
                height: daysCalendar.cellHeight

                onClicked: daysCalendar.activated(index, model, delegate)

                Connections {
                    target: daysCalendar
                    onActivateHighlightedItem: {
                        if (delegate.containsMouse) {
                            delegate.clicked(null)
                        }
                    }
                }
            }
        }
    }
}
