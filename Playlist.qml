import QtQuick 2.4

PlaylistForm {
    mouseArea {
        onClicked: {
            if (listView.indexAt(mouse.x, mouse.y) != -1)
                listView.currentIndex = listView.indexAt(mouse.x, mouse.y)
        }
    }
}
