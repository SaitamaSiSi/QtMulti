import QtQuick 2.0
import QtQuick.LocalStorage 2.0

import "../Components"

Item {
    id: root_analysis

    Item {
        id: sqlite
        anchors.fill: parent
        visible: false
        // reference to the database object
        property var db;
        Rectangle {
            id: crazy
            objectName: 'crazy'
            width: 100
            height: 100
            x: 50
            y: 50
            color: "#53d769"
            border.color: Qt.lighter(color, 1.1)
            Text {
                anchors.centerIn: parent
                text: Math.round(parent.x) + '/' + Math.round(parent.y)
            }
            MouseArea {
                anchors.fill: parent
                drag.target: parent
            }
        }

        function initDatabase() {
            // initialize the database object
            print('initDatabase()')
            db = LocalStorage.openDatabaseSync("CrazyBox", "1.0", "A box who remembers its position", 100000);
            db.transaction( function(tx) {
                print('... create table')
                tx.executeSql('CREATE TABLE IF NOT EXISTS data(name TEXT, value TEXT)');
            });
        }
        function storeData() {
            // stores data to DB
            print('storeData()')
            if(!db) { return; }
            db.transaction( function(tx) {
                print('... check if a crazy object exists')
                var result = tx.executeSql('SELECT * from data where name = "crazy"');
                // prepare object to be stored as JSON
                var obj = { x: crazy.x, y: crazy.y };
                if(result.rows.length === 1) {// use update
                    print('... crazy exists, update it')
                    result = tx.executeSql('UPDATE data set value=? where name="crazy"', [JSON.stringify(obj)]);
                } else { // use insert
                    print('... crazy does not exists, create it')
                    result = tx.executeSql('INSERT INTO data VALUES (?,?)', ['crazy', JSON.stringify(obj)]);
                }
            });
        }
        function readData() {
            // reads and applies data from DB
            print('readData()')
            if(!db) { return; }
            db.transaction( function(tx) {
                print('... read crazy object')
                var result = tx.executeSql('select * from data where name="crazy"');
                if(result.rows.length === 1) {
                    print('... update crazy geometry')
                    // get the value column
                    var value = result.rows[0].value;
                    // convert to JS object
                    var obj = JSON.parse(value)
                    // apply to object
                    crazy.x = obj.x;
                    crazy.y = obj.y;
                }
            });
        }
        Component.onCompleted: {
            initDatabase();
            readData();
        }
        Component.onDestruction: {
            storeData();
        }
    }

    Item {
        id: serialPort
        anchors.fill: parent
        visible: true

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
            MyInput {
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
                    var showText = serial.testValue + ": avaliable ports: "
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
                        var res = false;
                        var showText = ""
                        if (editSend.text === "test") {
                            res = serial.send_ic_test()
                            showText = "send " + editSend.text + ": " + (res ? "success" : "failed")
                            console.log(showText)
                            editReceive.append(showText)
                        } else {
                            res = serial.sendMessage(editSend.text)
                            showText = "send " + editSend.text + ": " + (res ? "success" : "failed")
                            console.log(showText)
                            editReceive.append(showText)
                        }
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
                    if (editSend.text === "test") {
                        serial.icDataVerifyTest(data);
                    }
                    editReceive.append("receive:" + data)
                }
            }
        }
    }
}
