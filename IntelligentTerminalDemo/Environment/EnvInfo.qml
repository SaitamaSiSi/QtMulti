import QtQuick 2.0

import "../Components"

Item {
    id: root_env
    Column {
        width: parent.width
        height: parent.height
        z: 1
        Rectangle {
            id: tableArea
            height: parent.height / 7 * 6
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            color: "transparent"
            Row {
                width: parent.width - 20
                height: parent.height
                spacing: 10
                Rectangle {
                    width: parent.width / 3
                    height: parent.height
                    color: "transparent"
                    border.color: "gray"
                    Text {
                        text: "大类一"
                        color: "white"
                        font.pixelSize: 20
                        anchors.verticalCenter: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                    }
                    Column {
                        width: parent.width
                        height: parent.height / 3 * 2
                        anchors.top: parent.top
                        anchors.topMargin: parent.height / 6

                        Row {
                            width: parent.width
                            height: parent.height / 4
                            Text {
                                text: "检测一："
                                color: "gray"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 18
                                horizontalAlignment: TextEdit.AlignRight
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                text: "XXXXXX"
                                color: "skyblue"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 24
                                horizontalAlignment: TextEdit.AlignLeft
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                        }
                        Row {
                            width: parent.width
                            height: parent.height / 4
                            Text {
                                text: "检测二："
                                color: "gray"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 18
                                horizontalAlignment: TextEdit.AlignRight
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                text: "XXXXXX"
                                color: "yellow"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 24
                                horizontalAlignment: TextEdit.AlignLeft
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                        }
                        Row {
                            width: parent.width
                            height: parent.height / 4
                            Text {
                                text: "检测三："
                                color: "gray"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 18
                                horizontalAlignment: TextEdit.AlignRight
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                text: "XXXXXX"
                                color: "skyblue"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 24
                                horizontalAlignment: TextEdit.AlignLeft
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                        }
                        Row {
                            width: parent.width
                            height: parent.height / 4
                            Text {
                                text: "检测四："
                                color: "gray"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 18
                                horizontalAlignment: TextEdit.AlignRight
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                text: "XXXXXX"
                                color: "red"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 24
                                horizontalAlignment: TextEdit.AlignLeft
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                        }
                    }
                }
                Rectangle {
                    width: parent.width / 3
                    height: parent.height
                    color: "transparent"
                    border.color: "gray"
                    Text {
                        text: "大类二"
                        color: "white"
                        font.pixelSize: 18
                        anchors.verticalCenter: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                    }
                    Column {
                        width: parent.width
                        height: parent.height / 3 * 2
                        anchors.top: parent.top
                        anchors.topMargin: parent.height / 6

                        Row {
                            width: parent.width
                            height: parent.height
                            Text {
                                text: "检测一："
                                color: "gray"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 18
                                horizontalAlignment: TextEdit.AlignRight
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                text: "XXXXXX"
                                color: "green"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 24
                                horizontalAlignment: TextEdit.AlignLeft
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                        }
                    }
                }
                Rectangle {
                    width: parent.width / 3
                    height: parent.height
                    color: "transparent"
                    border.color: "gray"
                    Text {
                        text: "大类三"
                        color: "white"
                        font.pixelSize: 18
                        anchors.verticalCenter: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                    }
                    Column {
                        width: parent.width
                        height: parent.height / 3 * 2
                        anchors.top: parent.top
                        anchors.topMargin: parent.height / 6

                        Row {
                            width: parent.width
                            height: parent.height
                            Text {
                                text: "检测一："
                                color: "gray"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 18
                                horizontalAlignment: TextEdit.AlignRight
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                text: "XXXXXX"
                                color: "green"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 24
                                horizontalAlignment: TextEdit.AlignLeft
                                verticalAlignment: TextEdit.AlignVCenter
                            }
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
                id: rightButton
                text: "功能键一"
                anchors.right: parent.right
                anchors.rightMargin: 30
                anchors.verticalCenter: parent.verticalCenter
                imgSource: "qrc:/resources/images/02_设置.png"
                borderWidth: 1
                borderRadius: 5
                borderColor: "white"
                onClicked: {
                    console.debug("功能键一")
                }
            }
        }
    }
}
