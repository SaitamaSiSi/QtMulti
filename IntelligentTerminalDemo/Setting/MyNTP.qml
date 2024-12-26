import QtQuick 2.0

import "../Components"

Item {
    id: root
    signal clicked(int type, string msg)

    Rectangle {
        width: parent.width / 3
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: (parent.width + 170 + 50 + 25) / 2 - 170 - 50 - width / 2
        color: "transparent"
        Column {
            width: parent.width
            height: parent.height - 20
            anchors.top: parent.top
            anchors.topMargin: 20
            spacing: 30
            Row {
                width: parent.width
                height: parent.height / 4
                Text {
                    text: "IP："
                    color: "white"
                    width: parent.width / 3
                    height: parent.height
                    font.pixelSize: 24
                    horizontalAlignment: TextEdit.AlignHCenter
                    verticalAlignment: TextEdit.AlignVCenter
                }
                MyInput {
                    width: parent.width / 3 * 2
                    height: 24
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row {
                width: parent.width
                height: parent.height / 4
                Text {
                    text: "端口："
                    color: "white"
                    width: parent.width / 3
                    height: parent.height
                    font.pixelSize: 24
                    horizontalAlignment: TextEdit.AlignHCenter
                    verticalAlignment: TextEdit.AlignVCenter
                }
                MyInput {
                    width: parent.width / 3 * 2
                    height: 24
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            MyTextButton {
                id: checkBt
                width: parent.width
                text: "保存"
                radius: 20
                onClicked: {
                    root.clicked(1, "ok")
                }
            }
        }
    }
}

