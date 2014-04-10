import QtQuick 2.0
import Sailfish.Silica 1.0


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
                text: "Map provider: " + mapPlugin
                onClicked: {
                    mapPlugin = mapPlugin == "nokia" ? "osm" : "nokia";
                    configManager.saveSetting("mapProvider", mapPlugin);
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
                    pageParams.mapPlugin = mapPlugin;
                }
                pageStack.push(Qt.resolvedUrl(fileName + ".qml"), pageParams);
            }
        }
    }
}





