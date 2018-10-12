import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: item1
    width: 200
    height: 400

    Label {
        id: label
        text: qsTr("Instruments")
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        font.family: "Prototype"
        font.pixelSize: 15
    }

    ListView {
        id: listView
        anchors.bottomMargin: 8
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        anchors.topMargin: 32
        anchors.fill: parent
        clip: true
        delegate: Row {
            Label {
                font.family: "Prototype"
                font.pixelSize: 13
                text: modelData
            }
        }
        model: player.instruments
    }
}
