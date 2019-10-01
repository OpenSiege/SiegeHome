import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.0
import QtQuick.Layouts 1.11

import SiegeHome.InstallationTab 1.0

ApplicationWindow {

    id: window

    width: 628
    height: 195
    visible: true
    title: "Siege Home"

    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
    }

    TabBar {
        id: bar
        width: parent.width
        contentHeight: 20

        TabButton {
            text: qsTr("Installation")
        }

        TabButton {
            text: qsTr("DS1 Reg Keys")
        }

        TabButton {
            text: qsTr("DS2 Reg Keys")
        }

        TabButton {
            text: qsTr("Tools")
        }
    }

    StackLayout {
        width: parent.width
        currentIndex: bar.currentIndex

        anchors.top: bar.bottom
        anchors.left: bar.left;
        anchors.right: bar.right;

        Item {
            id: installationTab

            GridLayout {

                id: grid
                columns: 2
                anchors.fill: parent

                TextField {
                    readOnly: true
                    objectName: "textFieldDS1InstallPath"
                    placeholderText: qsTr("Dungeon Siege 1 Install Path")
                    onTextChanged: installTabBackend.ds1InstallPath = text
                }
                
                CheckBox {
                    text: qsTr("Override Default Values")
                    onClicked: installTabBackend.toggleOverride();

                    Layout.column: 1
                    Layout.row: 0
                }

                TextField {
                    readOnly: true
                    objectName: "textFieldDS2InstallPath"
                    placeholderText: qsTr("Dungeon Siege 2 Install Path")
                    onTextChanged: installTabBackend.ds2InstallPath = text

                    Layout.column: 0
                    Layout.row: 1
                }

                Button {
                    text: qsTr("Resolve Paths")
                    onClicked: installTabBackend.reloadPaths()

                    Layout.column: 0
                    Layout.row: 2
                }
            }
        }
    }
}