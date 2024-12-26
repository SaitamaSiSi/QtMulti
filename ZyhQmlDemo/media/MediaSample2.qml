import QtQuick 2.0
import QtMultimedia 5.0

Item {
    SoundEffect {
        id: beep
        source: "qrc:/musics/sample-3s.wav"
    }
    Rectangle {
        id: button
        anchors.centerIn: parent
        width: 200
        height: 100
        color: "red"
        MouseArea {
            anchors.fill: parent
            onClicked: beep.play()
        }
    }

    /*
    SoundEffect {
        id: swosh
        source: "qrc:/musics/sample-6s.wav"
    }
    transitions: [
        Transition {
            ParallelAnimation {
                ScriptAction { script: swosh.play(); }
                PropertyAnimation { properties: "rotation"; duration: 200; }
            }
        }
    ]
    */
}
