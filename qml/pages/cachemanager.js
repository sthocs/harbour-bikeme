var stationsArray;
var cities;
var citiesByCountry = {};
var countries = [];


/**
 * Take an array of cities, and sort them by countries in a JSON object where
 * each key will be the "country_code".
 * A city has the following format:
 * {"name":"Paris","commercial_name":"Velib","country_code":"FR"}
 */
function setCities(cities) {
    sortCities(cities);
    citiesByCountry = {};
    countries = [];
    cities.forEach(function(city) {
        if (!citiesByCountry[city.country_code]) {
            citiesByCountry[city.country_code] = [];
            // The "country" field is important here, it will be used by the CityDelegate to
            // determines if the object to display is a country or a city
            countries.push( { country: city.country_code, name: "", commercial_name: "" } );
        }
        citiesByCountry[city.country_code].push(city);
    });
}

function saveStations(cartoJson) {
    stationsArray = JSON.parse(cartoJson);
    // JSON file with only stations has not the same structure. Modifying it.
    stationsArray.forEach(function(station) {
        if (typeof station.position !== "object") {
            station.position = {
                lat: station.latitude,
                lng: station.longitude
            }
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

function sortCities(cities) {
    cities.forEach(function(city) {
        // Temporary fix after API change.
        if (city.countryCode) {
            city.country_code = city.countryCode;
        }
        if (city.commercialName) {
            city.commercial_name = city.commercialName;
        }
    });
    cities.sort(function(a, b) {
        if (a.country_code === b.country_code) {
            if (a.name < b.name) {
                return -1
            }
            if (a.name > b.name) {
                return 1
            }
            return 0;
        }
        else if (a.country_code < b.country_code) {
            return -1;
        }
        else {
            return 1;
        }
    });
}
