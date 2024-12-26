import QtQuick 2.0

Rectangle {
    id: root
    width: 120; height: 240
    color: "black"
    Column {
        id: column
        anchors.centerIn: parent
        spacing: 8
        RedSquare {}
        GreenSquare {}
        BlueSquare {}
    }
}
