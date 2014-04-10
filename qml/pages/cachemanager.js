var stationsArray;

function saveStations(cartoJson) {
    stationsArray = JSON.parse(cartoJson);
    return stationsArray.length;
}

function getStations() {
    return stationsArray;
}
