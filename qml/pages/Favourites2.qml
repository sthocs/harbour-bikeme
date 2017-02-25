import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import Sailfish.Silica 1.0
import com.jolla.harbour.bikeme 1.0
import "./utils.js" as Utils
import "./db.js" as Db

Page {
    objectName: "favourites"

    property string coverLabel: "Favourites"
    property int nbRefreshingStations: 0

    property City city

    StationsModel {
        id: stations
        providerName: city.providerName;
        cityName: city.name
        allStationsDetailsUrl: city.allStationsDetailsUrl
        stationDetailsUrlTemplate: city.singleStationDetailsUrlTemplate

        onStationsLoaded: {
            if (!withDetails) {
                favouritesModel.refreshAll();
            }
        }
    }

    StationsFavouritesProxy {
        id: favouritesModel
        sourceModel: stations
    }

    Label {
        id: errorMsg
        visible: false
        font.pixelSize: Theme.fontSizeExtraSmall
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        anchors.bottomMargin: addFavouriteTextfield.height

        header: PageHeader {
            id: header
            title: "Favourites - " + city.name
        }

        PullDownMenu {
            id: topMenu
            MenuItem {
                text: "Refresh all"
                onClicked: {
                    //topMenu.busy =  favouritesModel.count > 0;
                    favouritesModel.refreshAll();
                }
            }
        }

        model: favouritesModel

        VerticalScrollDecorator {}

        delegate: ListItem {
            menu: contextMenuComponent

            Column {
                Label {
                    id: station_name
                    x: Theme.paddingLarge
                    text: model.name.toLowerCase()
                    font.capitalization: Font.Capitalize
                }
                Row {
                    x: Theme.paddingLarge
                    Image {
                        source: "../icons/bikeme.png"
                        sourceSize.height: Theme.fontSizeSmall
                        sourceSize.width: Theme.fontSizeSmall
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label {
                        text: " : " + (opened ? available_bikes : 0)
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeMedium
                        font.bold: true
                    }
                    Item {
                        width: Theme.paddingLarge
                        height: 1
                    }
                    Image {
                        source: "../icons/parking.png"
                        sourceSize.height: Theme.fontSizeSmall
                        sourceSize.width: Theme.fontSizeSmall
                        anchors.leftMargin: Theme.paddingSmall
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label {
                        text: " : " + (opened ? available_bike_stands : 0)
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeMedium
                        font.bold: true
                    }
                    Item {
                        width: Theme.paddingLarge * 2
                        height: 1
                    }
                    Label {
                        text: opened ? "Updated: " + Utils.makeLastUpdateDateHumanReadable2(model.last_update) :
                                       "Closed"
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }
            }

            Component {
                id: contextMenuComponent
                ContextMenu {
                    MenuItem {
                        text: "Refresh"
                        onClicked: {
                            if (!favouritesModel.refreshStationInfo(index)) {
                                errorMsg.text = "Individual refresh not available for this city";
                                errorMsg.visible = true;
                            }
                        }
                    }
                    MenuItem {
                        text: "Move Up"
                        onClicked: {
                            if (index < 1) {
                                return;
                            }
                            Db.moveUp(city.identifier, number);
                            favouritesModel.move(index - 1, index, 1);
                        }
                    }
                    MenuItem {
                        text: "Move Down"
                        onClicked: {
                            if (index > favouritesModel.count - 2) {
                                return;
                            }
                            Db.moveDown(city.identifier, number);
                            favouritesModel.move(index, index + 1, 1);
                        }
                    }
                    MenuItem {
                        text: "Delete"
                        onClicked: remove()
                    }
                }
            }

            function remove() {
                remorseAction("Deleting", function() {
                    Db.removeFavourite(city.identifier, number);
                    favouritesModel.remove(index);
                });
            }
        }

        Component.onCompleted: {
            var favs = Db.getFavourites(city.identifier);
            var numbers = [];
            favs.forEach(function(fav) {
                numbers.push(fav.number);
            });
            console.log('setting favs ' + JSON.stringify(numbers));
            favouritesModel.setFavourites(numbers);
            stations.loadStationsList();
        }
    }

    Item {
        id: addFavouriteTextfield
        anchors.bottom: parent.bottom
        height: Theme.itemSizeMedium
        width: parent.width

        Rectangle {
            anchors.fill: parent
            color: Theme.rgba(Theme.highlightDimmerColor, 0.9)
        }
        TextField {
            width: parent.width
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            placeholderText: "Add a new favourite"
            label: "Adding a favourite"         
            horizontalAlignment: Text.AlignLeft
            EnterKey.onClicked: {
                var stationNumber = parseInt(text);
                if (isNaN(stationNumber)) {
                    return;
                }

                if (favouritesModel.add(stationNumber)) {
                    Db.addFavourite(city.identifier, stationNumber);
                }
                text = "";
                listView.focus = true;
            }
        }
    }

    function updateStationNameInDB(stationNumber, stationName) {
        console.log("Updating name of station " + stationNumber);
        Db.updateStationName(city, stationNumber, stationName);
    }
}
