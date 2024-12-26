import QtQuick 2.0

import "../Components"

Item {
    id: root_env
    Column {
        width: parent.width
        height: parent.height
        z: 1
        Rectangle {
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
                        text: "风扇"
                        color: "white"
                        font.pixelSize: 18
                        anchors.verticalCenter: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                    }
                    MyGif {
                        id: fan_gif
                        imgSource: "qrc:/resources/images/风扇-关.png"
                        smooth: true
                        width: parent.width / 4 * 3
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width / 8
                        height: (parent.height  - fan_text_area.height) / 4 * 3
                        anchors.top: parent.top
                        anchors.topMargin: (parent.height  - fan_text_area.height) / 8
                    }
                    Column {
                        id: fan_text_area
                        width: parent.width
                        height: parent.height / 5
                        anchors.top: fan_gif.bottom
                        anchors.topMargin: 10
                        Row {
                            width: parent.width
                            height: parent.height / 2
                            Text {
                                text: "风扇状态："
                                color: "gray"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 18
                                horizontalAlignment: TextEdit.AlignRight
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                id: fan_status
                                text: "关闭"
                                color: "skyblue"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 24
                                horizontalAlignment: TextEdit.AlignLeft
                                verticalAlignment: TextEdit.AlignVCenter
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        if (fan_status.text == "关闭") {
                                            fan_status.text = "开启"
                                            fan_status.color = "green"
                                            fan_gif.setImgSource("qrc:/resources/images/风扇开.gif");
                                        } else if (fan_status.text == "开启") {
                                            fan_status.text = "关闭"
                                            fan_status.color = "skyblue"
                                            fan_gif.setImgSource("qrc:/resources/images/风扇-关.png");
                                        }
                                    }
                                }
                            }
                        }
                        Row {
                            width: parent.width
                            height: parent.height / 2
                            Text {
                                text: "运行时间："
                                color: "gray"
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize: 18
                                horizontalAlignment: TextEdit.AlignRight
                                verticalAlignment: TextEdit.AlignVCenter
                            }
                            Text {
                                text: "107:30:21"
                                color: "skyblue"
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
                    width: parent.width / 3 * 2
                    height: parent.height
                    color: "transparent"
                    border.color: "gray"
                    Text {
                        text: "机箱温度变化"
                        color: "white"
                        font.pixelSize: 18
                        anchors.verticalCenter: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                    }
                    ListModel {
                        id: chartData
                    }

                    MyChartView {
                        id: myChartView
                        anchors.fill: parent
                        points: chartData
                        showType: "sp"
                        showPoint: true
                        xTitle: ""
                        yTitle: ""
                    }
                    Component.onCompleted: {
                        var dataArray = [
                                    { x: 0, y: 8 },
                                    { x: 1, y: 10 },
                                    { x: 2, y: 18 },
                                    { x: 3, y: 23 },
                                    { x: 4, y: 23 },
                                    { x: 5, y: 26 },
                                    { x: 6, y: 31 },
                                    { x: 7, y: 33 },
                                    { x: 8, y: 33 },
                                    { x: 9, y: 33 },
                                    { x: 10, y: 36 },
                                    { x: 11, y: 39 },
                                    { x: 12, y: 44 }
                                ];
                        for (var i = 0; i < dataArray.length; ++i) {
                            chartData.append(dataArray[i]);
                        }
                    }
                }

            }
        }
        Rectangle {
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
