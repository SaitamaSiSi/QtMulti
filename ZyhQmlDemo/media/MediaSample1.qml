import QtQuick 2.0
import QtMultimedia 5.0

Item {
    width: 512; height: 300

    MediaPlayer {
        id: player
        source: "qrc:/videos/video.mp4"
        autoLoad: true
        loops: MediaPlayer.Infinite

        onStatusChanged: {
            switch (status) {
            case MediaPlayer.UnknownStatus:
                // 未知状态
                break
            case MediaPlayer.NoMedia:
                //  播放器没有指定媒体资源，播放停止
                break
            case MediaPlayer.Loading:
                // 播放器正在加载媒体
                break
            case MediaPlayer.Loaded:
                // 媒体已经加载完毕，播放停止
                break
            case MediaPlayer.Stalled:
                // 加载媒体已经停止
                break
            case MediaPlayer.Buffering:
                // 媒体缓冲完成
                break
            case MediaPlayer.EndOfMedia:
                // 媒体播放完毕，播放停止
                break
            case MediaPlayer.InvalidMedia:
                // 无法播放媒体，播放停止
                break
            case MediaPlayer.Failed:
                // console.error("媒体播放失败:", errorString)
                break
            case MediaPlayer.Paused:
                // console.log("媒体已暂停.")
                break
            case MediaPlayer.Playing:
                // console.log("正在播放媒体...")
                break
            case MediaPlayer.Buffered:
                // 准备播放
                break
            }
        }
    }

    VideoOutput {
        anchors.fill: parent
        source: player
    }

    Component.onCompleted: {
        player.play()
    }

    Rectangle {
        id: progressBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 100
        height: 30
        color: "lightGray"
        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: player.duration > 0 ? parent.width * player.position / player.duration : 0
            color: "darkGray"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (player.seekable)
                    player.seek(player.duration * mouse.x / width)
            }
        }
    }

    Connections {
        target: player
        onMediaObjectChanged: {
            if (player.mediaObject)
                player.mediaObject.notifyInterval = 50;
        }
    }

    /* import QtSystemInfo 5.0
    ScreenSaver {
        screenSaverEnabled: false
    }
    */
}
