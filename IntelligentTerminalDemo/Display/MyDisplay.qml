import QtQuick 2.0

import "../Components"

Item {
    Rectangle {
        id: leftArea
        width: parent.width / 3
        height: parent.height
        color: "transparent"
        Column {
            width: parent.width
            height: parent.height - 20
            anchors.top: parent.top
            anchors.topMargin: 20
            spacing: 30
            MySelect {
                id: equipSelect
                text: "选择设备"
                labelLength: 1
                selectLength: 2
                width: parent.width
                options: ListModel {
                    ListElement { label: "道路一路信号灯"; value: 1 }
                    ListElement { label: "道路二路人行灯"; value: 2 }
                    ListElement { label: "道路三路电子警察"; value: 3 }
                }
                onSelectChanged: function(label, value){
                    console.debug("选择设备", label, value)
                }
            }
            MyRadioButton {
                id: showSelect
                text: "展示方式"
                labelLength: 1
                radioLength: 2
                width: parent.width
                selectedOption: 1
                options: ListModel {
                    ListElement { label: "实时"; value: 1 }
                    ListElement { label: "历史"; value: 2 }
                }
                onChanged: function(label, value){
                    console.debug("展示时段", label, value)
                }
            }
            MySelect {
                id: timeSelect
                text: "展示时段"
                labelLength: 1
                selectLength: 2
                width: parent.width
                options: ListModel {
                    ListElement { label: "本月"; value: 1 }
                    ListElement { label: "本周"; value: 2 }
                    ListElement { label: "本日"; value: 3 }
                }
                onSelectChanged: function(label, value){
                    console.debug("展示时段", label, value)
                }
            }
            MySelect {
                id: objectSelect
                text: "展示对象"
                labelLength: 1
                selectLength: 2
                width: parent.width
                options: ListModel {
                    ListElement { label: "对象1"; value: 1 }
                    ListElement { label: "对象2"; value: 2 }
                    ListElement { label: "对象3"; value: 3 }
                }
                onSelectChanged: function(label, value){
                    console.debug("展示对象", label, value)
                }
            }
            Rectangle {
                id: buttonArea
                width: parent.width
                height: 40
                color: "transparent"
                MyTextButton {
                    id: resetBt
                    text: "重置"
                    radius: 20
                    anchors.right: checkBt.left
                    anchors.rightMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                }
                MyTextButton {
                    id: checkBt
                    text: "查看"
                    radius: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
    Rectangle {
        id: rightArea
        width: parent.width - leftArea.width - 10
        height: parent.height
        anchors.right: parent.right
        color: "transparent"

        ListModel {
            id: chartData
        }

        MyChartView {
            id: myChartView
            anchors.fill: parent
            points: chartData
            xTitle: "时间(日)"
            yTitle: "电流(A)"
            xMin: 1
        }
        Component.onCompleted: {
            var dataArray = [
                        { x: 1, y: 10 },
                        { x: 2, y: 18 },
                        { x: 3, y: 23 },
                        { x: 4, y: 23 },
                        { x: 5, y: 26 },
                        { x: 6, y: 31 },
                        { x: 7, y: 33 },
                        { x: 8, y: 33 },
                        { x: 9, y: 33 },
                        { x: 10, y: 36 },
                        { x: 11, y: 39 },
                        { x: 12, y: 44 }
                    ];
            for (var i = 0; i < dataArray.length; ++i) {
                chartData.append(dataArray[i]);
            }
        }
    }
}
