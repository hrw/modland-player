import QtQuick 2.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.0

Item {
    id: item1
    width: 350
    height: 400
    property alias listViewSongs: listViewSongs
    property alias listViewAuthors: listViewAuthors
    property alias mouseArea: mouseArea
    property alias mouseArea1: mouseArea1

    GridLayout {
        x: 0
        y: 0
        transformOrigin: Item.Center
        anchors.fill: parent
        rows: 2
        columns: 1

        GridLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            transformOrigin: Item.Top
            rows: 2
            columns: 1

            Label {
                id: label
                text: qsTr("Authors")
                transformOrigin: Item.Top
                font.family: "Prototype"
                font.pixelSize: 15
            }
            ListView {
                id: listViewAuthors
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 1
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
                    Label {
                        id: authorlabel
                        font.family: "Prototype"
                        font.pixelSize: 13
                        text: modelData
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
        }
        GridLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
            transformOrigin: Item.Bottom
            rows: 2
            columns: 1

            Label {
                id: label1
                text: qsTr("Modules")
                transformOrigin: Item.Top
                font.family: "Prototype"
                font.pixelSize: 15
            }

            ListView {
                id: listViewSongs
                Layout.fillHeight: true
                Layout.fillWidth: true
                delegate: Row {
                    width: parent.width - 16
                    height: 16
                    property alias modlabel: modlabel
                    Label {
                        id: modlabel
                        font.family: "Prototype"
                        font.pixelSize: 13
                        text: modelData
                    }
                }
                model: null

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
    }
}
