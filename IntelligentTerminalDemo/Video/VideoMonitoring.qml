import QtQuick 2.0
import QtAV 1.6
import QtQuick.Controls 2.1

import "../Components"

Item {
    id: root_video
    property int currentVideo: 0
    ListModel {
        id: rtspList
    }
    ListModel {
        id: videoIconList
        ListElement {
            iconId: 1
            chked: false
            text: "视频一"
            icon_img: "qrc:/resources/images/05_相机.png"
            icon_img2: "qrc:/resources/images/05_相机选中.png"
        }
        ListElement {
            iconId: 2
            chked: false
            text: "视频二"
            icon_img: "qrc:/resources/images/05_相机.png"
            icon_img2: "qrc:/resources/images/05_相机选中.png"
        }
        ListElement {
            iconId: 3
            chked: false
            text: "视频三"
            icon_img: "qrc:/resources/images/05_相机.png"
            icon_img2: "qrc:/resources/images/05_相机选中.png"
        }
        ListElement {
            iconId: 4
            chked: false
            text: "视频四"
            icon_img: "qrc:/resources/images/05_相机.png"
            icon_img2: "qrc:/resources/images/05_相机选中.png"
        }
    }
    MyPlaylist {
        id: playlist
        mediaPlayer: player
        items: rtspList
    }
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
                    width: parent.width / 3 * 2
                    height: parent.height
                    color: "transparent"
                    Video {
                        id: player
                        anchors.fill: parent
                    }
                }
                Rectangle {
                    width: parent.width / 3
                    height: parent.height
                    color: "transparent"
                    border.color: "gray"
                    Text {
                        text: "视频列表"
                        color: "white"
                        font.pixelSize: 18
                        anchors.verticalCenter: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                    }
                    ListView {
                        id: listVIew
                        width: parent.width
                        height: parent.height / 3 * 2
                        anchors.top: parent.top
                        anchors.topMargin: parent.height / 6
                        model: videoIconList
                        delegate: Row {
                            width: listVIew.width
                            height: listVIew.height / 4
                            Rectangle {
                                id: imgArea
                                width: parent.width / 3
                                height: parent.height
                                color: "transparent"
                                Image {
                                    source: model.chked ? model.icon_img2 : model.icon_img
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                }
                            }
                            Text {
                                text: model.text
                                color: model.chked ? "skyblue" : "gray"
                                width: parent.width - imgArea.width
                                height: parent.height
                                font.pixelSize: 24
                                horizontalAlignment: TextEdit.AlignLeft
                                verticalAlignment: TextEdit.AlignVCenter
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        console.debug(model.text)
                                        var dlg = dialogCmpt.createObject(parent);
                                        dlg.dlgTitle = "提示"
                                        dlg.dlgContent = "操作有风险，是否继续？"
                                        dlg.posX = -480
                                        dlg.posY = -50 - index * listVIew.height / 4
                                        dlg.dlgW = 500
                                        dlg.dlgH = 300
                                        dlg.accepted.connect(function() {
                                            console.log("自定义处理 accepted 信号");
                                            reset()
                                            currentVideo = model.iconId
                                            model.chked = true
                                            if (currentVideo === 3) {
                                                playlist.setIndex(0)
                                                playlist.start()
                                            } else if (playlist.index !== -1) {
                                                playlist.stop()
                                            }
                                        });
                                        dlg.rejected.connect(function() {
                                            console.log("自定义处理 rejected 信号");
                                        });
                                        dlg.open();
                                    }
                                }
                            }
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
    Component.onCompleted: {
        rtspList.append({ "source": "rtsp://192.168.100.198:554/live" })

        for (var i = 0; i < videoIconList.count; i++) {
            var item = videoIconList.get(i)
            if (item.iconId === currentVideo) {
                item.chked = true
            }
        }
    }

    /* 提示对话框 */
    Component {
        id: dialogCmpt
        MsgBox {
            onClosed: destroy()
        }
    }

    function reset() {
        var elm = null
        var i = 0
        for(i = 0; i < videoIconList.count; i++) {
            elm = videoIconList.get(i)
            elm.chked = false
        }
    }
}
