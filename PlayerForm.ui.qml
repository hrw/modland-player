import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4

Item {
    width: 250
    height: 400
    property alias labelPos: labelPos
    property alias labelBPM: labelBPM
    property alias labelTitle: labelTitle
    property alias slider1: slider1
    property alias slider: slider
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
        width: 222
        height: 16
        value: 0.5
    }

    Button {
        id: button
        x: 54
        y: 162
        width: 40
        height: 40
        text: qsTr("▶️")
    }

    Button {
        id: button1
        x: 146
        y: 162
        width: 40
        height: 40
        text: qsTr("■")
    }

    Label {
        id: text1
        x: 41
        y: 30
        width: 191
        height: 49
        text: qsTr("Text")
        font.family: "Prototype"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        font.pixelSize: 50
    }

    Label {
        id: labelTitle
        x: 10
        y: 8
        width: 232
        height: 16
        text: qsTr("Label")
        font.pixelSize: 15
        font.family: "Prototype"
    }

    Slider {
        id: slider
        x: 10
        y: 282
        width: 222
        height: 40
        orientation: Qt.Horizontal
        to: 1
        value: 1
    }

    Slider {
        id: slider1
        x: 10
        y: 352
        width: 222
        height: 40
        orientation: Qt.Horizontal
        value: 0.5
    }

    Button {
        id: button2
        x: 100
        y: 162
        width: 40
        height: 40
        text: qsTr("Ⅱ")
    }

    Button {
        id: button3
        x: 8
        y: 162
        width: 40
        height: 40
        text: qsTr("⏮️")
        display: AbstractButton.TextOnly
    }

    Button {
        id: button4
        x: 192
        y: 162
        width: 40
        height: 40
        text: qsTr("⏭")
    }

    Label {
        id: label1
        x: 10
        y: 90
        width: 32
        height: 16
        text: qsTr("dB-L")
        font.pixelSize: 13
        font.family: "Prototype"
    }

    Label {
        id: label2
        x: 10
        y: 111
        width: 32
        height: 16
        text: qsTr("dB-R")
        font.family: "Prototype"
        font.pixelSize: 13
    }

    Image {
        id: image1
        x: 41
        y: 108
        width: 191
        height: 20
        fillMode: Image.PreserveAspectCrop
        source: "qrc:img/bargraph_transp_off_20.png"
        clip: false
    }

    Image {
        id: signalRight
        x: 41
        y: 108
        width: 130
        height: 20
        horizontalAlignment: Image.AlignLeft
        transformOrigin: Item.Center
        fillMode: Image.Tile
        source: "qrc:img/bargraph_transp_20.png"
        clip: false
    }

    Image {
        id: image
        x: 41
        y: 87
        width: 191
        height: 20
        clip: false
        fillMode: Image.PreserveAspectCrop
        source: "qrc:img/bargraph_transp_off_20.png"
    }
    Image {
        id: signalLeft
        x: 41
        y: 87
        width: 130
        height: 20
        transformOrigin: Item.Center
        horizontalAlignment: Image.AlignLeft
        fillMode: Image.Tile
        source: "qrc:img/bargraph_transp_20.png"
        clip: false
    }

    Label {
        id: labelBPM
        x: 10
        y: 44
        text: qsTr("BPM: ")
        font.pixelSize: 12
        font.family: "Prototype"
    }

    Label {
        id: labelPos
        x: 10
        y: 60
        text: qsTr("Pos:")
        font.pixelSize: 12
        font.family: "Prototype"
    }

    Label {
        id: label
        x: 10
        y: 258
        text: qsTr("Master volume")
        font.family: "Prototype"
        font.pixelSize: 15
    }

    Label {
        id: label3
        x: 10
        y: 328
        text: qsTr("Stereo separation")
        font.family: "Prototype"
        font.pixelSize: 15
    }
}
