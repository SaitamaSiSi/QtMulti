import QtQuick 2.0

import "../Components"

Item {
    id: root_door
    Column {
        width: parent.width
        height: parent.height
        z: 1
        Rectangle {
            id: tableArea
            width: parent.width
            height: parent.height / 7 * 6
            color: "transparent"
            Column {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.left: parent.left
                anchors.leftMargin: 10
                Rectangle {
                    width: parent.width
                    height: parent.height
                    color: "transparent"
                    border.color: "gray"
                    Image {
                        id: back_door
                        source: "qrc:/resources/images/柜门-后开1-2.png"
                        width: parent.width / 4 * 3
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width / 8
                        height: (parent.height  - back_text_area.height) / 4 * 3
                        anchors.top: parent.top
                        anchors.topMargin: (parent.height  - back_text_area.height) / 8
                    }
                    Row {
                        id: back_text_area
                        width: parent.width / 4 * 3
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width / 6
                        height: parent.height / 6
                        anchors.top: back_door.bottom
                        anchors.topMargin: (parent.height  - back_text_area.height) / 16
                        Text {
                            text: "左前门："
                            color: "white"
                            width: parent.width / 4
                            height: parent.height
                            font.pixelSize: 18
                            verticalAlignment: TextEdit.AlignVCenter
                        }
                        Text {
                            text: "开启"
                            color: "red"
                            width: parent.width / 4
                            height: parent.height
                            font.pixelSize: 24
                            verticalAlignment: TextEdit.AlignVCenter
                        }
                        Text {
                            text: "右前门："
                            color: "white"
                            width: parent.width / 4
                            height: parent.height
                            font.pixelSize: 18
                            verticalAlignment: TextEdit.AlignVCenter
                        }
                        Text {
                            text: "开启"
                            color: "red"
                            width: parent.width / 4
                            height: parent.height
                            font.pixelSize: 24
                            verticalAlignment: TextEdit.AlignVCenter
                        }
                    }
                }
            }
            Column {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.right: parent.right
                anchors.rightMargin: 10
                Rectangle {
                    width: parent.width
                    height: parent.height
                    color: "transparent"
                    border.color: "gray"
                    Image {
                        id: front_door
                        source: "qrc:/resources/images/柜门-前开0.png"
                        width: parent.width / 4 * 3
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width / 8
                        height: (parent.height  - front_text_area.height) / 4 * 3
                        anchors.top: parent.top
                        anchors.topMargin: (parent.height  - front_text_area.height) / 8
                    }
                    Row {
                        id: front_text_area
                        width: parent.width / 4 * 3
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width / 6
                        height: parent.height / 6
                        anchors.top: front_door.bottom
                        anchors.topMargin: (parent.height  - front_text_area.height) / 16
                        Text {
                            text: "左后门："
                            color: "white"
                            width: parent.width / 4
                            height: parent.height
                            font.pixelSize: 18
                            verticalAlignment: TextEdit.AlignVCenter
                        }
                        Text {
                            text: "关闭"
                            color: "green"
                            width: parent.width / 4
                            height: parent.height
                            font.pixelSize: 24
                            verticalAlignment: TextEdit.AlignVCenter
                        }
                        Text {
                            text: "右后门："
                            color: "white"
                            width: parent.width / 4
                            height: parent.height
                            font.pixelSize: 18
                            verticalAlignment: TextEdit.AlignVCenter
                        }
                        Text {
                            text: "关闭"
                            color: "green"
                            width: parent.width / 4
                            height: parent.height
                            font.pixelSize: 24
                            verticalAlignment: TextEdit.AlignVCenter
                        }
                    }
                }
            }
        }
        Rectangle {
            id: buttonArea
            width: parent.width
            height: parent.height / 7
            color: "transparent"

            MyButton {
                text: "功能键一"
                anchors.right: rightButton.left
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                imgSource: "qrc:/resources/images/02_记录.png"
                borderWidth: 1
                borderRadius: 5
                borderColor: "white"
                onClicked: {
                    console.debug("功能键一")
                }
            }
            MyButton {
                id: rightButton
                text: "功能键二"
                anchors.right: parent.right
                anchors.rightMargin: 30
                anchors.verticalCenter: parent.verticalCenter
                imgSource: "qrc:/resources/images/02_设置.png"
                borderWidth: 1
                borderRadius: 5
                borderColor: "white"
                onClicked: {
                    console.debug("功能键二")
                }
            }
        }
    }
}
