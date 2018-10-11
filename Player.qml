import QtQuick 2.4

PlayerForm {
    property real vl: 0
    property real vr: 0

    Timer {
        interval: 20
        repeat: true
        running: true
        onTriggered: {

            signalLeft.width = 2 + 6*Math.round(31 * vl)
            signalRight.width = 2 + 6*Math.round(31 * vr)

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
            label.text = player.name
        }
        onMixChanged: {
            slider1.value = player.mix / 100;
        }
        onVolumeChanged: {
            slider.value = player.volume / 100;
        }
    }

    slider {
        onValueChanged: {
            player.volume = slider.value * 100;
        }
    }

    slider1 {
        onValueChanged: {
            player.mix = slider1.value * 100;
        }
    }

    button {
        onClicked: {
            player.playStart()
        }
    }

    button1 {
        onClicked: {
            player.playStop()
        }
    }
}
