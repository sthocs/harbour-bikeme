import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    Column {
        anchors.fill: parent

        PageHeader {
            title: qsTr("Settings")
        }

        ComboBox {
            label: qsTr("Map provider")

            menu: ContextMenu {
                MenuItem {
                    text: "nokia"
                    onClicked: configManager.saveSetting("mapProvider", text);
                }
                MenuItem {
                    text: "osm"
                    onClicked: configManager.saveSetting("mapProvider", text);
                }
            }

            Component.onCompleted: {
                var mapProvider = configManager.getSetting("mapProvider") || "nokia";
                currentIndex = mapProvider === "osm" ? 1 : 0;
            }
        }

        TextSwitch {
            id: activationSwitch
            text: "Display all stations status"
            description: "All station status will be downloaded and displayed on the map. Consume more data."
            onCheckedChanged: {
                configManager.saveSetting("displayAllStatus", checked)
            }

            Component.onCompleted: {
                checked = configManager.getSetting("displayAllStatus") === "true";
            }
        }
    }
    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: Theme.paddingMedium
        anchors.rightMargin: Theme.paddingMedium
        anchors.bottomMargin: Theme.paddingMedium

        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Clear cache")
            onClicked: remorse.execute("Clearing cache", function() { dataProvider.removeCacheFiles() } );
        }
        Text {
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            width: parent.width
            wrapMode: Text.WordWrap
            text: qsTr("BikeMe keeps in cache stations location to save mobile data." +
                       "If stations don't display, you can try to clear the cache to remove potentially corrupted files.")
        }
    }
    RemorsePopup { id: remorse }
}
