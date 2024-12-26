import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

Rectangle{
    id:rootRectangle
    anchors.fill: parent
    anchors.margins: 5

    //设置
    SerialLeftArea{
        id:leftArea
        anchors.left: rootRectangle.left
        anchors.top: rootRectangle.top
        width: 180
        height: parent.height
    }

    //发送 与接收显示
    SerialCenterArea{
        id:centerArea
        anchors.left: leftArea.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        showSend: leftArea.showSend
        repeatSend: leftArea.repeatSend
    }
}
