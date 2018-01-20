var http = require('http');
var https = require('https');
var urlModule = require('url');
var fs = require('fs');

var xmldoc = require('xmldoc');
var csvparse = require('csv-parse');

var assert = require('chai').assert;

var httpModules = {
    'http:': http,
    'https:': https,
};

var JCDECAUX_API_KEY = '';

var allCities = {};

function addCity(countryCode, cityName) {
    if (!allCities[countryCode]) {
        allCities[countryCode] = [];
    }
    allCities[countryCode].push(cityName);
}

describe('Providers', function() {
    after(function() {
        var nbCountries = 0;
        var nbCities = 0;
        var sortedCountries = [];
        for (var c in allCities) {
            ++nbCountries;
            nbCities += allCities[c].length;
            allCities[c].sort();
            sortedCountries.push(c + ": " + allCities[c].join(", "));
        }
        sortedCountries.sort();
        sortedCountries.push("");
        console.log(nbCities + " cities in " + nbCountries + " countries.");
        fs.writeFileSync('cities_list.txt', sortedCountries.join('\n'));
    });

    describe('JCDecaux', function() {
        var randomContract = 'seville';
        it('Cities list', function(done) {
            var jsonData = "";
            https.get('https://api.jcdecaux.com/vls/v1/contracts?apiKey=' + JCDECAUX_API_KEY, function(res) {
                res.on('data', function(d) {
                    jsonData += d;
                });
                res.on('end', function() {
                    var cities = JSON.parse(jsonData);
                    var res = {};
                    cities.forEach(function(city) {
                        assert.isDefined(city.name, "Missing 'name' field");
                        assert.isDefined(city.commercial_name, "Missing 'commercial_name' field");
                        assert.isDefined(city.country_code, "Missing 'country_code' field");
                        addCity(city.country_code, city.name);
                    });
                    var randomCity = Math.floor((Math.random() * cities.length));
                    randomContract = cities[randomCity].name;
                    done();
                });
            }).on('error', function(e) {
                assert.fail(e, undefined, "Request has failed: " + e);
            });
        });
        
        it('Stations list', function(done) {
            console.log("Testing random city: " + randomContract);
            var jsonData = "";
            https.get('https://api.jcdecaux.com/vls/v1/stations?contract=' + randomContract + '&apiKey=' + JCDECAUX_API_KEY, function(res) {
                res.on('data', function(d) {
                    jsonData += d;
                });
                res.on('end', function() {
                    jsonData = JSON.parse(jsonData);
                    assert.isDefined(jsonData[0].number, "Missing 'number' field");
                    assert.isDefined(jsonData[0].name, "Missing 'name' field");
                    assert.isDefined(jsonData[0].address, "Missing 'address' field");
                    assert.isDefined(jsonData[0].position.lat, "Missing 'lat' field");
                    assert.isDefined(jsonData[0].position.lng, "Missing 'longitude' field");
                    done();
                });

            }).on('error', function(e) {
                assert.fail(e, undefined, "Request has failed: " + e);
            });
        });
    });

    describe('Nextbike', function() {
        var randomCityToTest = 50;
        it('Cities list', function(done) {
            var jsonData = "";
            https.get('https://api.nextbike.net/maps/nextbike-live.json?list_cities=!', function(res) {
                res.on('data', function(d) {
                    jsonData += d;
                });
                res.on('end', function() {
                    jsonData = JSON.parse(jsonData);
                    var citiesNb = 0;
                    assert.isDefined(jsonData.countries, "Missing 'countries' field");
                    jsonData.countries.forEach(function(country) {
                        var cities = country.cities;
                        assert.isDefined(cities, "Missing 'cities' field");
                        cities.forEach(function(city) {
                            citiesNb++;
                            assert.isDefined(city.uid, "Missing 'uid' field");
                            assert.isDefined(city.lat, "Missing 'lat' field");
                            assert.isDefined(city.lng, "Missing 'lng' field");
                            addCity(country.country, city.name);
                        });
                    });
                    console.log("Countries: " + jsonData.countries.length + "; Cities: " + citiesNb);
                    var randomCountry = Math.floor((Math.random() * jsonData.countries.length));
                    var randomCity = Math.floor((Math.random() * jsonData.countries[randomCountry].cities.length));
                    randomCityToTest = jsonData.countries[randomCountry].cities[randomCity].uid;
                    done();
                });
            }).on('error', function(e) {
                assert.fail(e, undefined, "Request has failed: " + e);
            });
        });

        it('Stations list', function(done) {
            console.log("Testing random city: " + randomCityToTest);
            var jsonData = "";
            https.get('https://api.nextbike.net/maps/nextbike-live.json?city=' + randomCityToTest, function(res) {
                res.on('data', function(d) {
                    jsonData += d;
                });
                res.on('end', function() {
                    jsonData = JSON.parse(jsonData);
                    assert.isDefined(jsonData.countries, "Missing 'countries' field");
                    var country;
                    for (var i = 0; i < jsonData.countries.length; ++i) {
                        if (jsonData.countries[i].cities.length == 1) {
                            country = jsonData.countries[i];
                        }
                    }
                    assert.isDefined(country, "No valid country found");
                    var city = country.cities[0];
                    var places = city.places;
                    places.forEach(function(station) {
                        assert.isDefined(station.uid, "Missing 'uid' field");
                        assert.isDefined(station.lat, "Missing 'uid' field");
                        assert.isDefined(station.lng, "Missing 'uid' field");
                        assert.isDefined(station.bikes, "Missing 'bikes' field");
                        assert.isDefined(station.free_racks, "Missing 'free_racks' field");
                    });
                    done();
                });

            }).on('error', function(e) {
                assert.fail(e, undefined, "Request has failed: " + e);
            });
        });
    });
    
    describe('NABSA', function() {
        it('All city should have the same format', function(done) {
            var csvData = "";
            https.get('https://raw.githubusercontent.com/NABSA/gbfs/master/systems.csv', function(res) {
                res.on('data', function(d) {
                    csvData += d;
                });
                res.on('end', function() {
                    csvparse(csvData, function(err, output) {
                        var headers = output.splice(0, 1)[0];
                        var discoveryUrlIndex = headers.indexOf("Auto-Discovery URL");
                        var countryCodeIndex = headers.indexOf("Country Code");
                        var locationIndex = headers.indexOf("Location");
                        output.forEach(function(line) {
                            addCity(line[countryCodeIndex], line[locationIndex]);
                        });
                        var randomLineIndex = Math.floor((Math.random() * output.length));
                        var citiesTested = 0;
                        output.forEach(function(line) {
                            var currentCity = line[locationIndex];
                            get_data(line[discoveryUrlIndex], function(jsonData, err) {
                                if (err) {
                                    if (++citiesTested == output.length) {
                                        done();
                                    }
                                    return;
                                }
                                try {
                                    var cityUrls = JSON.parse(jsonData);
                                }
                                catch(e) {
                                    assert.fail(jsonData, "", "Bad json for city " + currentCity + ": " + jsonData)
                                }
                                if (!cityUrls.data) {
                                    console.log("Data not defined for " + currentCity + ": " + jsonData);
                                    ++citiesTested;
                                    return;
                                }
                                var feeds = cityUrls.data.en.feeds;
                                var executedRequests = 0;
                                feeds.forEach(function(feed) {
                                    if (feed.name == "station_information") {
                                        get_data(feed.url, function(res, err) {
                                            if (!err) {
                                                var stations;
                                                try {
                                                    stations = JSON.parse(res).data.stations;
                                                }
                                                catch(e) {
                                                    console.log(res);
                                                    assert.fail(res, "", "Invalid JSON for station_information. City: " + currentCity + ": " + e);
                                                }
                                                if (stations.length == 0) {
                                                    console.log("No stations for " + currentCity + ": " + res);
                                                }
                                                else {
                                                    var randomStationIndex = Math.floor((Math.random() * stations.length));
                                                    var idRegex = /.*\d+.*/g;
                                                    assert.isTrue(idRegex.test(stations[randomStationIndex].station_id), "station: " + stations[randomStationIndex].station_id);
                                                    assert.isString(stations[randomStationIndex].name);
                                                    assert.isNumber(stations[randomStationIndex].lat);
                                                    assert.isNumber(stations[randomStationIndex].lon);
                                                }
                                            }
                                            if (++executedRequests == 2) {
                                                if (++citiesTested == output.length) {
                                                    done();
                                                }
                                            }
                                        }, true);
                                    }
                                    if (feed.name == "station_status") {
                                        get_data(feed.url, function(res, err) {
                                            if (!err) {
                                                var stations;
                                                try {
                                                    stations = JSON.parse(res).data.stations;
                                                }
                                                catch(e) {
                                                    console.log(res);
                                                    assert.fail(res, "", "Invalid JSON for station_status. City: " + currentCity);
                                                }
                                                var randomStationIndex = Math.floor((Math.random() * stations.length));
                                                var idRegex = /.*\d+.*/g;
                                                if (stations.length == 0) {
                                                    console.log("No stations for " + currentCity + ": " + res);
                                                }
                                                else {
                                                    assert.isTrue(idRegex.test(stations[randomStationIndex].station_id), "station: " + stations[randomStationIndex].station_id);
                                                    assert.isNumber(stations[randomStationIndex].num_bikes_available);
                                                    assert.isNumber(stations[randomStationIndex].num_docks_available);
                                                    //assert.isNumber(stations[randomStationIndex].is_renting, "is_renting field of city " + currentCity);
                                                    assert.isNumber(stations[randomStationIndex].last_reported, "Missing last_reported field, station " +
                                                    stations[randomStationIndex].station_id + " (" + feed.url + ")");
                                                }
                                            }
                                            if (++executedRequests == 2) {
                                                if (++citiesTested == output.length) {
                                                    done();
                                                }
                                            }
                                        }, true);
                                    }
                                });
                            });
                        });
                    });
                });
            }).on('error', function(e) {
                assert.fail(e, undefined, "Request has failed: " + e);
            });
        });
    });

    describe('Standalone providers', function() {
        var cities = JSON.parse(fs.readFileSync('../data/cities.json'));
        cities.forEach(function(city) {
            addCity(city.country_code, city.name);
            it(city.name, function(done) {
                if (!tests[city.provider]) {
                    console.log("No tests for " + city.name);
                    done();
                    return;
                }
                tests[city.provider](city.country_code, city.name, city.allStationsDetailsUrl, function() {
                    done();
                });
            });
        });
    });
})

var tests = {
    Bixi: function(country, name, url, done) {
        get_data(url, function(jsonData) {
            var stations = JSON.parse(jsonData).stations;
            assert.isTrue(Array.isArray(stations));
            stations.forEach(function(station) {
                assert.isNumber(station.id, "id is missing");
                assert.isString(station.s, "Adresse is missing");
                assert.isNumber(station.la, "Lat is missing");
                assert.isNumber(station.lo, "Lon is missing");
                assert.isNumber(station.da, "da is missing");
                assert.isNumber(station.dx, "dx is missing");
                assert.isNumber(station.ba, "ba is missing");
                assert.isBoolean(station.b, "b is missing");
                assert.isBoolean(station.su, "su is missing");
                assert.isNumber(station.lu, "lu is missing");
            });
            done();
        });
    },
    HSL: function(country, name, url, done) {
        get_data(url, function(jsonData) {
            var stations = JSON.parse(jsonData).stations;
            assert.isTrue(Array.isArray(stations));
            stations.forEach(function(station) {
                assert.isString(station.id, "id is missing");
                assert.isString(station.name, "name is missing");
                assert.isNumber(station.y, "Lat is missing");
                assert.isNumber(station.x, "Lon is missing");
                assert.isNumber(station.bikesAvailable, "bikesAvailable is missing");
                assert.isNumber(station.spacesAvailable, "spacesAvailable is missing");
                assert.isBoolean(station.allowDropoff, "allowDropoff is missing");
            });
            done();
        });
    },
    EcoBici: function(country, name, url, done) {
        get_data(url, function(jsonData) {
            var stations = JSON.parse(jsonData);
            assert.isTrue(Array.isArray(stations));
            stations.forEach(function(station) {
                assert.isString(station.id, "id is missing");
                assert.isString(station.name, "name is missing");
                assert.isString(station.address, "address is missing");
                assert.isString(station.lat, "Lat is missing");
                assert.isString(station.lon, "Lon is missing");
                assert.isString(station.bikes, "bikes is missing");
                assert.isString(station.slots, "slots is missing");
                assert.isString(station.status, "status is missing");
            });
            done();
        });
    },
    Smoove: function(country, name, url, done) {
        var xmlData = "";
            
        http.get(url, function(res) {
            res.on('data', function(d) {
                xmlData += d;
            });
            res.on('end', function() {
                var document = new xmldoc.XmlDocument(xmlData);
                assert.equal(document.name, "vcs");
                assert.isDefined(document.childNamed('sl'), "Node 'sl' is missing");
                var slNode = document.childNamed('sl');
                slNode.eachChild(function(station) {
                    assert.isDefined(station.attr.id, "Attr 'id' is missing");
                    assert.isDefined(station.attr.na, "Attr 'na' is missing");
                    assert.isDefined(station.attr.la, "Attr 'la' is missing");
                    assert.isDefined(station.attr.lg, "Attr 'lg' is missing");
                    assert.isDefined(station.attr.av, "Attr 'av' is missing");
                    assert.isDefined(station.attr.fr, "Attr 'fr' is missing");
                });
                done();
            });
        }).on('error', function(e) {
            assert.fail(e, undefined, "Request has failed: " + e);
        });
    },
    Transpole: function(country, name, url, done) {
        var jsonData = "";
        https.get(url, function(res) {
            res.on('data', function(d) {
                jsonData += d;
            });
            res.on('end', function() {
                var carto = JSON.parse(jsonData).records;
                assert.isTrue(Array.isArray(carto));
                carto.forEach(function(station) {
                    assert.isNumber(station.fields.libelle, "Libelle is missing");
                    assert.isString(station.fields.etat, "Etat is missing");
                    assert.isDefined(station.fields.adresse, "Adresse is missing");
                    assert.isNumber(station.fields.geo["0"], "Lat is missing");
                    assert.isNumber(station.fields.geo["1"], "Lon is missing");
                    assert.isNumber(station.fields.nbvelosdispo, "nbvelosdispo is missing");
                    assert.isNumber(station.fields.nbplacesdispo, "nbplacesdispo is missing");
                });
                done();
            });
        }).on('error', function(e) {
            assert.fail(e, undefined, "Request has failed: " + e);
        });
    },
    RennesMetropole: function(country, name, url, done) {
        var jsonData = "";
        https.get(url, function(res) {
            res.on('data', function(d) {
                jsonData += d;
            });
            res.on('end', function() {
                var carto = JSON.parse(jsonData).records;
                assert.isTrue(Array.isArray(carto));
                carto.forEach(function(station) {
                    assert.isNumber(station.fields.idstation, "Libelle is missing");
                    assert.isString(station.fields.etat, "Etat is missing");
                    assert.isDefined(station.fields.nom, "Name is missing");
                    assert.isNumber(station.fields.coordonnees["0"], "Lat is missing");
                    assert.isNumber(station.fields.coordonnees["1"], "Lon is missing");
                    assert.isNumber(station.fields.nombrevelosdisponibles, "nbVelosDispo is missing");
                    assert.isNumber(station.fields.nombreemplacementsdisponibles, "nbPlacesDispo is missing");
                });
                done();
            });
        }).on('error', function(e) {
            assert.fail(e, undefined, "Request has failed: " + e);
        });
    },
    Veloway: function(country, name, url, done) {
        var jsonData = "";
        https.get(url, function(res) {
            res.on('data', function(d) {
                jsonData += d;
            });
            res.on('end', function() {
                var carto = JSON.parse(jsonData);
                assert.isTrue(Array.isArray(carto.stand));
                carto.stand.forEach(function(station) {
                    assert.isString(station.id, "id is missing");
                    assert.isDefined(station.wcom, "wcom is missing");
                    assert.isString(station.lat, "Lat is missing");
                    assert.isString(station.lng, "Lng is missing");
                    assert.isString(station.ab, "Available bikes is missing");
                    assert.isString(station.ap, "Available parking is missing");
                });
                done();
            });
        }).on('error', function(e) {
            assert.fail(e, undefined, "Request has failed: " + e);
        });
    },
    TfL: function(country, name, url, done) {
        var xmlData = "";
        https.get(url, function(res) {
            res.on('data', function(d) {
                xmlData += d;
            });
            res.on('end', function() {
                var document = new xmldoc.XmlDocument(xmlData);
                assert.equal(document.name, "stations");
                assert.isDefined(document.attr.lastUpdate, "Attr 'lastUpdate' is missing");
                document.eachChild(function(station) {
                    assert.isDefined(station.childNamed('id'), "Node 'id' is missing");
                    assert.isDefined(station.childNamed('name'), "Node 'name' is missing");
                    assert.isDefined(station.childNamed('lat'), "Node 'lat' is missing");
                    assert.isDefined(station.childNamed('long'), "Node 'long' is missing");
                    assert.isDefined(station.childNamed('nbBikes'), "Node 'nbBikes' is missing");
                    assert.isDefined(station.childNamed('nbEmptyDocks'), "Node 'nbEmptyDocks' is missing");
                });
                done();
            });
        }).on('error', function(e) {
            assert.fail(e, undefined, "Request has failed: " + e);
        });
    },
    Velobike: function(country, name, url, done) {
        var jsonData = "";
        https.get(url, function(res) {
            res.on('data', function(d) {
                jsonData += d;
            });
            res.on('end', function() {
                try {
                    var carto = JSON.parse(jsonData);
                }
                catch (e) {
                    console.log("Bad response for Moscow: " + e + ": " + jsonData);
                }
                assert.isTrue(Array.isArray(carto.Items));
                carto.Items.forEach(function(station) {
                    assert.isString(station.Id, "Id is missing");
                    assert.isDefined(station.Address, "Address is missing");
                    assert.isNumber(station.Position.Lat, "Lat is missing");
                    assert.isNumber(station.Position.Lon, "Lon is missing");
                    assert.isNumber(station.TotalPlaces, "TotalPlaces is missing");
                    assert.isNumber(station.FreePlaces, "FreePlaces is missing");
                });
                done();
            });
        }).on('error', function(e) {
            assert.fail(e, undefined, "Request has failed: " + e);
        });
    }
}

// Helper for GET method, handling redirections and user agent
function get_data(url, callback, followRedirect) {
    var parsedUrl = urlModule.parse(url);
    var options = {
        hostname: parsedUrl.hostname,
        path: parsedUrl.path,
        headers: { 'User-Agent': 'BikeMe urls test' }
    }
    httpModules[parsedUrl.protocol].get(options, function(res) {
        if (res.statusCode == 301 && followRedirect) {
            console.log("Redirecting to " + res.headers['location']);
            get_data(res.headers['location'], callback);
            return;
        }
        
        var rawData = "";
        res.on('data', function(d) { rawData += d; });
        res.on('end', function() {
            callback(rawData);
        });
    }).on('error', function(e) {
        console.log(e);
        callback(undefined, e);
    });
}
