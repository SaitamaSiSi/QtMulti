import QtQuick 2.0

import EquipmentDataModel 1.0

import "../Components"

Item {
    EquipmentDataModel {
        id: equipModel
    }

    Rectangle {
        id: equipTitle
        width: parent.width / 5 * 4
        height: 30
        z: 1
        color: "#6495ED"

        Text {
            id: equipIdTitle
            width: parent.width / 4
            text: "栏一"
            font.pointSize: 16
            color: "white"
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        Text {
            id: equipPhaseTitle
            width: parent.width / 4
            anchors.left: equipIdTitle.right
            text: "栏二"
            font.pointSize: 16
            color: "white"
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        Text {
            id: equipTypeTitle
            width: parent.width / 4
            anchors.left: equipPhaseTitle.right
            text: "栏三"
            font.pointSize: 16
            color: "white"
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
        Text {
            id: equipIdentificationTitle
            width: parent.width / 4
            anchors.left: equipTypeTitle.right
            text: "栏四"
            font.pointSize: 16
            color: "white"
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
        }
    }
    Rectangle {
        anchors.fill: view
        color: "transparent"
        border.color: "#6495ED"
    }
    ListView {
        id: view
        width: equipTitle.width
        height: parent.height - equipTitle.height
        anchors.top: equipTitle.bottom
        model: equipModel
        orientation: ListView.Vertical // 垂直列表
        interactive: true; // 元素可拖动
        clip: true //指定是否裁剪超出ListView边界的项。默认情况下是开启的
        delegate: Rectangle {
            width: view.width
            height: equipTitle.height // 每个条目的高度
            color: model.selected ? "lightblue" : "transparent"
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "#6495ED"
                Text {
                    id: equipId
                    width: parent.width / 4
                    text: model.display.equipId
                    font.pointSize: 16
                    color: model.selected ? "green" : "white"
                    horizontalAlignment: TextEdit.AlignHCenter
                    verticalAlignment: TextEdit.AlignVCenter
                }
                Text {
                    id: equipPhase
                    width: parent.width / 4
                    anchors.left: equipId.right
                    text: model.display.phase
                    font.pointSize: 16
                    color: model.selected ? "green" : "white"
                    horizontalAlignment: TextEdit.AlignHCenter
                    verticalAlignment: TextEdit.AlignVCenter
                }
                Text {
                    id: equipType
                    width: parent.width / 4
                    anchors.left: equipPhase.right
                    text: model.display.type
                    font.pointSize: 16
                    color: model.selected ? "green" : "white"
                    horizontalAlignment: TextEdit.AlignHCenter
                    verticalAlignment: TextEdit.AlignVCenter
                }
                Text {
                    id: equipIdentification
                    width: parent.width / 4
                    anchors.left: equipType.right
                    text: model.display.identification
                    font.pointSize: 16
                    color: model.selected ? "green" : "white"
                    horizontalAlignment: TextEdit.AlignHCenter
                    verticalAlignment: TextEdit.AlignVCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        view.currentIndex = index // 设置当前选中的索引
                    }
                }
            }
        }

        onCurrentIndexChanged: {
            model.setSelectedIndex(view.currentIndex) // 更新模型中的选中项
        }

        Component.onCompleted: {
            model.append("0x000000000001", "1", "车行灯", "三色灯")
            for (var i = 0; i < 15; i++) {
                model.append("0x000000000002", "2", "人行灯", "双色灯")
            }
            model.append({id: "0x000000000001", phase: "1", type: "车行灯", identification:"三色灯"})
        }
    }

    Rectangle {
        width: parent.width / 5
        height: parent.height
        color: "transparent"
        anchors.left: equipTitle.right
        anchors.top: equipTitle.top
        Column {
            anchors.centerIn: parent
            spacing: 10

            MyButton {
                text: "功能键一"
                imgSource: "qrc:/resources/images/参数配置.png"
                onClicked: {
                    console.debug("功能键一")
                }
            }
            MyButton {
                text: "功能键二"
                imgSource: "qrc:/resources/images/同步设备.png"
                onClicked: {
                    console.debug("功能键二")
                }
            }
        }
    }
}
