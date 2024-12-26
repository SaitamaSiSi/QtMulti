import QtQuick 2.0

Rectangle {
    id: root
    width: 400; height: 120
    color: "yellow"
    Row {
        id: row
        anchors.centerIn: parent
        spacing: 20
        BlueSquare {}
        GreenSquare {}
        RedSquare {}
    }
}

