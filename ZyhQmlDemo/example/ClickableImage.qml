import QtQuick 2.0

Image {
    id:root_img
    signal clicked
    MouseArea {
        anchors.fill: root_img
        onClicked: root_img.clicked()
    }
}
