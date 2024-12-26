import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root_radioBt
    width: 200; height: 40
    property alias text: label.text
    property alias options: radioRep.model
    property int labelLength: 1
    property int radioLength: 1
    property int selectedOption: 0
    signal changed(string label, int value)
    Rectangle {
        id: labelRect
        width: root_radioBt.width / (labelLength + radioLength) * labelLength
        height: root_radioBt.height
        anchors.left: root_radioBt.left
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
        width: root_radioBt.width / (labelLength + radioLength) * radioLength
        height: root_radioBt.height
        anchors.right: root_radioBt.right
        color: "transparent"
        border.width: 0
        Row {
            anchors.centerIn: parent
            spacing: 10

            Repeater {
                id: radioRep
                model: ListModel {
                    ListElement { label: "实时"; value: 1 }
                    ListElement { label: "历史"; value: 2 }
                }

                MyTextButton {
                    text: model.label
                    chcked: selectedOption === model.value
                    onClicked: {
                        for(var i = 0; i < radioRep.model.count; i++) {
                            var elm = radioRep.model.get(i);
                            elm.checked = false;
                        }
                        selectedOption = model.value
                        model.checked = true
                        root_radioBt.changed(model.label, model.value)
                    }
                }
            }
        }
    }
}
