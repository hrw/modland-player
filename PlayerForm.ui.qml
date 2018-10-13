import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import XMPlayer 1.0

Item {
    width: 250
    height: 400
    property alias sliderVolume: sliderVolume
    property alias buttonStop: buttonStop
    property alias buttonPlay: buttonPlay
    property alias labelPos: labelPos
    property alias labelBPM: labelBPM
    property alias labelTitle: labelTitle
    property alias sliderSeparation: sliderSeparation
    property alias progressBar: progressBar
    property alias text1: text1
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
        id: buttonPlay
        x: 54
        y: 162
        width: 40
        height: 50
        text: qsTr("P")
        font.pixelSize: 18
        font.family: "Heydings Controls"
    }

    Button {
        id: buttonStop
        x: 146
        y: 162
        width: 40
        height: 50
        text: qsTr("S")
        font.pixelSize: 18
        font.family: "Heydings Controls"
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
        id: sliderVolume
        x: 10
        y: 282
        width: 222
        height: 40
        orientation: Qt.Horizontal
        to: 1
        value: 1
    }

    Slider {
        id: sliderSeparation
        x: 10
        y: 352
        width: 222
        height: 40
        orientation: Qt.Horizontal
        value: 0.5
    }

    Button {
        id: buttonPause
        x: 100
        y: 162
        width: 40
        height: 50
        text: qsTr("u")
        font.family: "Heydings Controls"
        font.pixelSize: 18
        font.capitalization: Font.MixedCase
    }

    Button {
        id: buttonPrev
        x: 8
        y: 162
        width: 40
        height: 50
        text: qsTr("p")
        font.family: "Heydings Controls"
        font.pixelSize: 18
        font.capitalization: Font.MixedCase
    }

    Button {
        id: buttonNext
        x: 192
        y: 162
        width: 40
        height: 50
        text: qsTr("j")
        font.family: "Heydings Controls"
        font.pixelSize: 18
        font.capitalization: Font.MixedCase
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

    VUMeter {
        id: signalRight
        x: 45
        y: 111
        width: 185
        height: 14
        minimumValue: 0
        maximumValue: 1
    }

    VUMeter {
        id: signalLeft
        x: 45
        y: 90
        width: 185
        height: 14
        minimumValue: 0
        maximumValue: 1
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
