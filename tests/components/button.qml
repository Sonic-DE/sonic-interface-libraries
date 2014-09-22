import QtQuick 2.0

import org.kde.plasma.components 2.0 as PlasmaComponents

Rectangle
{
    width: 500
    height: 300
    color: "white"

    Flow {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        PlasmaComponents.Button {
            iconSource: "list-remove"
            text: "test"
        }
        PlasmaComponents.Button{
            iconSource: "list-remove"
        }
        PlasmaComponents.Button{
            text: "test"
        }

    }
}

