import QtQuick 2.0

Item {
    id: root
    property alias rootWidth: root.width
    property alias rootHeight: root.height
    signal menuChanged(string name)
    property int currentPage: 1

    ListModel {
        id: leftListModel
        ListElement {
            menuId: 1
            chked: false
            text: "系统状态"
            icon_img: "qrc:/resources/images/icon系统状态.png"
            icon_img2: "qrc:/resources/images/icon系统状态-选中.png"
            txt_img: "qrc:/resources/images/系统状态-发光.png"
            txt_img2: "qrc:/resources/images/系统状态-发光.png"
            page: "System"
            source: "qrc:/System/SystemInfo.qml"
        }
        ListElement {
            menuId: 2
            chked: false
            text: "电力状态"
            icon_img: "qrc:/resources/images/icon电力状态.png"
            icon_img2: "qrc:/resources/images/icon电力状态-选中.png"
            txt_img: "qrc:/resources/images/电力状态-发光.png"
            txt_img2: "qrc:/resources/images/电力状态-发光.png"
            page: "Electricity"
            source: "qrc:/Electricity/ElectricityInfo.qml"
        }
        ListElement {
            menuId: 3
            chked: false
            text: "柜门状态"
            icon_img: "qrc:/resources/images/icon柜门状态.png"
            icon_img2: "qrc:/resources/images/icon柜门状态-选中.png"
            txt_img: "qrc:/resources/images/柜门状态-发光.png"
            txt_img2: "qrc:/resources/images/柜门状态-发光.png"
            page: "Door"
            source: "qrc:/Door/DoorInfo.qml"
        }
        ListElement {
            menuId: 4
            chked: false
            text: "环境状态"
            icon_img: "qrc:/resources/images/icon环境状态.png"
            icon_img2: "qrc:/resources/images/icon环境状态-选中.png"
            txt_img: "qrc:/resources/images/环境状态-发光.png"
            txt_img2: "qrc:/resources/images/环境状态-发光.png"
            page: "Environment"
            source: "qrc:/Environment/EnvInfo.qml"
        }
    }
    ListModel {
        id: rightListModel
        ListElement {
            menuId: 5
            chked: false
            text: "设备管理"
            icon_img: "qrc:/resources/images/设备管理1.png"
            icon_img2: "qrc:/resources/images/设备管理2.png"
            txt_img: "qrc:/resources/images/设备管理-未选中.png"
            txt_img2: "qrc:/resources/images/设备管理-选中.png"
            page: "equip"
            source: "qrc:/Equip/MyEquip.qml"
        }
        ListElement {
            menuId: 6
            chked: false
            text: "数据展示"
            icon_img: "qrc:/resources/images/电流电压1.png"
            icon_img2: "qrc:/resources/images/电流电压2.png"
            txt_img: "qrc:/resources/images/数据展示-未选中.png"
            txt_img2: "qrc:/resources/images/数据展示-选中.png"
            page: "display"
            source: "qrc:/Display/MyDisplay.qml"
        }
        ListElement {
            menuId: 7
            chked: false
            text: "方案分析"
            icon_img: "qrc:/resources/images/方案优化1.png"
            icon_img2: "qrc:/resources/images/方案优化2.png"
            txt_img: "qrc:/resources/images/方案分析-未选中.png"
            txt_img2: "qrc:/resources/images/方案分析-选中.png"
            page: "analysis"
            source: "qrc:/Analysis/MyAnalysis.qml"
        }
        ListElement {
            menuId: 5
            chked: false
            text: "防雷参数"
            icon_img: "qrc:/resources/images/icon防雷参数.png"
            icon_img2: "qrc:/resources/images/icon防雷参数-选中.png"
            txt_img: "qrc:/resources/images/防雷参数-发光.png"
            txt_img2: "qrc:/resources/images/防雷参数-发光.png"
            page: "LightningProtection"
            source: "qrc:/LightningProtection/LightningProtection.qml"
        }
        ListElement {
            menuId: 6
            chked: false
            text: "UPS状态"
            icon_img: "qrc:/resources/images/iconUPS状态.png"
            icon_img2: "qrc:/resources/images/iconUPS状态-选中.png"
            txt_img: "qrc:/resources/images/UPS状态-发光.png"
            txt_img2: "qrc:/resources/images/UPS状态-发光.png"
            page: "USP"
            source: "qrc:/USP/USPInfo.qml"
        }
        ListElement {
            menuId: 7
            chked: false
            text: "视频监控"
            icon_img: "qrc:/resources/images/icon视频监控.png"
            icon_img2: "qrc:/resources/images/icon视频监控-选中.png"
            txt_img: "qrc:/resources/images/视频监控-发光.png"
            txt_img2: "qrc:/resources/images/视频监控-发光.png"
            page: "Video"
            source: "qrc:/Video/VideoMonitoring.qml"
        }
        ListElement {
            menuId: 8
            chked: false
            text: "风机状态"
            icon_img: "qrc:/resources/images/icon风机状态.png"
            icon_img2: "qrc:/resources/images/icon风机状态-选中.png"
            txt_img: "qrc:/resources/images/风机状态-发光.png"
            txt_img2: "qrc:/resources/images/风机状态-发光.png"
            page: "Fan"
            source: "qrc:/Fan/FanInfo.qml"
        }
    }
    // 左侧菜单栏
    Rectangle {
        id: leftContainer
        anchors.left: parent.left
        anchors.leftMargin: 50
        y: 30
        width: 170
        height: parent.height - x
        color: "#00000000"

        ListView {
            width: parent.width
            height: parent.height
            model: leftListModel
            delegate: Rectangle {
                height: 80
                Image {
                    width:30
                    height: 30
                    y: 30
                    source: model.chked ? model.icon_img2 : model.icon_img
                }
                Image {
                    width:120
                    height: 46
                    source: model.chked ? model.txt_img2 : model.txt_img
                    x: 35
                    y: 20
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            reset()

                            currentPage = model.menuId
                            contentLoader.source = model.source
                            model.chked = true
                            root.menuChanged(model.page)
                        }
                    }
                }
            }
        }
    }

    // 右侧菜单栏
    Rectangle {
        id: rightContainer
        anchors.top: leftContainer.top
        anchors.right: parent.right
        width: 195
        height: leftContainer.height
        color: "#00000000"

        ListView {
            width: 195
            height: parent.height
            model: rightListModel
            delegate: Rectangle {
                height: 80
                Image {
                    width:30
                    height: 30
                    y:30
                    source: model.chked ? model.icon_img2 : model.icon_img
                }
                Image {
                    width:120
                    height: 46
                    source: model.chked ? model.txt_img2 : model.txt_img
                    x:35
                    y:20
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            reset()

                            currentPage = model.menuId
                            contentLoader.source = model.source
                            model.chked = true
                            root.menuChanged(model.page)
                        }
                    }
                }
            }
        }
    }

    // 内容区域
    Loader {
        id: contentLoader
        height: leftContainer.height
        anchors.top: leftContainer.top // 与左侧菜单栏顶部对齐
        anchors.left: leftContainer.right // 与左侧菜单栏右侧对齐
        anchors.right: rightContainer.left
        anchors.rightMargin: 25
        source: "" // 默认加载第一个项目的内容
    }

    Component.onCompleted: {
        var isFind = false
        var item = null
        var i = 0
        for (i = 0; i < leftListModel.count; i++) {
            item = leftListModel.get(i)
            if (item.menuId === currentPage) {
                item.chked = true
                contentLoader.source = item.source
                isFind = true
            }
        }
        if (!isFind) {
            for (i = 0; i < rightListModel.count; i++) {
                item = rightListModel.get(i)
                if (item.menuId === currentPage) {
                    item.chked = true
                    contentLoader.source = item.source
                }
            }
        }
    }

    function reset() {
        var elm = null
        var i = 0
        for(i = 0; i < leftListModel.count; i++) {
            elm = leftListModel.get(i)
            elm.chked = false
        }
        for(i = 0; i < rightListModel.count; i++) {
            elm = rightListModel.get(i)
            elm.chked = false
        }
    }
}
