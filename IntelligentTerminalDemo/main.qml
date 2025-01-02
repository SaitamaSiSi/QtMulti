import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 2.0

Window {
    id: rootMain
    visible: true
    width: 1280
    height: 480
    minimumWidth: 1280
    minimumHeight: 480
    maximumWidth: 1280
    maximumHeight: 480
    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2
    title: "路灯智能机箱控制系统"
    property int pageIndex: 0

    Image {
        id: bgicon_img
        //source: "qrc:/resources/images/背景+菜单栏.jpg"
        source: "qrc:/resources/images/bg1.jpg"
        z: 0
    }
    Image {
        id:headicon_img
        //source: "qrc:/resources/images/head.png"
        source: "qrc:/resources/images/顶部H60不含标题.png"
        z: 1
    }
    Image {
        id:returnicon_img
        source: "qrc:/resources/images/返回.png"
        z: 1
        x: 25
        y: (headicon_img.height - seticon_img.width) / 2
        MouseArea {
            anchors.fill: parent
            onClicked: {
                rootMain.pageIndex = 0
            }
        }
    }
    Image {
        id:seticon_img
        source: "qrc:/resources/images/设置.png"
        z: 1
        x: rootMain.width - seticon_img.width - 30
        y: (headicon_img.height - seticon_img.width) / 2
        MouseArea {
            anchors.fill: parent
            onClicked: {
                rootMain.pageIndex = 1
            }
        }
    }
    Rectangle {
        width: rootMain.width
        height: headicon_img.height
        color: "transparent"
        border.width: 0
        z: 1
        Text {
            id: topTitleText
            text: qsTr("路灯智能机箱控制系统")
            color: "lightskyblue"
            font.pixelSize: 24
            font.bold: true
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    Rectangle {
        width: 260
        height: headicon_img.height
        anchors.right: seticon_img.left
        color: "transparent"
        border.width: 0
        z: 1
        Text {
            id: topTimeText
            text: formatDateTime()
            color: "white"
            font.pixelSize: 24
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    SwipeView {
        id: swipview
        interactive: false
        orientation: Qt.Horizontal
        currentIndex: rootMain.pageIndex
        anchors.fill: parent
        anchors.topMargin: headicon_img.height

        MyFrame {
            id: myFrame
            rootWidth: rootMain.width
            rootHeight: rootMain.height - headicon_img.height
            onMenuChanged: function(name){
                console.debug("clicked => ", name);
            }
        }

        MySettingFrame {
            id: mySettingFrame
            rootWidth: rootMain.width
            rootHeight: rootMain.height - headicon_img.height
            onMenuChanged: function(name){
                console.debug("in rootMain clicked => ", name);
            }
        }

    }

    Timer {
        interval: 1000 // 每秒更新一次时间
        running: true
        repeat: true
        onTriggered: {
            topTimeText.text = formatDateTime()
        }
    }

    function formatDateTime() {
        const currentDateTime = new Date()
        const year = currentDateTime.getFullYear()
        const month = ('0' + (currentDateTime.getMonth() + 1)).slice(-2) // 月份加1并格式化
        const day = ('0' + currentDateTime.getDate()).slice(-2)
        const hour = ('0' + currentDateTime.getHours()).slice(-2)
        const minute = ('0' + currentDateTime.getMinutes()).slice(-2)
        const second = ('0' + currentDateTime.getSeconds()).slice(-2)
        //return `${year}${month}${day}-${hour}${minute}${second}`
        return year + "." + month + "." + day + " " + hour + ":" + minute + ":" + second
    }

}
