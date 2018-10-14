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
        database.getListForAuthor(listViewAuthors.currentItem.id)
        listViewSongs.model = database.authorsModules
    }

    mouseArea1 {
        onClicked: {
            var ind = Math.floor((mouse.y + listViewSongs.contentY)/16)

            if (ind < listViewSongs.count)
                listViewSongs.currentIndex = ind
        }
    }

    listViewSongs.onCurrentIndexChanged: {
        player.loadFromData(database.downloadModule(listViewSongs.currentItem.path))
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
