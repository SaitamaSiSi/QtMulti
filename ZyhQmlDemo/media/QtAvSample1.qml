import QtQuick 2.0
import QtAV 1.6

Item {
    width: 800
    height: 400
    Video {
        id:vidwidget
        anchors.fill: parent
        source: "rtsp://192.168.100.198:554/live"
        autoPlay: true
    }
}
