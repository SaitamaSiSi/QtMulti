import QtQuick 2.0

Rectangle {
    id: root
    width: 160; height: 160
    color: "yellow"
    Grid {
        id: grid
        rows: 2; columns: 2
        anchors.centerIn: parent
        spacing: 8
        RedSquare {}
        BlueSquare {}
        GreenSquare {}
        RedSquare {}
    }
}
