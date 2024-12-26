import QtQuick 2.0

Item {
    id: root_system
    Image {
        id: bgicon_img
        anchors.fill: parent
        source: "qrc:/resources/images/主页井字分割线.png"
        z: 0
    }
    Column {
        width: parent.width
        height: parent.height
        z: 1
        Row {
            id: topRow
            width: parent.width
            height: parent.height / 2 - 5
            Rectangle {
                width: parent.width / 3
                height: parent.height
                color: "transparent"
                Image {
                    id: img1
                    source: "qrc:/resources/images/01_主电-选中.png"
                    x: 50
                    y: 90
                }
                Text {
                    id: text1
                    text: "信息1"
                    color: "white"
                    anchors.left: img1.right
                    anchors.leftMargin: 10
                    anchors.top: img1.top
                }
                Text {
                    text: "值1"
                    color: "skyblue"
                    font.pixelSize: 32
                    anchors.left: text1.left
                    anchors.top: text1.bottom
                }
            }
            Rectangle {
                width: parent.width / 3
                height: parent.height
                color: "transparent"
                Image {
                    id: img2
                    source: "qrc:/resources/images/01_电压-选中.png"
                    x: 50
                    y: 90
                }
                Text {
                    id: text2
                    text: "信息2"
                    color: "white"
                    anchors.left: img2.right
                    anchors.leftMargin: 10
                    anchors.top: img2.top
                }
                Text {
                    text: "值2"
                    color: "skyblue"
                    font.pixelSize: 32
                    anchors.left: text2.left
                    anchors.top: text2.bottom
                }
            }
            Rectangle {
                width: parent.width / 3
                height: parent.height
                color: "transparent"
                Image {
                    id: img3
                    source: "qrc:/resources/images/01_电流-选中.png"
                    x: 50
                    y: 90
                }
                Text {
                    id: text3
                    text: "信息3"
                    color: "white"
                    anchors.left: img3.right
                    anchors.leftMargin: 10
                    anchors.top: img3.top
                }
                Text {
                    text: "值3"
                    color: "skyblue"
                    font.pixelSize: 32
                    anchors.left: text3.left
                    anchors.top: text3.bottom
                }
            }
        }

        Row {
            width: parent.width
            height: parent.height / 2
            Rectangle {
                width: parent.width / 3
                height: parent.height
                color: "transparent"
                Image {
                    id: img4
                    source: "qrc:/resources/images/01_温度-选中.png"
                    x: 40
                    y: 40
                }
                Text {
                    id: text4
                    text: "信息4"
                    color: "white"
                    anchors.left: img4.right
                    anchors.leftMargin: 10
                    anchors.top: img4.top
                }
                Text {
                    text: "值4"
                    color: "red"
                    font.pixelSize: 32
                    anchors.left: text4.left
                    anchors.top: text4.bottom
                }
            }
            Rectangle {
                width: parent.width / 3
                height: parent.height
                color: "transparent"
                Image {
                    id: img5
                    source: "qrc:/resources/images/01_湿度-选中.png"
                    x: 40
                    y: 40
                }
                Text {
                    id: text5
                    text: "信息5"
                    color: "white"
                    anchors.left: img5.right
                    anchors.leftMargin: 10
                    anchors.top: img5.top
                }
                Text {
                    text: "值5"
                    color: "green"
                    font.pixelSize: 32
                    anchors.left: text5.left
                    anchors.top: text5.bottom
                }
            }
            Rectangle {
                width: parent.width / 3
                height: parent.height
                color: "transparent"
                Image {
                    id: img6
                    source: "qrc:/resources/images/01_柜门-选中.png"
                    x: 40
                    y: 40
                }
                Text {
                    id: text6
                    text: "信息6"
                    color: "white"
                    anchors.left: img6.right
                    anchors.leftMargin: 10
                    anchors.top: img6.top
                }
                Text {
                    text: "值6"
                    color: "yellow"
                    font.pixelSize: 32
                    anchors.left: text6.left
                    anchors.top: text6.bottom
                }
            }
        }
    }
}
