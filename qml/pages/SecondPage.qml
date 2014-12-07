import QtQuick 2.0
import Sailfish.Silica 1.0

/* Themes values on Jolla:
 * itemSizeExtraSmall=70; itemSizeSmall=80; itemSizeMedium=100
 * iconSizeSmall=32; iconSizeMedium=64; iconSizeLarge=96
 */

Page {
    id: secondPage

    property string city: "Paris"
    property string mapPlugin: "nokia"

    Component.onCompleted: {
        mapPlugin = configManager.getSetting("mapProvider") || "nokia";
        configManager.saveSetting("city", city);
    }

    SilicaListView {
        id: listView
        anchors.fill: parent

        PullDownMenu {
            id: topMenu
            MenuItem {
                text: qsTr("Settings")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("Settings.qml"));
                }
            }
        }

        model: ListModel {
            id: favouritesModel

            ListElement {
                name: "Interactive Map"
                fileName: "InteractiveMap"
            }
            ListElement {
                name: "Favourites"
                fileName: "Favourites"
            }
        }

        header: PageHeader {
            title: city
        }
        delegate: BackgroundItem {
            id: delegate

            Label {
                x: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                text: name
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            onClicked: {
                var pageParams = { city : city };
                if (fileName == "InteractiveMap") {
                    pageParams.mapPlugin = configManager.getSetting("mapProvider") || "nokia";
                    pageParams.displayAllStatus = configManager.getSetting("displayAllStatus") === "true";
                }
                pageStack.push(Qt.resolvedUrl(fileName + ".qml"), pageParams);
            }
        }
    }

    Label {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: Theme.paddingMedium

        visible: city === "London"
        font.pixelSize: Theme.fontSizeExtraSmall
        color: Theme.secondaryColor
        text: "Powered by TfL Open Data"
    }
}
