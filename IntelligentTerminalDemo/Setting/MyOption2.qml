import QtQuick 2.0

Item {
    id: root
    signal clicked2(int type, string msg)
    Text {
        anchors.centerIn: parent
        text: "This is 选项二"
        font.pixelSize: 24
        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.clicked2(1, "ok")
            }
        }
    }
}

