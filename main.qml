import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import XMPlayer 1.0

ApplicationWindow {
    width: 800
    height: 400
    visible: true
    maximumWidth: 800
    maximumHeight: 400
    minimumWidth: 800
    minimumHeight: 400

    title: qsTr("Modland Player")

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    XMPlayer {
        id: player
    }

    Modland {
        id: database
    }

    RowLayout {
        Player {
            id: playerView
        }

        Instruments {
            id: instrumentView
        }

        Playlist {
            id: playlist
        }
    }

    Component.onCompleted: {

        //player.load("/Users/michal/a sleep so deep.mod")
        //player.load("/Users/michal/Downloads/AceMan - El Plato De Pato.xm")
        //player.playStart()
    }
}
