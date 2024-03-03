import QtQuick
import QtQuick.Controls

// Create an about dialog
Dialog {
    modal: true
    // print program name in title with major and minor version
    title: "EVEIntelMonitor " + APP_MAJOR_VERSION + "." + APP_MINOR_VERSION
    standardButtons: Dialog.Ok
    Column {
        spacing: 10
        //set background color

        Text {
            text: "This application uses eve online chat logs to notify of potential threats to ships."
        }
        Text {
            // state that license is GPLv3
            text: "This application is licensed under the GPLv3. See the LICENSE file for details."
        }
        // hORIZONTAL LINE
        Rectangle {
            width: parent.width
            height: 1
            color: "black"
        }
        // Qt Licensing
        Text {
            text: "This application uses the Qt framework, which is licensed under the LGPLv3."
        }

        // CCP / Eve Online Licensing
        Text {
            text: "Eve Online is a registered trademark of CCP hf."
        }
    }
}