import QtQuick 2.0
import QtMultimedia 5.0

import "../components"

Item {
    width: 800
    height: 400
    MyPlaylist {
        id: playlist
        mediaPlayer: player
        items: ListModel {
            ListElement { source: "qrc:/videos/video.mp4" }
            ListElement { source: "qrc:/videos/video2.mp4" }
        }
    }
    MediaPlayer {
        id: player
    }
    VideoOutput {
        anchors.fill: parent
        source: player
    }
    Component.onCompleted: {
        playlist.setIndex(0)
        playlist.start()
    }
}
