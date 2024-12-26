import QtQuick 2.6
import QtQuick.Window 2.2

import "./example"
import "./media"
import "./cppExamples"
import "./Sqlite"
import "./components"

Window {
    id: root
    visible: true
    width: 800
    height: 400
    title: qsTr("Hello World")
    color: 'white'

    /* 快速入门
    MyExample {
        id: myExample
    }
    */

    /* 多媒体
    MyAllMediaSample {
        id: myAllMediaSample
    }
    */


    /* cpp 测试
    MyCppExample {

    }
     */


    /* sqlite
    MySqlite {

    }
     */

    /*
    MySerialPort{
    }
     */

    /*
    Rectangle {
        id: receiveData
        anchors.top: parent.top
        anchors.left: parent.left
        border.color: "#d7cfcf"
        height: parent.height / 1.5
        width: parent.width

        Flickable {
            id: flickReceive
            anchors.fill: parent
            clip: true
            anchors.margins: 5

            function ensureVisible(r)
            {
                if (contentX >= r.x)
                    contentX = r.x;
                else if (contentX+width <= r.x+r.width)
                    contentX = r.x+r.width-width;
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY+height <= r.y+r.height)
                    contentY = r.y+r.height-height;
            }

            TextEdit {
                id: editReceive
                width: flickReceive.width
                focus: false
                wrapMode: TextEdit.Wrap
                onCursorRectangleChanged: flickReceive.ensureVisible(cursorRectangle)
            }

        }
    }

    Rectangle {
        id: sendData
        width: parent.width
        height: parent.height / 4.5
        border.color: "#d7cfcf"
        anchors.top: receiveData.bottom
        Flickable {
            id: flickSend
            anchors.fill: parent
            clip: true
            anchors.margins: 5

            function ensureVisible(r)
            {
                if (contentX >= r.x)
                    contentX = r.x;
                else if (contentX+width <= r.x+r.width)
                    contentX = r.x+r.width-width;
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY+height <= r.y+r.height)
                    contentY = r.y+r.height-height;
            }

            TextEdit {
                id: editSend
                width: flickSend.width
                focus: true
                wrapMode: TextEdit.Wrap
                onCursorRectangleChanged: flickSend.ensureVisible(cursorRectangle)
            }
        }
    }

    Row {
        id: buttons
        spacing: 10
        width: parent.width
        anchors.top: sendData.bottom
        anchors.topMargin: 10
        x: 10
        TLineEditV2 {
            id: input
            width: open.width; height: open.height
            focus: true
            text: "/dev/ttyv0"
        }

        MyButton {
            id: getPortNames
            text: "get portNames"
            onClicked: {
                var names = serial.getAvaliablePorts();
                var showText = "avaliable ports: "
                for (var i = 0; i < names.length - 1; i++) {
                    showText = showText + "/dev/" + names[i] + ","
                }
                if (names.length > 0) {
                    showText = showText + "/dev/" + names[i]
                    editReceive.append(showText)
                } else {
                    editReceive.append(showText + "empty")
                }
            }
        }

        MyButton {
            id: open
            text: "open"
            onClicked: {
                var names = serial.getAvaliablePorts();
                names = ["/dev/ttyv0"]
                if (input.text.length > 0) {
                    var res = serial.openPort(input.text)
                    var showText = "open " + input.text + ": " + (res ? "success" : "failed")
                    console.log(showText)
                    editReceive.append(showText)
                }
            }
        }

        MyButton {
            id: send
            text: "send"
            onClicked: {
                if (editSend.text.length > 0) {
                    var res = serial.sendMessage(editSend.text)
                    var showText = "send " + editSend.text + ": " + (res ? "success" : "failed")
                    console.log(showText)
                    editReceive.append(showText)
                }
            }
        }

        MyButton {
            id: read
            text: "read"
            onClicked: {
                var res = serial.readMessage();
                if (res.length > 0) {
                    var showText = "recv: " + res
                    console.log(showText)
                    editReceive.append(showText)
                }
            }
        }

        MyButton {
            id: close
            text: "close"
            onClicked: {
                serial.closePort()
                editReceive.append("close")
            }
        }

        Connections{
            target: serial
            onReadMsgSignal: function(data){
                console.log("receive:" + data)
                editReceive.append("receive:" + data)
            }
        }
    }
    */

    Flipable {
         id: flipable_test
         x: 0//22
         y: 0//25
         width: 200
         height: 64

         property bool flipped: false

         front: Item { anchors.fill: parent }
         back: Item {
            anchors.fill: parent
            Text {
                x:22
                y:20
                text: qsTr("Test文本内容")
//                font.family: Constants.fontOPPOSans
//                font.pixelSize: 16
                color: "#8696a3"
                font.pixelSize: 22
                lineHeight: 1
                font.wordSpacing: 4.9
                font.weight: Font.Medium
                font.family: Constants.fontOPPOSans
            }
         }

         transform: Rotation {
             id: rotation
             origin.x: flipable_test.width/2
             origin.y: flipable_test.height/2
             axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
             angle: 0    // the default angle
         }

         states: State {
             name: "back"
             PropertyChanges { target: rotation; angle: 180 }
             when: flipable_test.flipped
         }

         transitions: Transition {
             NumberAnimation { target: rotation; property: "angle"; duration: 300 }
         }

         MouseArea {
             anchors.fill: parent
             onClicked: flipable_test.flipped = !flipable_test.flipped
         }
     }

}
