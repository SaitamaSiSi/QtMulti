import QtQuick 2.0

Item {
    id: root
    signal clicked3(int type, string msg)
    Text {
        anchors.centerIn: parent
        text: "This is 选项三"
        font.pixelSize: 24
        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.clicked3(1, "ok")
            }
        }
    }
}

