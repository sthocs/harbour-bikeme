.pragma library
.import QtQuick.LocalStorage 2.0 as Sql

var DB_VERSION = "0.1";

var db = Sql.LocalStorage.openDatabaseSync("BikeMe", DB_VERSION, "BikeMe DB", 1000000);

db.transaction(function(tx) {
    // Create the database if it doesn't already exist
    tx.executeSql('CREATE TABLE IF NOT EXISTS favourites(city TEXT, station_nb INT, name TEXT, rank INT)');

    tx.executeSql('DELETE FROM favourites WHERE station_nb IS NULL');
});

/**
 * Get favourites in DB and return an array of objects ready to be put in the Favourites ListView
 */
function getFavourites(city) {
    var res = [];
    db.transaction(function(tx) {
        var favs = tx.executeSql('SELECT * FROM favourites WHERE city = "' + city + '" ORDER BY rank');
        for (var i = 0; i < favs.rows.length; ++i) {
            var name = favs.rows.item(i).name;
            res.push({ number: favs.rows.item(i).station_nb,
                       name: name ? removeHTML(name, city) : 'Station ' + favs.rows.item(i).station_nb,
                       available_bikes: 0,
                       available_bike_stands: 0,
                       last_update: 0});
        }
    });
    return res;
}

function addFavourite(city, stationNumber) {
    if (isNaN(stationNumber)) {
        return;
    }
    db.transaction(function(tx) {
        tx.executeSql('INSERT INTO favourites (city, station_nb, rank) VALUES(?, ?,' +
                      '(SELECT COUNT(*) FROM favourites WHERE city = ?))',
                      [ city, stationNumber, city ]);
    });
}

function removeFavourite(city, stationNumber) {
    db.transaction(function(tx) {
        // Update the ranks of the rows below the one removed
        tx.executeSql('UPDATE favourites SET rank = rank - 1 '+
                      'WHERE city = ? AND rank > ' +
                      '(SELECT rank FROM favourites WHERE city = ? AND station_nb = ?)',
                      [ city, city, stationNumber ]);

        tx.executeSql('DELETE FROM favourites WHERE city = ? AND station_nb = ?',
                      [ city, stationNumber ]);
    });
}

function moveDown(city, stationNumber) {
    db.transaction(function(tx) {
        tx.executeSql('UPDATE favourites SET rank = rank + 1 WHERE city = ? AND station_nb = ?',
                      [ city, stationNumber ]);
        tx.executeSql('UPDATE favourites ' +
                      'SET rank = rank - 1 ' +
                      'WHERE rank = (SELECT rank FROM favourites WHERE city = ? AND station_nb = ?) ' +
                      'AND city = ? AND station_nb != ?',
                      [ city, stationNumber, city, stationNumber ]);
    });
}

function moveUp(city, stationNumber) {
    db.transaction(function(tx) {
        tx.executeSql('UPDATE favourites SET rank = rank - 1 WHERE city = "' + city +
                      '" AND station_nb = ' + stationNumber);
        tx.executeSql('UPDATE favourites ' +
                      'SET rank = rank + 1 ' +
                      'WHERE rank = (SELECT rank FROM favourites WHERE city = ? AND station_nb = ?) ' +
                      'AND city = ? AND station_nb != ?',
                      [ city, stationNumber, city, stationNumber ]);
    });
}

function updateStationName(city, stationNumber, name) {
    db.transaction(function(tx) {
        tx.executeSql('UPDATE favourites SET name="' + name + '" WHERE city="' + city +
                      '" AND station_nb=' + stationNumber);
    });
}

function removeHTML(name, city) {
    // The data of those cities are encoded :/
    if (city === "Nice" || city === "Calais" || city === "Vannes") {
        return decodeURIComponent(name).replace(/\+/g, ' ');
    }
    return name;
}
