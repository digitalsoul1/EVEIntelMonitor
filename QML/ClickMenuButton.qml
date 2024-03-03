import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic

Button {
    implicitWidth: 200
    implicitHeight: 30
    opacity: 0.8

    background: Rectangle {
        color: "#807948"
        border.color: "#4e492b"
    }

    contentItem: Text {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.centerIn: parent
        font.pointSize: 14
        color: "#e6e6e6"
        text: parent.text
        font.bold: false
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: parent.opacity = 1
        onExited: parent.opacity = 0.8
        onClicked: parent.buttonClicked()
        cursorShape: Qt.PointingHandCursor
    }

    function buttonClicked() {
        console.log(parent.text + " button clicked")
    }
}