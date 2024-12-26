import QtQuick 2.0

Item {
    id: root
    signal clicked4(int type, string msg)
    Text {
        anchors.centerIn: parent
        text: "This is 选项四"
        font.pixelSize: 24
        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.clicked4(1, "ok")
            }
        }
    }
}

