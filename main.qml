import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import XMPlayer 1.0

ApplicationWindow {
    width: 640
    height: 400
    visible: true
    /*maximumWidth: 640
    maximumHeight: 400
    minimumWidth: 640
    minimumHeight: 400
*/
    title: qsTr("Modland Player")

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    XMPlayer {
        id: player
    }

    Player {
        id: playerView
    }

    Component.onCompleted: {

        player.load("/Users/michal/Downloads/proba mikrofonu.mod")
        //player.load("/Users/michal/Downloads/AceMan - El Plato De Pato.xm")
        player.playStart()
    }
}
