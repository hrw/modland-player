import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import XMPlayer 1.0

Item {
    id: item1
    width: 250
    height: 400
    property alias sliderVolume: sliderVolume
    property alias buttonStop: buttonStop
    property alias buttonPause: buttonPause
    property alias buttonPlay: buttonPlay
    property alias buttonNext: buttonNext
    property alias buttonPrev: buttonPrev
    property alias labelPos: labelPos
    property alias labelBPM: labelBPM
    property alias labelTitle: labelTitle
    property alias sliderSeparation: sliderSeparation
    property alias progressBar: progressBar
    property alias textTime: textTime
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

    Label {
        id: textTime
        x: 41
        y: 30
        width: 191
        height: 49
        text: qsTr("0:00")
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
        text: qsTr("")
        font.pixelSize: 15
        font.family: "Prototype"
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

    ColumnLayout {
        x: 8
        y: 336
        transformOrigin: Item.Center

        Label {
            id: label3
            x: 10
            y: 328
            text: qsTr("Stereo separation")
            font.family: "Prototype"
            font.pixelSize: 15
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
    }

    RowLayout {
        x: 8
        y: 162

        Button {
            id: buttonPrev
            text: qsTr("p")
            Layout.preferredHeight: 50
            Layout.preferredWidth: 40
            font.family: "Heydings Controls"
            font.pixelSize: 18
            font.capitalization: Font.MixedCase
        }

        Button {
            id: buttonPlay
            text: qsTr("P")
            Layout.preferredHeight: 50
            Layout.preferredWidth: 40
            font.pixelSize: 18
            font.family: "Heydings Controls"
        }

        Button {
            id: buttonPause
            text: qsTr("u")
            Layout.preferredHeight: 50
            Layout.preferredWidth: 40
            font.family: "Heydings Controls"
            font.pixelSize: 18
            font.capitalization: Font.MixedCase
        }

        Button {
            id: buttonStop
            text: qsTr("S")
            Layout.preferredHeight: 50
            Layout.preferredWidth: 40
            font.pixelSize: 18
            font.family: "Heydings Controls"
        }

        Button {
            id: buttonNext
            text: qsTr("j")
            Layout.preferredHeight: 50
            Layout.preferredWidth: 40
            font.family: "Heydings Controls"
            font.pixelSize: 18
            font.capitalization: Font.MixedCase
        }
    }

    ColumnLayout {
        x: 8
        y: 258

        Label {
            id: label
            text: qsTr("Master volume")
            font.family: "Prototype"
            font.pixelSize: 15
        }

        Slider {
            id: sliderVolume
            Layout.preferredHeight: 40
            Layout.preferredWidth: 222
            orientation: Qt.Horizontal
            to: 1
            value: 1
        }
    }
}
