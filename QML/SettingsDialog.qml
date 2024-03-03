import QtQuick
import QtQuick.Controls
import QtTextToSpeech
import Qt.labs.settings 1.0

Dialog {
    property TextToSpeech ttsEngine

    id: settingsDialog
    title: "Settings"
    standardButtons: Dialog.Ok | Dialog.Cancel
    visible: false

    Column {
        id: settingsColumn
        spacing: 10
        CheckBox {
            checked: settings.ttsEnabled
            id: enableSpeech
            text: "Enable speech synthesizer"
        }
        // text label, input field and text label in one line
        Row {
            spacing: 10
            Label {
                // align in the middle of the row
                anchors.verticalCenter: parent.verticalCenter
                enabled: enableSpeech.checked
                text: "Announce hostiles within"
            }
            TextField {
                enabled: enableSpeech.checked
                id: minJumps
                width: 50
                placeholderText: "10"
                text: settings.jumpRange
                validator: IntValidator {
                    bottom: 1
                }
            }
            Label {
                enabled: settings.ttsEnabled
                anchors.verticalCenter: parent.verticalCenter
                text: "jumps"
            }

        }

        CheckBox {
            checked: settings.turnurTheraAlert
            enabled: enableSpeech.checked
            id: enableTheraTurnurAnnounce
            text: "Announce nearby connections to Thera / Turnur"
        }
    }

    Settings {
        category: "TTS"
        id: settings
        property bool ttsEnabled
        property int jumpRange
        property bool turnurTheraAlert
    }

    onAccepted: {
        if (minJumps.text === "" || isNaN(minJumps.text)) {
            if (minJumps.text === "") {
                minJumps.text = "10"
            }
        }
        settings.ttsEnabled = enableSpeech.checked
        settings.jumpRange = parseInt(minJumps.text)
        settings.turnurTheraAlert = enableTheraTurnurAnnounce.checked
        settings.sync()
    }

    function getTtsEnabled() {
        return settings.ttsEnabled
    }

    function getJumpRange() {
        return settings.jumpRange
    }

    function getTurnurTheraAlert() {
        return settings.turnurTheraAlert
    }

}
