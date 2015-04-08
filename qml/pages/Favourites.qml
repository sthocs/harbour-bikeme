import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import Sailfish.Silica 1.0

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
                Label {
                    id: station_infos
                    x: Theme.paddingLarge
                    color: Theme.secondaryColor
                    text: "Bikes: " + model.available_bikes + " - Parking: " + model.available_bike_stands
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
                    var db = LocalStorage.openDatabaseSync("BikeMe", dbVersion, "BikeMe DB", 1000000);

                    db.transaction(
                                function(tx) {
                                    tx.executeSql('DELETE FROM favourites WHERE city = "' + city +
                                                  '" AND station_nb = ' + favouritesModel.get(index).number );
                                });
                    var i = index;
                    favouritesModel.remove(index);
                    for (i; i < favouritesModel.count; ++i) {
                        db.transaction(
                                    function(tx) {
                                        tx.executeSql('UPDATE favourites SET rank="' + i + '" WHERE city="' + city +
                                                      '" AND station_nb=' + favouritesModel.get(i).number );
                                    });
                    }
                })
            }
            function moveDown() {
                if (index > favouritesModel.count - 2) {
                    return;
                }
                var i = index;
                favouritesModel.move(i, i + 1, 1);
                var db = LocalStorage.openDatabaseSync("BikeMe", dbVersion, "BikeMe DB", 1000000);
                db.transaction(
                            function(tx) {
                                tx.executeSql('UPDATE favourites SET rank="' + i + '" WHERE city="' + city +
                                              '" AND station_nb=' + favouritesModel.get(i).number );
                                tx.executeSql('UPDATE favourites SET rank="' + (i + 1) + '" WHERE city="' + city +
                                              '" AND station_nb=' + favouritesModel.get(i + 1).number );
                            });
            }
            function moveUp() {
                if (index < 1) {
                    return;
                }
                var i = index;
                favouritesModel.move(i - 1, i, 1);
                var db = LocalStorage.openDatabaseSync("BikeMe", dbVersion, "BikeMe DB", 1000000);
                db.transaction(
                            function(tx) {
                                tx.executeSql('UPDATE favourites SET rank="' + i + '" WHERE city="' + city +
                                              '" AND station_nb=' + favouritesModel.get(i).number );
                                tx.executeSql('UPDATE favourites SET rank="' + (i - 1) + '" WHERE city="' + city +
                                              '" AND station_nb=' + favouritesModel.get(i - 1).number );
                            });
            }
        }

        Component.onCompleted: {
            var db = LocalStorage.openDatabaseSync("BikeMe", dbVersion, "BikeMe DB", 1000000);

            db.transaction(function(tx) {
                // Create the database if it doesn't already exist
                tx.executeSql('CREATE TABLE IF NOT EXISTS favourites(city TEXT, station_nb INT, name TEXT, rank INT)');

                tx.executeSql('DELETE FROM favourites WHERE station_nb IS NULL');
                var rs = tx.executeSql('SELECT * FROM favourites WHERE city = "' + city + '" ORDER BY rank');

                for (var i = 0; i < rs.rows.length; ++i) {
                    var name = rs.rows.item(i).name;
                    favouritesModel.append({ number: rs.rows.item(i).station_nb,
                                             name: name ? removeHTML(name) : 'Station ' + rs.rows.item(i).station_nb,
                                             available_bikes: 0,
                                             available_bike_stands: 0});
                }
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
                                         available_bike_stands: 0});
                text = "";
                listView.focus = true;
                var db = LocalStorage.openDatabaseSync("BikeMe", dbVersion, "BikeMe DB", 1000000);

                db.transaction(
                            function(tx) {
                                tx.executeSql('INSERT INTO favourites (city, station_nb, rank) VALUES(?, ?, ?)',
                                              [ city, stationNumber, favouritesModel.count - 1 ]);
                            });
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
        var db = LocalStorage.openDatabaseSync("BikeMe", dbVersion, "BikeMe DB", 1000000);

        db.transaction(
                    function(tx) {
                        tx.executeSql('UPDATE favourites SET name="' + stationName + '" WHERE city="' + city +
                                      '" AND station_nb=' + stationNumber);
                    });
    }

    function removeHTML(name) {
        // The data of those cities are encoded :/
        if (city === "Nice" || city === "Calais" || city === "Vannes") {
            return decodeURIComponent(name).replace(/\+/g, ' ');
        }
        return name;
    }
}
