import QtQuick 2.0

Item {
    id: root_button
    property alias text: label.text
    signal clicked
    width: 116; height: 26
    Rectangle {
        anchors.fill: parent
        color: "lightsteelblue"
        border.color: "slategrey"
    }
    Text {
        id: label
        anchors.centerIn: parent
        text: qsTr("ok")
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            root_button.clicked()
        }
    }
}
