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
        anchors.fill: parent
        Player {
            id: playerView
            Layout.fillHeight: true
        }

        Instruments {
            id: instrumentView
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }

        Playlist {
            id: playlist
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
        }
    }

    Component.onCompleted: {

        //player.load("/Users/michal/a sleep so deep.mod")
        //player.load("/Users/michal/Downloads/AceMan - El Plato De Pato.xm")
        //player.playStart()
    }
}
