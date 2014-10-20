var stationsArray;

function saveStations(cartoJson) {
    stationsArray = JSON.parse(cartoJson);
    // JSON file with only stations has not the same structure. Modifying it.
    stationsArray.forEach(function(station) {
        station.position = {
            lat: station.latitude,
            lng: station.longitude
        }
        station.available_bike_stands = -1;
        station.available_bikes = -1;
    });
    return stationsArray.length;
}

function saveStationsWithDetails(cartoJson) {
    stationsArray = JSON.parse(cartoJson);
    return stationsArray.length;
}

function getStations() {
    return stationsArray;
}
