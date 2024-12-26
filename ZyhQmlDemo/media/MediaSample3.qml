import QtQuick 2.0
import QtMultimedia 5.0

Item {
    width: 800
    height: 400
    MediaPlayer {
        id: player
        source: "rtsp://192.168.100.198:554/live"
        autoLoad: true
        autoPlay: true

        onError: {
            console.error("MediaPlayer错误:", errorString);
        }
        onStatusChanged: {
            if (errorString !== null) {
                console.error("错误信息:", errorString)
            }
            switch (status) {
            case MediaPlayer.UnknownStatus:
                console.log("未知状态")
                break
            case MediaPlayer.NoMedia:
                console.log("播放器没有指定媒体资源，播放停止")
                break
            case MediaPlayer.Loading:
                console.log("播放器正在加载媒体")
                break
            case MediaPlayer.Loaded:
                console.log("媒体已经加载完毕，播放停止")
                break
            case MediaPlayer.Stalled:
                console.log("加载媒体已经停止")
                break
            case MediaPlayer.Buffering:
                console.log("媒体缓冲完成")
                break
            case MediaPlayer.EndOfMedia:
                console.log("媒体播放完毕，播放停止")
                break
            case MediaPlayer.InvalidMedia:
                console.log("无法播放媒体，播放停止")
                break
            case MediaPlayer.Failed:
                console.error("媒体播放失败:")
                break
            case MediaPlayer.Paused:
                console.log("媒体已暂停.")
                break
            case MediaPlayer.Playing:
                console.log("正在播放媒体...")
                break
            case MediaPlayer.Buffered:
                console.log("准备播放")
                break
            default:
                console.log("default:", status)
                break
            }
        }
    }

    VideoOutput {
        anchors.fill: parent
        source: player
    }

    Component.onCompleted: {
        console.log("Component.onCompleted,准备播放")
        player.play()
    }

    Connections {
        target: player
        onMediaObjectChanged: {
            if (player.mediaObject)
                player.mediaObject.notifyInterval = 50;
        }
    }
}
