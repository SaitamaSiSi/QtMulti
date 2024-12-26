import QtQuick 2.0
import QtQuick.Controls 2.1

Dialog {
    id: dialog

    property string dlgTitle: "Title"
    property string dlgContent: "Content"
    property string bgColor: "#606060"
    property string tImgSource: "qrc:/resources/images/提示.png"
    property string cImgSource: "qrc:/resources/images/关闭.png"
    property int posX: (parent.width - width) / 2
    property int posY: (parent.height - height) / 2
    property int dlgW: 500
    property int dlgH: 300

    x: posX; y: posY
    width: dlgW; height: dlgH
    //implicitWidth: 500;
    //implicitHeight: 300;
    modal: false; /* 非模态对话框 */
    standardButtons: Dialog.Ok | Dialog.Cancel

    header: Rectangle {
        id: headerArea
        width: parent.width
        height: parent.height / 5
        color: bgColor
        Image {
            id: titleImg
            source: tImgSource
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }
        Image {
            source: cImgSource
            anchors.horizontalCenter: parent.right
            anchors.verticalCenter: parent.top
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    dialog.reject()
                }
            }
        }
        Text {
            id: titleLabel
            text: dlgTitle
            color: "white"
            font.pixelSize: 16
            anchors.left: titleImg.right
            anchors.leftMargin: 10
            height: parent.height
            verticalAlignment: Text.AlignVCenter
        }
        /* 水平分隔线 */
        Rectangle {
            id: horizontalLine
            width: parent.width - 20
            anchors.left: parent.left
            anchors.leftMargin: 10
            height: 1
            anchors.bottomMargin: 5
            anchors.bottom: parent.bottom
            color: "black"
        }
    }

    contentItem: Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: headerArea.bottom
        anchors.bottom: footArea.top
        color: bgColor
        Text{
            text: dlgContent
            font.pixelSize: 20
            color: "white"
            wrapMode: Text.WordWrap
            anchors.centerIn: parent
        }
    }

    footer: Rectangle {
        id: footArea
        width: parent.width
        height: parent.height / 5
        color: bgColor
        Row {
            width: parent.width / 3 * 2 - 10
            anchors.left: parent.left
            anchors.leftMargin: parent.width / 6
            height: parent.height
            spacing: 10
            MyTextButton {
                id: cancelBtn
                text: qsTr("取消")
                bgColor: "gray"
                btnWidth: parent.width / 2
                btnHeight: parent.height / 5 * 3
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    dialog.reject()
                }
            }
            MyTextButton {
                text: qsTr("确定")
                chcked: true
                btnWidth: cancelBtn.btnWidth
                btnHeight: cancelBtn.btnHeight
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    dialog.accept()
                }
            }
        }
    }
}
