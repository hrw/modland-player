import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4

Item {
    width: 210
    height: 400
    property alias slider1: slider1
    property alias slider: slider
    property alias label: label
    property alias progressBar: progressBar
    property alias text1: text1
    property alias button1: button1
    property alias button: button
    property alias signalRight: signalRight
    property alias signalLeft: signalLeft

    ProgressBar {
        id: progressBar
        x: 10
        y: 134
        width: 189
        height: 16
        value: 0.5
    }

    Image {
        id: image
        x: 8
        y: 87
        width: 191
        height: 20
        clip: false
        fillMode: Image.PreserveAspectCrop
        source: "qrc:img/bargraph_transp_off_20.png"
    }

    Image {
        id: signalLeft
        x: 8
        y: 86
        width: 130
        height: 20
        transformOrigin: Item.Center
        horizontalAlignment: Image.AlignLeft
        fillMode: Image.Tile
        source: "qrc:img/bargraph_transp_20.png"
        clip: false
    }

    Image {
        id: image1
        x: 8
        y: 108
        width: 191
        height: 20
        fillMode: Image.PreserveAspectCrop
        source: "qrc:img/bargraph_transp_off_20.png"
        clip: false
    }

    Image {
        id: signalRight
        x: 8
        y: 108
        width: 130
        height: 20
        horizontalAlignment: Image.AlignLeft
        transformOrigin: Item.Center
        fillMode: Image.Tile
        source: "qrc:img/bargraph_transp_20.png"
        clip: false
    }

    Button {
        id: button
        x: 10
        y: 162
        width: 60
        height: 40
        text: qsTr("▶️")
    }

    Button {
        id: button1
        x: 142
        y: 162
        width: 60
        height: 40
        text: qsTr("■")
    }

    Label {
        id: text1
        x: 8
        y: 30
        width: 191
        height: 49
        text: qsTr("Text")
        font.family: "Verdana"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        font.pixelSize: 50
    }

    Label {
        id: label
        x: 10
        y: 8
        width: 189
        height: 16
        text: qsTr("Label")
    }

    Slider {
        id: slider
        x: 10
        y: 208
        width: 45
        height: 184
        orientation: Qt.Vertical
        to: 1
        value: 1
    }

    Slider {
        id: slider1
        x: 61
        y: 208
        width: 45
        height: 184
        orientation: Qt.Vertical
        value: 0.5
    }
}
