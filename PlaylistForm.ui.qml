import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: item1
    width: 400
    height: 400
    property alias listView: listView
    property alias mouseArea: mouseArea

    Label {
        id: label
        x: 8
        y: 8
        text: qsTr("Authors")
        font.family: "Prototype"
        font.pixelSize: 15
    }

    ListView {
        id: listView
        clip: true
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 32
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 195
        highlightFollowsCurrentItem: true
        highlight: Rectangle {
            color: "lightsteelblue"
            radius: 5
        }
        focus: true
        delegate: Item {
            x: 5
            width: 80
            height: 40
            Row {
                id: row1
                Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                }

                Text {
                    text: name
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                spacing: 10
            }
        }
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        MouseArea {
            id: mouseArea
            anchors.bottomMargin: 0
            anchors.fill: parent
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
        id: listView1
        x: 8
        y: 232
        width: 384
        height: 160
        clip: true
        delegate: Item {
            x: 5
            width: 80
            height: 40
            Row {
                id: row2
                Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                }

                Text {
                    text: name
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                spacing: 10
            }
        }
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
    }
}
