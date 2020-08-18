import QtQuick 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.2
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

ComponentBase {
    id: root
    title: "Plasma Components 2 CheckBox"
    contentItem: GridLayout {
        rowSpacing: PlasmaCore.Units.smallSpacing
        columnSpacing: PlasmaCore.Units.largeSpacing
        columns: 2

        Label {
            text: "text"
        }
        PlasmaComponents.CheckBox {
            text: "Some awesome checkbox"
        }

        Label {
            text: "focus"
        }
        PlasmaComponents.CheckBox {
            text: "Some awesome checkbox"
            focus: true
        }

        Label {
            text: "checked"
        }
        PlasmaComponents.CheckBox {
            text: "Some awesome checkbox"
            checkedState: Qt.Checked
        }

        Label {
            text: "tri-state"
        }
        PlasmaComponents.CheckBox {
            text: "Some awesome checkbox"
            checkedState: Qt.PartiallyChecked
        }

        Label {
            text: "disabled"
        }
        PlasmaComponents.CheckBox {
            text: "Some awesome checkbox"
            enabled: false
        }

        Label {
            text: "disabled and checked"
        }
        PlasmaComponents.CheckBox {
            text: "Some awesome checkbox"
            enabled: false
            checkedState: Qt.Checked
        }
    }
}

