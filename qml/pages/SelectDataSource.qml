import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
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
            title: qsTr("Data source")
        }

        ComboBox {
            label: qsTr("Data source")

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Official data")
                    onClicked: {
                        configManager.saveSetting("dataSource", 0);
                        infoMsg.visible = true;
                    }
                }
                MenuItem {
                    text: "CityBikes"
                    onClicked: {
                        configManager.saveSetting("dataSource", 1);
                        infoMsg.visible = true;
                    }
                }
            }

            Component.onCompleted: {
                currentIndex = configManager.getSetting("dataSource") || 0;
            }
        }
        Text {
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            linkColor: Theme.highlightColor
            width: parent.width
            wrapMode: Text.WordWrap
            text: qsTr("BikeMe uses data directly from bikes providers as much as possible, \
but a lot of them don't expose their data, so it's a tedious work to support those cities.<br>\
A project is doing that work very well: <a href=\"https://citybik.es\">CityBikes</a>. \
So if your city is not available in the \"Official data\", \
check out if it is in the data from CityBikes!")
            onLinkActivated: { Qt.openUrlExternally(link) }
        }
        Item {
            width: parent.width
            height: Theme.paddingLarge
        }
        Text {
            id: infoMsg
            visible: false
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.highlightColor
            width: parent.width
            wrapMode: Text.WordWrap
            text: qsTr("Cities list must be updated after changing data source")
        }
    }
}
