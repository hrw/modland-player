import QtQuick 2.4

PlayerForm {
    property real vl: 0
    property real vr: 0

    Timer {
        interval: 20
        repeat: true
        running: true
        onTriggered: {

            signalLeft.value = vl
            signalRight.value = vr

            if (vl > 0.02)
                vl = vl * 0.92
            else vl = 0

            if (vr > 0.02)
                vr = vr * 0.92
            else vr = 0

        }
    }

    Connections {
        target: player

        onVuLeftChanged: {
            if (player.vuLeft > vl)
                vl = player.vuLeft;
        }
        onVuRightChanged: {
            if (player.vuRight > vr)
                vr = player.vuRight
        }
        onTimeChanged: {
            var t = Math.round(player.time / 1000);
            var t_s = t % 60
            var t_m = Math.floor(t / 60)
            text1.text = t_m + ":" + (t_s < 10 ? "0":"") + t_s.toString()
            progressBar.value = player.time / player.totalTime
        }
        onNameChanged: {
            labelTitle.text = player.name
        }
        onBpmChanged: {
            labelBPM.text = "BPM: " + player.bpm
        }
        onPosChanged: {
            labelPos.text = "Pos: " + player.pos + "/" + player.len
        }

        onMixChanged: {
            sliderSeparation.value = player.mix / 100;
        }
        onVolumeChanged: {
            sliderVolume.value = player.volume / 100;
        }
    }

    sliderVolume {
        onValueChanged: {
            player.volume = sliderVolume.value * 100;
        }
    }

    sliderSeparation {
        onValueChanged: {
            player.mix = sliderSeparation.value * 100;
        }
    }

    buttonPlay {
        onClicked: {
            player.playStart()
        }
    }

    buttonStop {
        onClicked: {
            player.playStop()
        }
    }
}
