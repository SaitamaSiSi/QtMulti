import QtQuick 2.0
import QtMultimedia 5.0

Item {
    id: root
    property int index: -1
    property MediaPlayer mediaPlayer
    property ListModel items: ListModel {}
    function setIndex(i)
    {
        console.log("setting index to: " + i);
        index = i;
        if (index < 0 || index >= items.count)
        {
            index = -1;
            mediaPlayer.source = "";
        }
        else
            mediaPlayer.source = items.get(index).source;
    }
    function start()
    {
        if(index === -1) {
            console.log("You need to setIndex");
        } else {
            mediaPlayer.play();
        }
    }
    function next()
    {
        if (index + 1 >= items.count) {
            setIndex(0);
        } else {
            setIndex(index + 1);
        }
    }
    function previous()
    {
        if (index < 0) {
            setIndex(items.count - 1);
        } else {
            setIndex(index - 1);
        }
    }

    Connections {
        target: root.mediaPlayer
        onStopped: {
            if (root.mediaPlayer.status === MediaPlayer.EndOfMedia)
            {
                root.next();
                if (root.index == -1)
                    root.mediaPlayer.stop();
                else
                    root.mediaPlayer.play();
            }
        }
    }
}
