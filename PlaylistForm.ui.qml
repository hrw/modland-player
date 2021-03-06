import QtQuick 2.4
import QtQuick.Controls 2.4

Item {
    id: item1
    width: 350
    height: 400
    property alias listViewSongs: listViewSongs
    property alias listViewAuthors: listViewAuthors
    property alias mouseArea: mouseArea
    property alias mouseArea1: mouseArea1

    Label {
        id: label
        x: 8
        y: 8
        text: qsTr("Authors")
        font.family: "Prototype"
        font.pixelSize: 15
    }

    ListView {
        id: listViewAuthors
        clip: true
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 32
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 210
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 200
        highlight: Rectangle {
            color: "dimgray"
            radius: 3
        }
        delegate: Row {
            width: parent.width - 16
            height: 16
            property alias authorlabel: authorlabel
            property int id: modelData.id
            Label {
                id: authorlabel
                font.family: "Prototype"
                font.pixelSize: 13
                text: modelData.name
            }
        }
        model: database.authors

        MouseArea {
            id: mouseArea
            anchors.bottomMargin: 0
            anchors.fill: parent
        }

        ScrollBar.vertical: ScrollBar {
            minimumSize: 0.1
        }
    }

    Label {
        id: label1
        x: 8
        y: 208
        text: qsTr("Modules")
        font.family: "Prototype"
        font.pixelSize: 15
    }

    ListView {
        id: listViewSongs
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 232
        clip: true
        delegate: Row {
            width: parent.width - 16
            height: 16
            property alias modlabel: modlabel
            property int id: modelData.id
            property int authorId: modelData.authorId
            property string name: modelData.name
            property string path: modelData.path
            Label {
                id: modlabel
                font.family: "Prototype"
                font.pixelSize: 13
                text: modelData.name
            }
        }
        model: database.authorsModules

        highlightFollowsCurrentItem: true
        highlightMoveDuration: 200
        highlight: Rectangle {
            color: "dimgray"
            radius: 3
        }

        MouseArea {
            id: mouseArea1
            anchors.fill: parent
        }

        ScrollBar.vertical: ScrollBar {
            minimumSize: 0.1
        }
    }
}
