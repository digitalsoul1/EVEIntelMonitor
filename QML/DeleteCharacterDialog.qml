import QtQuick
import QtQuick.Controls

// create dialog with the following message, do you really wish to delete the character?
Dialog {
    id: deleteDialog
    standardButtons: Dialog.Ok | Dialog.Cancel
    // position the dialog in the center of the screen
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    title: "Delete Character"

    contentItem: Text {
        text: "Do you really wish to delete the character?"
    }
}