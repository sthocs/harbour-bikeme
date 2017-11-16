import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    SilicaFlickable {
        anchors.fill: parent
        anchors.bottomMargin: Theme.paddingMedium
        contentHeight: mainColumn.height

    Column {
        id: mainColumn
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: Theme.paddingMedium
            rightMargin: Theme.paddingMedium
            bottomMargin: Theme.paddingMedium
        }

        PageHeader {
            title: qsTr("Settings")
        }

        ComboBox {
            label: qsTr("Map provider")

            menu: ContextMenu {
                MenuItem {
                    text: "here"
                    onClicked: configManager.saveSetting("mapProvider", text);
                }
                MenuItem {
                    text: "osm"
                    onClicked: configManager.saveSetting("mapProvider", text);
                }
            }

            Component.onCompleted: {
                var mapProvider = configManager.getSetting("mapProvider") || "here";
                currentIndex = mapProvider === "osm" ? 1 : 0;
            }
        }

        ComboBox {
            id: startPage
            label: qsTr("Start page")

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Default")
                    onClicked: configManager.saveSetting("startPage", 0);
                }
                MenuItem {
                    text: qsTr("Map")
                    onClicked: configManager.saveSetting("startPage", 1);
                }
                MenuItem {
                    text: qsTr("Favourites")
                    onClicked: configManager.saveSetting("startPage", 2);
                }
            }

            Component.onCompleted: {
                currentIndex = configManager.getSetting("startPage") || 0;
            }
        }

        TextSwitch {
            id: activationSwitch
            text: qsTr("Display all stations status")
            description: qsTr("All station status will be downloaded and displayed on the map. Consume more data.")
            onCheckedChanged: {
                configManager.saveSetting("displayAllStatus", checked)
            }

            Component.onCompleted: {
                checked = configManager.getSetting("displayAllStatus") !== "false";
            }
        }
        TextSwitch {
            text: qsTr("Open map on my position")
            description: qsTr("When opening the map, the GPS will be already enabled and the map will center on your position.")
            onCheckedChanged: {
                configManager.saveSetting("autoEnableGPS", checked)
            }

            Component.onCompleted: {
                checked = configManager.getSetting("autoEnableGPS") !== "false";
            }
        }
        Slider {
            label: qsTr("Map initial zoom level")
            width: parent.width
            minimumValue: 12
            maximumValue: 18
            value: !isNaN(configManager.getSetting("zoomLevel")) ?
                       parseInt(configManager.getSetting("zoomLevel")) : 15
            valueText: value
            stepSize: 1
            onValueChanged: {
                configManager.saveSetting("zoomLevel", value);
            }
         }
        Item {
            width: parent.width
            height: Theme.paddingLarge * 5
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Clear cache")
            onClicked: remorse.execute(qsTr("Clearing cache"), function() { configManager.removeCacheFiles() } );
        }
        Text {
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            width: parent.width
            wrapMode: Text.WordWrap
            text: qsTr("BikeMe keeps in cache stations location to save mobile data. " +
                       "If stations don't display, you can try to clear the cache to remove potentially corrupted files.")
        }
    }
    VerticalScrollDecorator {}
    }
    RemorsePopup { id: remorse }
}
