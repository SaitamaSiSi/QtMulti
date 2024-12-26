import QtQuick 2.0

import MyDataEntryModel 1.0

ListView {
    id: view
    anchors.fill: parent
    model: DataEntryModel { }
    delegate: Rectangle {
        width: view.width
        height: 50 // 每个条目的高度

        Rectangle {
            anchors.fill: parent
            color: model.selected ? "lightblue" : "white" // 改变背景颜色
            border.color: "gray"

            Text {
                anchors.centerIn: parent
                text: model.display.id // 访问 display 角色
                font.pointSize: 16
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                view.currentIndex = index // 设置当前选中的索引
            }
        }
    }

    highlight: Rectangle {
        width: view.width
        height: 50 // 高度与条目一致

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.5 // 透明度设置为 0.5
        }
    }

    onCurrentIndexChanged: {
        model.setSelectedIndex(view.currentIndex) // 更新模型中的选中项
    }
}
