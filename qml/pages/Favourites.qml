import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import Sailfish.Silica 1.0
import "./utils.js" as Utils
import "./db.js" as Db

Page {
    objectName: "favourites"

    property string city: "Paris"
    property string coverLabel: "Favourites"
    property int nbRefreshingStations: 0
    property string dbVersion: "0.1"
    // passed by the InteractiveMap
    property int favouriteToAdd: 0


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
            title: "Favourites - " + city
        }

        PullDownMenu {
            id: topMenu
            MenuItem {
                text: "Refresh all"
                onClicked: {
                    topMenu.busy = favouritesModel.count > 0;
                    refreshAll();
                }
            }
        }

        model: ListModel {
            id: favouritesModel
        }

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
                        text: " : " + model.available_bikes
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
                        text: " : " + model.available_bike_stands
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeMedium
                        font.bold: true
                    }
                    Item {
                        width: Theme.paddingLarge * 2
                        height: 1
                    }
                    Label {
                        id: numberOfParking
                        text: "Updated: " + Utils.makeLastUpdateDateHumanReadable(model.last_update)
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
                            if (dataProvider.isGetSingleStationDataSupported(city)) {
                                topMenu.busy = true;
                                nbRefreshingStations++;
                                dataProvider.getStationDetails(city, model.number);
                            }
                            else {
                                errorMsg.text = "Individual refresh not available for this city";
                                errorMsg.visible = true;
                            }
                        }
                    }
                    MenuItem {
                        text: "Move Down"
                        onClicked: moveDown()
                    }
                    MenuItem {
                        text: "Move Up"
                        onClicked: moveUp()
                    }
                    MenuItem {
                        text: "Delete"
                        onClicked: remove()
                    }
                }
            }

            function remove() {
                remorseAction("Deleting", function() {
                    Db.removeFavourite(city, favouritesModel.get(index).number);
                    favouritesModel.remove(index);
                })
            }
            function moveDown() {
                if (index > favouritesModel.count - 2) {
                    return;
                }
                var i = index;
                favouritesModel.move(index, index + 1, 1);
                Db.moveDown(city, favouritesModel.get(index).number);
            }
            function moveUp() {
                if (index < 1) {
                    return;
                }
                var i = index;
                favouritesModel.move(index - 1, index, 1);
                Db.moveUp(city, favouritesModel.get(index).number)
            }
        }

        Component.onCompleted: {
            var favs = Db.getFavourites(city);
            favs.forEach(function(fav) {
                favouritesModel.append(fav);
            });
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

                favouritesModel.append({ number: stationNumber,
                                         name: 'Station ' + stationNumber,
                                         available_bikes: 0,
                                         available_bike_stands: 0,
                                         last_update: 0});
                text = "";
                listView.focus = true;
                Db.addFavourite(city, stationNumber);
            }

            Component.onCompleted: {
                if (favouriteToAdd !== 0) {
                    text = favouriteToAdd;
                    focus = true;
                }  
            }
        }
    }

    Connections {
        target: dataProvider
        onGotStationDetails: {
            var res = stationDetails;
            console.log(res);
            try {
                var station_details = JSON.parse(res);
                for (var i = 0; i < favouritesModel.count; ++i) {
                    if (favouritesModel.get(i).number === station_details.number) {
                        if (favouritesModel.get(i).name !== station_details.name) {
                            updateStationNameInDB(station_details.number, station_details.name);
                        }
                        station_details.name = removeHTML(station_details.name);
                        favouritesModel.remove(i);
                        favouritesModel.insert(i, station_details);
                    }
                }
                errorMsg.visible = false;
            }
            catch (e) {
                errorMsg.text = res;
                errorMsg.visible = true;
                topMenu.busy = false;
            }

            nbRefreshingStations--;
            if (nbRefreshingStations == 0) {
                topMenu.busy = false;
                updateCoverLabel();
            }
        }

        onGotAllStationsDetails: {
            var res = allStationsDetails;
            try {
                var stationsDetails = JSON.parse(res);
                stationsDetails.forEach(function(station) {
                    for (var i = 0; i < favouritesModel.count; ++i) {
                        if (favouritesModel.get(i).number === station.number) {
                            if (favouritesModel.get(i).name !== station.name) {
                                updateStationNameInDB(station.number, station.name);
                            }
                            station.name = removeHTML(station.name);
                            favouritesModel.remove(i);
                            favouritesModel.insert(i, station);
                        }
                    }
                    errorMsg.visible = false;
                });
                topMenu.busy = false;
                updateCoverLabel();
            }
            catch(e) {
                errorMsg.text = e.message;
                errorMsg.visible = true;
                topMenu.busy = false;
            }
            nbRefreshingStations--;
        }
    }

    function refreshAll() {
        if (!dataProvider.isGetSingleStationDataSupported(city)) {
            nbRefreshingStations++;
            dataProvider.downloadAllStationsDetails(city);
        }
        else {
            for (var i = 0; i < favouritesModel.count; ++i) {
                nbRefreshingStations++;
                dataProvider.getStationDetails(city, favouritesModel.get(i).number);
            }
        }
    }

    function updateCoverLabel() {
        // Generating cover label: 5 first stations.
        coverLabel = "";
        for (var i = 0; i < 5 && i < favouritesModel.count; ++i) {
            var name;
            if (favouritesModel.get(i).name.indexOf(" - ") !== -1) {
                name = favouritesModel.get(i).name.split(" - ");
            }
            else {
                name = favouritesModel.get(i).name.split("-");
            }

            if (name.length > 1) {
                name = name[1].substr(0, 7);
            }
            else {
                name = name[0].substr(0, 7);
            }
            name = removeHTML(name);

            coverLabel += "[" + name + "] B:" + favouritesModel.get(i).available_bikes +
                    " P:" + favouritesModel.get(i).available_bike_stands + "\n";
        }
    }

    function updateStationNameInDB(stationNumber, stationName) {
        console.log("Updating name of station " + stationNumber);
        Db.updateStationName(city, stationNumber, stationName);
    }

    function removeHTML(name) {
        // The data of those cities are encoded :/
        if (city === "Nice" || city === "Calais" || city === "Vannes") {
            return decodeURIComponent(name).replace(/\+/g, ' ');
        }
        return name;
    }
}
