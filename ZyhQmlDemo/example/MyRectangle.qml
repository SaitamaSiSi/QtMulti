import QtQuick 2.0

Rectangle {
    id: rect
    width: 120
    height: 120
    color: "#D8D8D8"
    radius: 8
    //border.color: "lightsteelblue"
    gradient:
        Gradient {
        GradientStop {
            position:	0.0;	color:	"lightsteelblue"
        }
        GradientStop {
            position:	1.0;	color:	"slategray"
        }
    }
    border.color:	"slategray"
    border.width: 4
    x: 20
    y: 20

    Image {
        id: star
        source: '../images/star.png'
        anchors.fill: parent
        anchors.centerIn: rect
    }

    Text {
        id: label
        width: rect.width
        height: rect.height

        // 长文本省略 '...' shall appear in the middle
        elide:	Text.ElideMiddle
        // red	sunken text styling
        style:	Text.Sunken
        styleColor:	'#FF4444'
        verticalAlignment:	Text.AlignVCenter
        horizontalAlignment: Text.Center
        // only sensible when no elide mode
        // wrapMode: Text.WordWrap

        color: focus ? "red" : "black"

        property int spacePresses: 0
        text: "Star Space pressed: " + spacePresses + " times"
        onTextChanged: console.log("text change to:", text)
        Keys.onSpacePressed: {
            console.log("push space button")
            increment()
        }
        Keys.onEnterPressed: {
            console.log("push enter button")
            increment()
        }
        Keys.onReturnPressed: {
            console.log("push return button")
            increment()
        }
        Keys.onEscapePressed: {
            console.log("push escape button")
            label.text = ''
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.forceActiveFocus()
                console.log("clicked")
                label.increment()
            }
        }
        function increment() {
            spacePresses = spacePresses + 1
        }
    }
}
