import QtQuick 2.4

PlaylistForm {
    mouseArea {
        onClicked: {
            var ind = Math.floor((mouse.y + listViewAuthors.contentY)/16)

            if (ind < listViewAuthors.count)
                listViewAuthors.currentIndex = ind
        }
    }

    listViewAuthors.onCurrentIndexChanged: {
        listViewSongs.model = database.getAuthorsModules(listViewAuthors.currentIndex)
    }

    mouseArea1 {
        onClicked: {
            var ind = Math.floor((mouse.y + listViewSongs.contentY)/16)

            if (ind < listViewSongs.count)
                listViewSongs.currentIndex = ind
        }
    }

    listViewSongs.onCurrentIndexChanged: {
        var mod = listViewSongs.currentItem.modlabel.text
        var aut = listViewAuthors.currentItem.authorlabel.text

        console.log(aut + ": " + mod);

        player.loadFromData(database.downloadModule(aut, mod))
        player.playStart()
    }

    Connections {
        target: player

        onPlayFinished: {
            console.log("player finished");

            if (listViewSongs.currentIndex == listViewSongs.count - 1) {
                listViewAuthors.currentIndex++
            } else {
                listViewSongs.currentIndex++;
            }
        }
    }
}
