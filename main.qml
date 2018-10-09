import QtQuick 2.0
import QtQuick.Controls 2.2
import XMPlayer 1.0

ApplicationWindow {
    width: 640
    height: 400
    visible: true

    title: qsTr("MODLAND Player")

    XMPlayer {
        id: player

        onNameChanged: {
            t.text = player.name
        }

        onRowChanged: {
            r.text = player.row
        }

        onPosChanged: {
            p.text = player.pos
        }
    }

    Text {
        id: t
        x: 0
        y: 0
    }

    Text {
        x: 0
        y: 20
        id: r
    }

    Text {
        id: p
        x:0
        y:40
    }

    Component.onCompleted: {

        player.load("/Users/michal/Downloads/AceMan - El Plato De Pato.xm")
        player.playStart()
    }
}
