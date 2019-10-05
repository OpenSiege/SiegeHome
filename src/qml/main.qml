import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.0
import QtQuick.Layouts 1.11

import SiegeHome.InstallationTab 1.0

ApplicationWindow {

    id: window

    width: 628
    height: 195

    maximumWidth: width
    maximumHeight: height

    minimumWidth: width
    minimumHeight: height

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
                columns: 3
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5

                Label {
                    text: "DS1 Install Path:"
                }

                TextField {
                    readOnly: true
                    objectName: "textFieldDS1InstallPath"
                    placeholderText: qsTr("Dungeon Siege 1 Install Path")
                    onTextChanged: installTabBackend.ds1InstallPath = text

                    Layout.preferredWidth: 350
                }
                
                CheckBox {
                    text: qsTr("Override")
                    onClicked: installTabBackend.toggleOverride();
                }

                Label {
                    text: "DS 2 Install Path:"
                }

                TextField {
                    readOnly: true
                    objectName: "textFieldDS2InstallPath"
                    placeholderText: qsTr("Dungeon Siege 2 Install Path")
                    onTextChanged: installTabBackend.ds2InstallPath = text

                    Layout.preferredWidth: 350
                }

                Button {
                    text: qsTr("Resolve Paths")
                    onClicked: installTabBackend.reloadPaths()

                    Layout.column: 1
                    Layout.row: 3

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                Button {
                    text: qsTr("Write Registry Keys")
                    onClicked: installTabBackend.writeRegistryKeys()

                    Layout.column: 1
                    Layout.row: 4

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }
            }
        }

        Item {
            id: ds1RegistryKeysTab

            ColumnLayout {

                id: ds1RegistryGrid
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5

                Label {
                    objectName: "ds1RegistryExePath"
                    text: qsTr("ExePath: <Undefined>");
                }
            }
        }

        Item {
            id: ds2RegistryKeysTab

            ColumnLayout {

                id: ds2RegistryGrid
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.bottomMargin: 5

                Label {
                    objectName: "ds2RegistryAppPath"
                    text: qsTr("AppPath: <Undefined>");
                }
            }
        }

        Item {
            id: toolsTab

            ColumnLayout {

                id: toolsGrid
                anchors.fill: parent

                Button {
                    text: qsTr("Install DSTK for DS1")

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    onClicked: installTabBackend.downloadDS1TK()
                }

                Button {
                    text: qsTr("Install DSTK for DS2")

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    onClicked: installTabBackend.downloadDS2TK()
                }
            }
        }
    }
}