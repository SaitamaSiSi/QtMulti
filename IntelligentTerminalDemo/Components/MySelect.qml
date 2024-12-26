import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root_select
    property alias text: label.text
    property alias options: combox.model
    property int labelLength: 1
    property int selectLength: 1
    signal selectChanged(string text, int value)
    width: 160; height: 40
    Rectangle {
        id: labelRect
        width: root_select.width / (labelLength + selectLength) * labelLength
        height: root_select.height
        anchors.left: root_select.left
        color: "transparent"
        border.width: 0
        Text {
            id: label
            anchors.centerIn: parent
            color: "lightblue"
            text: ""
            font.pointSize: 16
            horizontalAlignment: TextEdit.AlignRight
            verticalAlignment: TextEdit.AlignVCenter
        }
    }
    Rectangle {
        id: selectRect
        width: root_select.width / (labelLength + selectLength) * selectLength
        height: root_select.height
        anchors.right: root_select.right
        color: "transparent"
        border.width: 0
        BasicCombox {
            id: combox
            model: []
            textRole: "label"
            textColor: "white"
            //textLeft: "No.["
            //textRight: "]"
            //radius: 3
            anchors.fill: parent
            itemNormalColor: "lightsteelblue"
            itemHighlightColor: "skyblue"
            //indicatorSource: "qrc:/updown.png"
            background: Rectangle{
                // color: basic_combobox2_2.hovered ? Qt.lighter("blue") : "lightsteelblue"
                color: combox.hovered ? "#6495ED" : "skyblue"
                border.width: 1
                border.color: "black"
            }
            onCurrentIndexChanged: {
                if (combox.model !== 1) {
                    root_select.selectChanged(options.get(currentIndex).text, options.get(currentIndex).value)
                }
            }
        }
    }
}
