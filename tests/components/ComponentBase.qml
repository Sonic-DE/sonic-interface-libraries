import QtQuick 2.0
import QtQuick.Layouts 1.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as PlasmaComponents

PlasmaComponents.Page {
    id: mainPage
    title: "Test"
    width: Math.max(contentWidth, implicitHeaderWidth, implicitFooterWidth) + leftPadding + rightPadding
    height: contentHeight + implicitHeaderHeight + implicitFooterHeight + topPadding + bottomPadding

    padding: PlasmaCore.Units.gridUnit

    header: PlasmaComponents.ToolBar {
        implicitWidth: toolBarRowLayout.implicitWidth + leftPadding + rightPadding
        leftPadding: mainPage.padding
        rightPadding: mainPage.padding
        contentItem: RowLayout {
            id: toolBarRowLayout
            spacing: PlasmaCore.Units.smallSpacing
            PlasmaComponents.Label {
                id: titleLabel
                text: mainPage.title
                Layout.alignment: Qt.AlignLeft
            }
        }
    }

    background: Rectangle {
        color:  PlasmaCore.ColorScope.backgroundColor
    }
}

