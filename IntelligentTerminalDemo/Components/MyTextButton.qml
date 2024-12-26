import QtQuick 2.0

Item {
    id: root_button
    property alias text: label.text
    property alias radius: labelRect.radius
    property bool chcked: false
    property string bgColor: "transparent"
    signal clicked
    property int btnWidth: 80
    property int btnHeight: 40
    width: btnWidth; height: btnHeight
    Rectangle {
        id: labelRect
        anchors.fill: parent
        color: chcked ? "skyblue" : bgColor
        border.color: "lightblue"
        radius: 0
        Text {
            id: label
            anchors.centerIn: parent
            color: "white"
            text: ""
            font.pointSize: 16
            horizontalAlignment: TextEdit.AlignRight
            verticalAlignment: TextEdit.AlignVCenter
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            root_button.clicked()
        }
    }
}
