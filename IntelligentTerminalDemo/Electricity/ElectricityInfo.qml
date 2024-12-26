import QtQuick 2.0

import VoltageMonitoringDataModel 1.0

import "../Components"

Item {
    id: root_elect
    VoltageMonitoringDataModel {
        id: acModel
    }
    VoltageMonitoringDataModel {
        id: dcModel
    }
    Image {
        id: bgicon_img
        source: "qrc:/resources/images/首页电力状态分割线.png"
        anchors.centerIn: parent
        width: 2
        height: parent.height
        z: 0
    }
    Column {
        width: parent.width
        height: parent.height
        z: 1
        Row {
            width: parent.width
            height: parent.height / 7
            Text {
                width: parent.width / 2
                height: parent.height
                text: "监测数据列表一"
                color: "skyblue"
                font.pixelSize: 24
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }

            Text {
                width: parent.width / 2
                height: parent.height
                text: "监测数据列表二"
                color: "green"
                font.pixelSize: 24
                horizontalAlignment: TextEdit.AlignHCenter
                verticalAlignment: TextEdit.AlignVCenter
            }
        }
        Rectangle {
            id: tableArea
            width: parent.width
            height: parent.height / 7 * 5
            color: "transparent"
            Column {
                id: acArea
                width: parent.width / 2 - 20
                height: parent.height
                anchors.left: parent.left
                anchors.leftMargin: 10
                Rectangle {
                    id: acTitleArea
                    width: parent.width
                    height: parent.height / 10
                    color: "gray"
                    Text {
                        id: acTitle1
                        width: parent.width / 4
                        text: "栏一"
                        color: "yellow"
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    Text {
                        id: acTitle2
                        width: parent.width / 4
                        text: "栏二"
                        color: "yellow"
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                        anchors.left: acTitle1.right
                    }
                    Text {
                        id: acTitle3
                        width: parent.width / 4
                        text: "栏三"
                        color: "yellow"
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                        anchors.left: acTitle2.right
                    }
                    Text {
                        width: parent.width / 4
                        text: "栏四"
                        color: "yellow"
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                        anchors.left: acTitle3.right
                    }
                }
                ListView {
                    id: acView
                    width: parent.width
                    height: parent.height - acTitleArea.height
                    model: acModel
                    orientation: ListView.Vertical // 垂直列表
                    interactive: true; // 元素可拖动
                    clip: true //指定是否裁剪超出ListView边界的项。默认情况下是开启的
                    delegate: Rectangle {
                        width: parent.width
                        height: acTitleArea.height // 每个条目的高度
                        color: model.selected ? "lightblue" : (index % 2 == 0 ? "transparent" : "#333333")
                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            Text {
                                id: acCol1
                                width: parent.width / 4
                                height: parent.height
                                text: "值" + model.display.id
                                color: "blue"
                                horizontalAlignment: TextEdit.AlignHCenter
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                id: acCol2
                                width: parent.width / 4
                                height: parent.height
                                anchors.left: acCol1.right
                                text: "值" + model.display.v
                                color: model.selected ? acCol1.color : "white"
                                horizontalAlignment: TextEdit.AlignHCenter
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                id: acCol3
                                width: parent.width / 4
                                height: parent.height
                                anchors.left: acCol2.right
                                text: "值" + model.display.a
                                color: model.selected ? acCol1.color : "white"
                                horizontalAlignment: TextEdit.AlignHCenter
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                width: parent.width / 4
                                height: parent.height
                                anchors.left: acCol3.right
                                text: "值" + model.display.w
                                color: model.selected ? acCol1.color : "white"
                                horizontalAlignment: TextEdit.AlignHCenter
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    acView.currentIndex = index // 设置当前选中的索引
                                }
                            }
                        }
                    }

                    onCurrentIndexChanged: {
                        model.setSelectedIndex(acView.currentIndex) // 更新模型中的选中项
                    }

                    Component.onCompleted: {
                        model.append(1, 220, 10, 120)
                        for (var i = 2; i < 15; i++) {
                            model.append(i, 220, 10, 40)
                        }
                        model.append({id: 15, v: 220, a: 10, w: 30 })
                    }
                }
            }
            Column {
                id: dcArea
                width: parent.width / 2 - 20
                height: parent.height
                anchors.right: parent.right
                anchors.rightMargin: 10
                Rectangle {
                    id: dcTitleArea
                    width: parent.width
                    height: parent.height / 10
                    color: "gray"
                    Text {
                        id: dcTitle1
                        width: parent.width / 4
                        text: "栏一"
                        color: "yellow"
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                    }
                    Text {
                        id: dcTitle2
                        width: parent.width / 4
                        text: "栏二"
                        color: "yellow"
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                        anchors.left: dcTitle1.right
                    }
                    Text {
                        id: dcTitle3
                        width: parent.width / 4
                        text: "栏三"
                        color: "yellow"
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                        anchors.left: dcTitle2.right
                    }
                    Text {
                        width: parent.width / 4
                        text: "栏四"
                        color: "yellow"
                        horizontalAlignment: TextEdit.AlignHCenter
                        verticalAlignment: TextEdit.AlignVCenter
                        anchors.left: dcTitle3.right
                    }
                }
                ListView {
                    id: dcView
                    width: parent.width
                    height: parent.height - dcTitleArea.height
                    model: dcModel
                    orientation: ListView.Vertical // 垂直列表
                    interactive: true; // 元素可拖动
                    clip: true //指定是否裁剪超出ListView边界的项。默认情况下是开启的
                    delegate: Rectangle {
                        width: parent.width
                        height: dcTitleArea.height // 每个条目的高度
                        color: model.selected ? "lightblue" : (index % 2 == 0 ? "transparent" : "#333333")
                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            Text {
                                id: dcCol1
                                width: parent.width / 4
                                height: parent.height
                                text: "值" + model.display.id
                                color: "green"
                                horizontalAlignment: TextEdit.AlignHCenter
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                id: dcCol2
                                width: parent.width / 4
                                height: parent.height
                                anchors.left: dcCol1.right
                                text: "值" + model.display.v
                                color: model.selected ? dcCol1.color : "white"
                                horizontalAlignment: TextEdit.AlignHCenter
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                id: dcCol3
                                width: parent.width / 4
                                height: parent.height
                                anchors.left: dcCol2.right
                                text: "值" + model.display.a
                                color: model.selected ? dcCol1.color : "white"
                                horizontalAlignment: TextEdit.AlignHCenter
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                width: parent.width / 4
                                height: parent.height
                                anchors.left: dcCol3.right
                                text: "值" + model.display.w
                                color: model.selected ? dcCol1.color : "white"
                                horizontalAlignment: TextEdit.AlignHCenter
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    dcView.currentIndex = index // 设置当前选中的索引
                                }
                            }
                        }
                    }

                    onCurrentIndexChanged: {
                        model.setSelectedIndex(dcView.currentIndex) // 更新模型中的选中项
                    }

                    Component.onCompleted: {
                        model.append(1, 12, 10, 10)
                        model.append(2, 12, 10, 18)
                        model.append(3, 12, 10, 20)
                        model.append({id: 4, v: 12, a: 10, w: 15 })
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
