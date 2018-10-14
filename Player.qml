import QtQuick 2.4

PlayerForm {

    Connections {
        target: player

        onVuLeftChanged: {
            signalLeft.value = player.vuLeft
        }
        onVuRightChanged: {
            signalRight.value = player.vuRight
        }
        onTimeChanged: {
            var t = Math.round(player.time / 1000);
            var t_s = t % 60
            var t_m = Math.floor(t / 60)
            textTime.text = t_m + ":" + (t_s < 10 ? "0":"") + t_s.toString()
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

    buttonPause {
        onClicked: {
            player.playPause()
        }
    }
}
