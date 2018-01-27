import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import Sailfish.Silica 1.0
import harbour.bikeme 1.0
import "./utils.js" as Utils
import "./db.js" as Db

Page {
    objectName: "favourites"

    property string coverLabel: "Favourites"
    property int nbRefreshingStations: 0

    property City city

    StationsModel {
        id: stations

        onStationsLoaded: {
            if (!withDetails) {
                favouritesModel.refreshAll();
            }
            window.cover.favouritesModel = favouritesModel
        }
        onError: {
            errorMsgLabel.text = errorMsg;
            errorMsgLabel.visible = true;
        }
    }

    StationsFavouritesProxy {
        id: favouritesModel
        sourceModel: stations
    }

    Label {
        id: errorMsgLabel
        visible: false
        font.pixelSize: Theme.fontSizeExtraSmall
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        anchors.bottomMargin: addFavouriteTextfield.height

        header: PageHeader {
            id: header
            title: qsTr("Favourites - %1").arg(city.name)
        }

        ViewPlaceholder {
            id: placeholder
            enabled: listView.count == 0
            text: qsTr("No favourites yet")
            hintText: qsTr("You can add your favourites from the map or using the field below if you know the station number.")
        }

        PullDownMenu {
            id: topMenu
            MenuItem {
                text: qsTr("Update all")
                onClicked: {
                    //topMenu.busy =  favouritesModel.count > 0;
                    errorMsgLabel.visible = false;
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
                    text: decodeURIComponent(model.name.toLowerCase())
                    font.capitalization: Font.Capitalize
                }
                Row {
                    x: Theme.paddingLarge
                    Image {
                        source: "../icons/icon-bikeme.svg"
                        sourceSize.height: Theme.fontSizeSmall
                        sourceSize.width: Theme.fontSizeSmall
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label {
                        text: " : " + (opened && available_bikes != -1 ?
                                           available_bikes : 0)
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeMedium
                        font.bold: true
                    }
                    Item {
                        width: Theme.paddingLarge
                        height: 1
                    }
                    Image {
                        visible: city.hasElectricBikes
                        source: "../icons/icon-bikeme-elec.svg"
                        sourceSize.height: Theme.fontSizeSmall
                        sourceSize.width: Theme.fontSizeSmall
                        anchors.leftMargin: Theme.paddingSmall
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label {
                        visible: city.hasElectricBikes
                        text: " : " + (opened && available_electric_bikes != -1 ?
                                           available_electric_bikes : 0)
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeMedium
                        font.bold: true
                    }
                    Item {
                        visible: city.hasElectricBikes
                        width: Theme.paddingLarge
                        height: 1
                    }
                    Image {
                        source: "../icons/parking.svg"
                        sourceSize.height: Theme.fontSizeSmall
                        sourceSize.width: Theme.fontSizeSmall
                        anchors.leftMargin: Theme.paddingSmall
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label {
                        text: " : " + (opened && available_bike_stands != -1 ?
                                           available_bike_stands : 0)
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeMedium
                        font.bold: true
                    }
                    Item {
                        width: Theme.paddingLarge * 2
                        height: 1
                    }
                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        visible: Utils.isValidDate(last_update)
                        text: opened ? qsTr("Updated: %1").arg(Utils.makeLastUpdateDateHumanReadable(model.last_update)) :
                                       qsTr("Closed")
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }
                }
            }

            Component {
                id: contextMenuComponent
                ContextMenu {
                    MenuItem {
                        text: qsTr("Update")
                        onClicked: {
                            errorMsgLabel.visible = false;
                            if (!favouritesModel.refreshStationInfo(index)) {
                                errorMsgLabel.text = qsTr("Individual update not available for this city");
                                errorMsgLabel.visible = true;
                            }
                        }
                    }
                    MenuItem {
                        text: qsTr("Move Up")
                        onClicked: {
                            if (index < 1) {
                                return;
                            }
                            Db.moveUp(city.identifier, number);
                            favouritesModel.move(index - 1, index, 1);
                        }
                    }
                    MenuItem {
                        text: qsTr("Move Down")
                        onClicked: {
                            if (index > favouritesModel.count - 2) {
                                return;
                            }
                            Db.moveDown(city.identifier, number);
                            favouritesModel.move(index, index + 1, 1);
                        }
                    }
                    MenuItem {
                        text: qsTr("Delete")
                        onClicked: remove()
                    }
                }
            }

            function remove() {
                remorseAction(qsTr("Deleting"), function() {
                    Db.removeFavourite(city.identifier, number);
                    favouritesModel.removeAt(index);
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
            stations.city = city;
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
            placeholderText: qsTr("Add a new favourite")
            label: qsTr("Adding a favourite")
            horizontalAlignment: Text.AlignLeft
            EnterKey.onClicked: {
                var stationNumber = parseInt(text);
                if (isNaN(stationNumber)) {
                    return;
                }

                errorMsgLabel.visible = false;
                if (stations.exists(stationNumber)) {
                    if (favouritesModel.add(stationNumber)) {
                        Db.addFavourite(city.identifier, stationNumber);
                    }
                }
                else {
                    errorMsgLabel.text = qsTr("Station %1 does not exist").arg(text);
                    errorMsgLabel.visible = true;
                }

                text = "";
                listView.focus = true;
            }
        }
    }
}
