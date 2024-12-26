import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

Rectangle{
    border.color: "#d7cfcf"
    property bool showSend: false
    property bool repeatSend: false
    Column{
        id:leftColumn
        anchors.fill: parent
        anchors.margins: 5
        spacing: 0

        //串口设置
        Label{
            text: qsTr("串口设置")
            color: "#000000"
        }

        GridLayout {
            id: grid
            columns: 2
            rowSpacing: 0
            Label {
                text: qsTr("串  口")
            }
            ComboBox {
                id:serialName
                model: []//serial.querySerialInfo()// [ "Banana", "Apple", "Coconut" ]
                onActivated: {
                    //serial.setPortName(currentText)
                }
            }

            Label {
                text: qsTr("波特率")

            }
            ComboBox {
                id:baud
                model: [ "9600", "19200", "38400", "57600","115200"]
                onActivated: {
                    serial.setBaud(currentText)
                }
            }
        }
        Label{
            text: qsTr("接收设置")
            color: "#000000"
        }
        RowLayout{
            RadioButton { text: qsTr("ASCII"); checked: true }
            RadioButton {
                text: qsTr("Hex")
                onCheckedChanged: {
                    if(checked)
                      serial.setReceiveHex(true)
                    else
                        serial.setReceiveHex(false)
                }
            }
        }
        CheckBox {
            checked: false
            text: qsTr("显示发送")
            onCheckedChanged: {
                showSend=checked
            }
        }
        Label{
            text: qsTr("发送设置")
            color: "#000000"
        }
        RowLayout{
            RadioButton { text: qsTr("ASCII"); checked: true }
            RadioButton {
                text: qsTr("Hex")
                onCheckedChanged: {
                    if(checked)
                        serial.setSedHex(true)
                    else
                        serial.setSedHex(false)
                }
            }
        }
        CheckBox {
            id:repeatSend
            checked: false
            text: qsTr("重复发送(ms)")
            onCheckedChanged: {
                showSend=checked
            }
        }
        SpinBox {
            id: spinbox
            width: 150
            from: 0
            to:100000
            value: 1000
            editable: true
            visible: repeatSend.checked
        }
    }
    Component.onCompleted: {
        //serial.setPortName(serialName.currentText)
        //serial.setBaud(baud.currentText)
    }
}
