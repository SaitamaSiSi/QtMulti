import QtQuick 2.0

Item {
    id: root_button
    property alias text: label.text
    property alias imgSource: img.source
    property int borderWidth: 0
    property int borderRadius: 0
    property string borderColor: "black"
    property string hoverColor: "skyblue"
    signal clicked
    width: 120; height: 40
    Rectangle {
        id: rectangle
        anchors.fill: parent
        border.width: borderWidth
        radius: borderRadius
        color: "transparent"
        border.color: borderColor
        Rectangle {
            id: imgRect
            width: root_button.width / 4
            height: root_button.height
            color: "transparent"
            border.width: 0
            Image {
                id: img
                anchors.centerIn: parent
                fillMode: Image.PreserveAspectFit
                source: ""
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle {
            id: labelRect
            width: root_button.width / 4 * 3
            height: root_button.height
            anchors.left: imgRect.right
            color: "transparent"
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
    }
    // 鼠标区域，用于检测鼠标事件
    MouseArea {
        id: mouseArea
        anchors.fill: parent // 填充整个Rectangle

        onClicked: {
            root_button.clicked()
        }

        // 鼠标悬停事件处理
        onEntered: {
            rectangle.border.color = hoverColor; // 鼠标悬停时修改边框颜色为蓝色
        }

        // 鼠标离开事件处理
        onExited: {
            rectangle.border.color = borderColor; // 鼠标离开时恢复默认边框颜色
        }
    }
}
