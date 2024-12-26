import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root
    property string imgSource: ""
    function setImgSource(imgSrc) {
        imageViewer.source = imgSrc
    }

    Rectangle {
        id: text
        anchors.fill: parent
        color: "#00955656"

        // 用来显示一个等待图元
        BusyIndicator {
            id: busy
            running: true
            anchors.centerIn: parent
            z: 2
        }

        Text {
            id: stateLabel
            visible: false
            anchors.centerIn: parent
            z: 3
        }

        AnimatedImage
        //Image
        {
            id: imageViewer
            // 开启异步加载模式，专门使用一个线程来加载图片
            asynchronous: true
            // 图片较大的情况下,指定不缓存图像(cache默认为true)
            //cache: false
            anchors.fill: parent
            onStatusChanged: {
                if (imageViewer.status === Image.Loading) {
                    busy.running = true;
                    stateLabel.visible = false
                }
                else if(imageViewer.status === Image.Ready){
                    stateLabel.visible = false
                    busy.running = false;
                    playing = true;
                }
                else if(imageViewer.status === Image.Error) {
                    busy.running = false;
                    stateLabel.visible = true
                    stateLabel.text = "Error"
                }
                else if(imageViewer.status === Image.Null)
                {
                    busy.running = false;
                    stateLabel.visible = true
                    stateLabel.text = "no image has been set"
                }
            }

            // 组件加载完成再加载图片
            Component.onCompleted: {
                imageViewer.source = imgSource
            }
        }
    }
}
