import QtQuick 2.4

PlaylistForm {
    mouseArea {
        onClicked: {
            var ind = Math.floor((mouse.y + listView.contentY)/16)

            if (ind < listView.count)
                listView.currentIndex = ind
        }
    }

    listView.onCurrentIndexChanged: {
        listView1.model = database.getAuthorsModules(listView.currentIndex)
    }

    mouseArea1 {
        onClicked: {
            var ind = Math.floor((mouse.y + listView1.contentY)/16)

            if (ind < listView1.count)
                listView1.currentIndex = ind
        }
    }

    listView1.onCurrentIndexChanged: {
        var mod = listView1.currentItem.modlabel.text
        var aut = listView.currentItem.authorlabel.text

        console.log(aut + ": " + mod);

        player.loadFromData(database.downloadModule(aut, mod))
        player.playStart()
    }

    Connections {
        target: player

        onPlayFinished: {
            console.log("player finished");

            if (listView1.currentIndex == listView1.count - 1) {
                listView.currentIndex++
            } else {
                listView1.currentIndex++;
            }
        }
    }
}
