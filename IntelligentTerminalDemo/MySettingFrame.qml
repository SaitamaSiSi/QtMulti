import QtQuick 2.0

Item {
    id: root
    property alias rootWidth: root.width
    property alias rootHeight: root.height
    signal menuChanged(string name)
    property int currentPage: 1

    ListModel {
        id: listModel
        ListElement {
            menuId: 1
            chked: false
            text: "NTP"
            page: "ntp"
            source: "qrc:/Setting/MyNTP.qml"
        }
        ListElement {
            menuId: 2
            chked: false
            text: "选项二"
            page: "opt2"
            source: "qrc:/Setting/MyOption2.qml"
        }
        ListElement {
            menuId: 3
            chked: false
            text: "选项三"
            page: "opt3"
            source: "qrc:/Setting/MyOption3.qml"
        }
        ListElement {
            menuId: 4
            chked: false
            text: "选项四"
            page: "opt4"
            source: "qrc:/Setting/MyOption4.qml"
        }
        ListElement {
            menuId: 5
            chked: false
            text: "选项五"
            page: "opt5"
            source: "qrc:/Setting/MyOption5.qml"
        }
    }
    // 左侧菜单栏
    Rectangle {
        id: leftContainer
        x:50
        width: 170
        height: parent.height - x
        color: "#00000000"

        ListView {
            id: menuList
            width: 170
            height: parent.height
            model: listModel
            delegate: Rectangle {
                height: 60
                Text {
                    width:120
                    height: 46
                    color: model.chked ? "skyblue" : "white"
                    text: model.text
                    x:35
                    y:20
                    font.pixelSize: 24
                    horizontalAlignment: TextEdit.AlignCenter
                    verticalAlignment: TextEdit.AlignVCenter
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            for(var i = 0; i < listModel.count; i++) {
                                var elm = listModel.get(i)
                                elm.chked = false
                            }
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

    // 右侧内容区域
    Loader {
        id: contentLoader
        width: parent.width - leftContainer.width - leftContainer.x * 1.5
        height: leftContainer.height
        anchors.top: leftContainer.top // 与左侧菜单栏顶部对齐
        anchors.left: leftContainer.right // 与左侧菜单栏右侧对齐
        source: ""
        onLoaded: {
            // Access the loaded item and connect signals
            var loadedItem = contentLoader.item;
            if (loadedItem) {
                switch (currentPage) {
                    case 1:
                        // Connect signal from loaded item
                        loadedItem.clicked.connect(handleNtpSignal);
                        break
                    case 2:
                        loadedItem.clicked2.connect(handleOpt2Signal);
                        break
                    case 3:
                        loadedItem.clicked3.connect(handleOpt3Signal);
                        break
                    case 4:
                        loadedItem.clicked4.connect(handleOpt4Signal);
                        break
                    case 5:
                        loadedItem.clicked5.connect(handleOpt5Signal);
                        break
                }
            }
        }
    }

    Component.onCompleted: {
        for (var i = 0; i < listModel.count; i++) {
            var item = listModel.get(i)
            if (item.menuId === currentPage) {
                item.chked = true
                contentLoader.source = item.source
            }
        }
    }

    function handleNtpSignal(type, msg) {
        console.log("signal", type, msg)
    }
    function handleOpt2Signal(type, msg) {
        console.log("signal2", type, msg)
    }
    function handleOpt3Signal(type, msg) {
        console.log("signal3", type, msg)
    }
    function handleOpt4Signal(type, msg) {
        console.log("signal4", type, msg)
    }
    function handleOpt5Signal(type, msg) {
        console.log("signal5", type, msg)
    }
}
