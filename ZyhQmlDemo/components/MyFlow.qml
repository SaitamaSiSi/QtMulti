import QtQuick 2.0

Rectangle {
    id: root
    width: 160; height: 160
    color: "yellow"
    Flow {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20
        RedSquare {}
        BlueSquare {}
        GreenSquare {}
    }
}

