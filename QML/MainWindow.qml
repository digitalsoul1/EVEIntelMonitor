import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import QtQuick.Layouts
import Qt.labs.qmlmodels
import Qt.labs.settings
import QtTextToSpeech
import EVEIntelMonitor
import Qt5Compat.GraphicalEffects

ApplicationWindow {
    id: applicationWindow
    width: screen.width / 2
    height: screen.height / 2
    title: qsTr("EVEIntelMonitor")
    visible: true

    Controller {
        id: myController
    }


    TextToSpeech {
        id: tts
        volume: 1.0
        VoiceSelector.language: Qt.locale("en_US")
        VoiceSelector.gender: Voice.Female
        onStateChanged: ttsStateDebug(tts.state)

        // macos is deprecated, but it's the only one that works with enqueue
        engine: "macos"


        function ttsStateDebug(state) {
            switch (state) {
                case TextToSpeech.Ready:
                    console.log("TTS: Ready")
                    break
                case TextToSpeech.Speaking:
                    console.log("TTS: Speaking")
                    break
                case TextToSpeech.Paused:
                    console.log("TTS: Paused")
                    break
                case TextToSpeech.Error:
                    console.log("TTS: Error")
                    break
            }
        }

        function readText(text : string) {
            if(tts.state === TextToSpeech.Ready) {
                tts.say(text)
            } else {
                tts.enqueue(text)
            }
        }
    }


    Component.onCompleted: {
        if (myController.isFullyInitialized()) {
            leftSide.state = "PostLogin"
            rightSide.state = "PostLogin"
        } else {
            leftSide.state = "PreLogin"
            rightSide.state = "PreLogin"
        }
    }

    Connections {
        target: myController

        function onAppIsPostAuth() {
            leftSide.state = "PostLogin"
            rightSide.state = "PostLogin"
            console.log("App is post auth")
        }

        function onAppIsPreAuth() {
            leftSide.state = "PreLogin"
            rightSide.state = "PreLogin"
            console.log("App is pre auth")
        }

        function onSsoIvalidRefreshToken() {
            invalidAuthTokenDialog.visible = true
        }
    }

    Connections {
        id: intelTableConnections
        target: myController.getMainWindowView().getIntelTableModel()

        function onHostilesJumpsAway(jumps) {
            if (settingsDialog.getTtsEnabled() === false) {
                return
            }
            var jumpRange = settingsDialog.getJumpRange()
            if (jumps === 0) {
                tts.readText("Hostiles in system")
            } else if (jumps <= jumpRange) {
                tts.readText("Hostiles " + jumps + " jumps away")
            }
        }
    }

    Connections {
        target: myController.getMainWindowView()

        function onNewClosestTheraConnection() {
            if (settingsDialog.getTurnurTheraAlert() === true) {
                tts.readText("Closest connection to Thera is " + myController.getMainWindowView().getTheraClosestConnectionJumps() + " jumps away")
            }
        }

        function onNewClosestTurnurConnection() {
            if (settingsDialog.getTurnurTheraAlert() === true) {
                tts.readText("Closest connection to Turnur is " + myController.getMainWindowView().getTurnurClosestConnectionJumps() + " jumps away")
            }
        }
    }

    AboutDialog {
        id: aboutDialog
    }

    SettingsDialog {
        id: settingsDialog
    }

    DeleteCharacterDialog {
        id: deleteCharacterDialog

        onAccepted: {
            myController.deleteCharacter()
        }
    }

    Dialog {
        id: chooseIntelChannelsDialog
        title: "Choose Intel Channels"
        visible: false
        width: 600
        height: 300
        contentItem:
            ListView {
                Connections {
                    target: myController.getMainWindowView().getIntelChannelsListModel()

                    function onListIsEditable() {
                        intelChannelsList.enabled = true
                    }

                    function onListIsNotEditable() {
                        intelChannelsList.enabled = false
                    }
                }
                id: intelChannelsList
                width: 600
                height: 100
                clip: true

                model: myController.getMainWindowView().getIntelChannelsListModel()
                delegate: Row {
                    CheckBox {
                        id: checkBox
                        checked: model.enabled

                        onCheckedChanged: {
                            myController.getMainWindowView().getIntelChannelsListModel().toggleChannel(model.channelName, checkBox.checked)
                        }
                    }
                    spacing: 10
                    Label {
                        text: model.channelName
                        // alight to the center of the checkbox
                        anchors.verticalCenter: checkBox.verticalCenter
                        font.pointSize: 14
                    }
                }
            }
        footer:
            Button {
                text: "Ok"
                onClicked: {
                    chooseIntelChannelsDialog.close()
                }

            }
    }

    GridLayout {
        id: gridLayout
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        columns: 2
        rows: 1
        columnSpacing: 0
        rowSpacing: 0


        // Left side
        Rectangle {
            id: leftSide
            implicitWidth: 256
            Layout.fillHeight: true
            Layout.leftMargin: 0
            Layout.column: 0

            // Background
            Rectangle {
                width: leftSide.width
                height: leftSide.height
                anchors.top: parent.top
                // set background image
                Image {
                    source: "qrc:///images/left-panel-background.png"
                    width: 299
                    fillMode: Image.PreserveAspectCrop
                }
                GridLayout {
                    anchors.top: parent.top
                    columns: 1
                    rows: 10
                    rowSpacing: 5

                    Image {
                        // set image to be 100 px from the top
                        id: characterPortrait
                        // align to top center
                        Layout.alignment: Qt.AlignHCenter
                        source: myController.getMainWindowView().getCharacterPortraitUrl()
                        width: 256
                        height: 256
                        opacity: 0.8
                        fillMode: Image.PreserveAspectCrop
                        layer.enabled: true

                        // overlay text
                        layer.effect: OpacityMask {
                            maskSource: Label {
                                text: "I"
                            }
                        }

                        Connections {
                            target: myController.getMainWindowView()

                            function onCharacterPortraitReady() {
                                characterPortrait.source = myController.getMainWindowView().getCharacterPortraitUrl()
                                console.log("Character portrait ready")
                            }
                        }
                    }
                    // Character name
                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        id: characterName
                        font.pointSize: 24
                        color: "white"
                        text: ""
                    }

                    // Character location
                    Label {
                        id: characterLocation
                        font.pointSize: 15
                        color: "white"
                        text: "Location: "
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Rectangle {
                        height: 10
                    }

                    ClickMenuButton {
                        text: "Delete Character"
                        Layout.alignment: Qt.AlignHCenter

                        // display a confirmation dialog in the center of the application window
                        function buttonClicked() {
                            deleteCharacterDialog.x = (applicationWindow.width - deleteCharacterDialog.width) / 2
                            deleteCharacterDialog.y = (applicationWindow.height - deleteCharacterDialog.height) / 2
                            deleteCharacterDialog.open()
                        }
                    }

                    // 10 pixel gap
                    Rectangle {
                        height: 10
                    }

                    ClickMenuButton {
                        text: "Choose Intel Channels"
                        Layout.alignment: Qt.AlignHCenter

                        // open the intel channels dialog in the center of the application window
                        function buttonClicked() {
                            chooseIntelChannelsDialog.x = (applicationWindow.width - chooseIntelChannelsDialog.width) / 2
                            chooseIntelChannelsDialog.y = (applicationWindow.height - chooseIntelChannelsDialog.height) / 2
                            chooseIntelChannelsDialog.open()
                        }
                    }

                    // 10 pixel gap
                    Rectangle {
                        height: 10
                    }

                    ClickMenuButton {
                        text: "Settings"
                        Layout.alignment: Qt.AlignHCenter

                        function buttonClicked() {
                            // get tts volume and pitch and print it
                            settingsDialog.x = (applicationWindow.width - settingsDialog.width) / 2
                            settingsDialog.y = (applicationWindow.height - settingsDialog.height) / 2
                            settingsDialog.open()
                        }
                    }

                    // 10 pixel gap
                    Rectangle {
                        height: 10
                    }

                    ClickMenuButton {
                        text: "About"
                        Layout.alignment: Qt.AlignHCenter

                        function buttonClicked() {
                            // center in application window
                            aboutDialog.x = (applicationWindow.width - aboutDialog.width) / 2
                            aboutDialog.y = (applicationWindow.height - aboutDialog.height) / 2
                            aboutDialog.open()
                        }
                    }

                    // 10 pixel gap
                    Rectangle {
                        height: 10
                    }

                    // Display closest connection to Thera
                    Label {
                        id: closestTheraConnection
                        font.pointSize: 15
                        color: "white"
                        font.bold: false
                        width: parent.width
                        // center the text
                        Layout.alignment: Qt.AlignHCenter
                        wrapMode: Text.AlignJustify

                        Connections {
                            target: myController.getMainWindowView()

                            function onNewClosestTheraConnection() {
                                closestTheraConnection.text = "Closest connection to Thera is <br> <b>" + myController.getMainWindowView().getTheraClosestConnectionJumps() + " </b>jumps away in <b>" + myController.getMainWindowView().getTheraClosestConnectionName() + "</b>"
                            }
                        }
                    }

                    // Blank row 25 pixel high
                    Rectangle {
                        height: 10
                    }
                    // Display closest connection to Turnur
                    Label {
                        id: closestTurnurConnection
                        font.pointSize: 15
                        color: "white"
                        font.bold: false
                        width: parent.width
                        // center the text
                        Layout.alignment: Qt.AlignHCenter
                        wrapMode: Text.AlignJustify

                        Connections {
                            target: myController.getMainWindowView()

                            function onNewClosestTurnurConnection() {
                                closestTurnurConnection.text = "Closest connection to Turnur is <br> <b>" + myController.getMainWindowView().getTurnurClosestConnectionJumps() + " </b>jumps away in <b>" + myController.getMainWindowView().getTurnurClosestConnectionName() + "</b>"
                            }
                        }
                    }
                }
            }

            states: [
                State {
                    name: "PreLogin"
                    PropertyChanges {
                        leftSide.visible: false

                        // clear the left side
                        characterName.text: ""
                        characterPortrait.source: ""
                        characterLocation.text: ""
                    }
                },
                State {
                    name: "PostLogin"
                    PropertyChanges {
                        leftSide.visible: true
                        characterName.text: myController.getMainWindowView().getCharacterName();
                        characterLocation.text: "Location: " + myController.getMainWindowView().getCharacterLocation()
                    }
                }
            ]
        }

        // Right side
        Rectangle {
            id: rightSide
            Layout.fillHeight: true
            Layout.fillWidth: true

            // set background color to black
            // Pre login state

            // Configure states
            states: [
                State {
                    name: "PreLogin"
                    PropertyChanges {
                        rightSideGridLayoutPreLogin.visible: true
                        horizontalHeader.visible: false
                        intelTable.visible: false
                        noIntelLabel.visible: false
                        loginButton.visible: true
                        rightSide.color: "white"
                        loginProgressBar.visible: false

                        // clear the right side
                        intelTable.model: null
                    }
                },
                State {
                    name: "PostLogin"
                    PropertyChanges {
                        // set the right side
                        rightSideGridLayoutPreLogin.visible: false
                        loginButton.visible: false
                        loginProgressBar.visible: false
                        labelAuthError.visible: false
                        horizontalHeader.visible: true
                        intelTable.visible: true
                        intelTable.model: myController.getMainWindowView().getIntelTableModel()
                        rightSide.color: "#0a0a0a"

                    }
                }
            ]


            GridLayout {
                id: rightSideGridLayoutPreLogin
                anchors.centerIn: parent
                visible: false


                Connections {
                    target: myController

                    // Show why auth failed
                    function onSsoAuthFailed(why) {
                        labelAuthError.text = why;
                        labelAuthError.visible = true;
                        loginButton.visible = true;
                        loginProgressBar.visible = false;
                        tts.say(why)
                    }

                    function onSsoAuthStarted() {
                        labelAuthError.visible = false;
                        labelAuthError.text = "";
                        loginButton.visible = false;
                        loginProgressBar.visible = true;
                    }

                    function onAppIsPreAuth() {
                        loginProgressBar.visible = false;
                    }
                }

                Label {
                    Layout.row: 0
                    id: preLoginLabel
                    font.pointSize: 24
                    color: "black"
                    text: "You need to authenticate with your EVE character first"
                }
                Image {
                    Layout.row: 1
                    Layout.alignment: Qt.AlignHCenter
                    id: loginButton
                    source: "/images/ssologin.png"
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.CursorShape.PointingHandCursor

                        onClicked: {
                            Qt.openUrlExternally(myController.SSOAuthUrl());
                            myController.SSOBeginAuth();
                        }
                    }
                }
                ProgressBar {
                    Layout.row: 1
                    Layout.alignment: Qt.AlignHCenter
                    id: loginProgressBar
                    to: 1
                    height: 10
                    width: 200
                    indeterminate: true
                    visible: false
                    value: 10
                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 10
                        color: "#e6e6e6"
                        radius: 3
                    }

                }
                Label {
                    id: labelAuthError

                    Layout.row: 2
                    Layout.alignment: Qt.AlignHCenter
                    visible: false
                    font.pointSize: 14
                    color: "red"
                    text: ""
                }
            }


            // Post login state
            HorizontalHeaderView {
                id: horizontalHeader
                anchors.left: intelTable.left
                anchors.top: parent.top
                syncView: intelTable
                clip: false
                delegate: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 30
                    color: "#000000"
                    border.width: 0
                    Text {
                        text: display
                        // set to bold
                        font.bold: true
                        color: "#bebebe"
                        anchors.centerIn: parent
                    }
                }

            }

            Label {
                visible: intelTable.rows === 0
                id: noIntelLabel
                anchors.centerIn: parent
                font.pointSize: 30
                color: "#c7c7c7"
                text: "No intel yet"
            }

            TableView {
                id: intelTable
                anchors.top: horizontalHeader.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                clip: true
                width: parent.width
                height: parent.height

                columnWidthProvider: function (column) {
                    if (column === 0)
                        return 200
                    else if (column === 1)
                        return 100
                    else if (column === 2)
                        return 200
                    else if (columns === 3)
                        return 100
                    else if (column === 4)
                        return 100
                    else if (column === 5)
                        return rightSide.width - 500;
                }

                resizableColumns: false
                resizableRows: false

                DelegateChooser {
                    id: tableViewDelegateChooser
                    DelegateChoice {
                        column: 5
                        Rectangle {
                            implicitHeight: 30
                            implicitWidth: rightSide.width - 600;
                            color: "black"
                            Label {
                                text: display
                                color: "#c7c7c7"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }

                    DelegateChoice {
                        Rectangle {
                            implicitHeight: 30
                            color: "black"
                            Label {
                                text: display
                                color: "#c7c7c7"
                                // horizontal and vertical
                                anchors.centerIn: parent
                            }
                        }
                    }
                }

                delegate: tableViewDelegateChooser
            }
        }
    }

    Popup {
        id: invalidAuthTokenDialog
        modal: true
        anchors.centerIn: parent
        width: 400
        height: 200
        visible: false
        contentItem: Rectangle {
            implicitWidth: 600
            implicitHeight: 200
            color: "#1c0f08"
            ColumnLayout {
                anchors.fill: parent
                Label {
                    text: "Refresh token is invalid.<br> Please re-authenticate."
                    color: "white"
                    font.pointSize: 20
                    Layout.alignment: Qt.AlignHCenter
                }
                Button {
                    text: "OK"
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        invalidAuthTokenDialog.close()
                        myController.deleteCharacter()
                    }
                }
            }
        }
    }
}
