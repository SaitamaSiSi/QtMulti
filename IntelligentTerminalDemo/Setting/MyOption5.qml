import QtQuick 2.0

Item {
    id: root
    signal clicked5(int type, string msg)
    Text {
        anchors.centerIn: parent
        text: "This is 选项五"
        font.pixelSize: 24
        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.clicked5(1, "ok")
            }
        }
    }
}

