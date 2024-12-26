import QtQuick 2.0
import QtMultimedia 5.0

import "../components"

Item {
    width: 400
    height: 200

    VideoOutput {
        anchors.fill: parent
        source: camera
    }

    Camera {
        id: camera
    }

    ListModel {
        id: imagePaths
    }
    ListView {
        id: listView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.bottom
        anchors.bottomMargin: 10
        height: 100
        orientation: ListView.Horizontal
        spacing: 10
        model: imagePaths
        delegate: Image { source: path; fillMode: Image.PreserveAspectFit; height: 100; }
        Rectangle {
            anchors.fill: parent
            anchors.topMargin: -10
            color: "black"
            opacity: 0.5
        }
    }

    MyButton {
        id: shotButton
        width: 200
        height: 75
        anchors.left: parent.right
        text: "Take Photo"
        onClicked: {
            const screenPath = "/home/zyh/Projects/Qt/ZyhQmlDemo/screen-" + formatDateTime()
            camera.imageCapture.captureToLocation(screenPath)
        }
    }

    Connections {
        target: camera.imageCapture
        onImageSaved: {
            imagePaths.append({"path": path})
            listView.positionViewAtEnd();
        }
    }

    property int _imageIndex: -1
    function startPlayback()
    {
        root.state = "playing";
        setImageIndex(0);
        playTimer.start();
    }
    function setImageIndex(i)
    {
        _imageIndex = i;
        if (_imageIndex >= 0 && _imageIndex < imagePaths.count)
            image.source = imagePaths.get(_imageIndex).path;
        else
            image.source = "";
    }
    Timer {
        id: playTimer
        interval: 200
        repeat: false
        onTriggered: {
            if (_imageIndex + 1 < imagePaths.count)
            {
                setImageIndex(_imageIndex + 1);
                playTimer.start();
            }
            else
            {
                setImageIndex(-1);
                root.state = "";
            }
        }
    }

    // 获取当前时间并格式化为"YYYYMMDD-HHMMSS"的字符串
    function formatDateTime() {
        const currentDateTime = new Date()
        const year = currentDateTime.getFullYear()
        const month = ('0' + (currentDateTime.getMonth() + 1)).slice(-2) // 月份加1并格式化
        const day = ('0' + currentDateTime.getDate()).slice(-2)
        const hour = ('0' + currentDateTime.getHours()).slice(-2)
        const minute = ('0' + currentDateTime.getMinutes()).slice(-2)
        const second = ('0' + currentDateTime.getSeconds()).slice(-2)
        //return `${year}${month}${day}-${hour}${minute}${second}`
        return year+month+day+"-"+hour+minute+second
    }
}
