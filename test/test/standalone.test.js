const fs = require('fs');
const fetch = require('node-fetch');
const xmldoc = require("xmldoc");
const assert = require('chai').assert;

describe('Standalone providers', async () => {
  const cities = JSON.parse(fs.readFileSync("../data/cities.json"));
  cities.forEach(city => {
    it(city.name, async () => {
      if (!tests[city.provider]) {
        console.log("No tests for " + city.name);
        return;
      }
      await tests[city.provider](
        city.country_code,
        city.name,
        city.allStationsDetailsUrl
      );
    });
  });
});

const tests = {
  Bixi: async function(country, name, url) {
    const res = await fetch(url);
    const stations = (await res.json()).stations;
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
  },
  HSL: async function(country, name, url) {
    const res = await fetch(url);
    const stations = (await res.json()).stations;
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
  },
  EcoBici: async function(country, name, url) {
    const res = await fetch(url);
    const stations = await res.json();
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
  },
  Smoove: async function(country, name, url) {
    const xmlData = await (await fetch(url)).text();
    const document = new xmldoc.XmlDocument(xmlData);
    assert.equal(document.name, "vcs");
    assert.isDefined(document.childNamed("sl"), "Node 'sl' is missing");
    const slNode = document.childNamed("sl");
    slNode.eachChild(function(station) {
      assert.isDefined(station.attr.id, "Attr 'id' is missing");
      assert.isDefined(station.attr.na, "Attr 'na' is missing");
      assert.isDefined(station.attr.la, "Attr 'la' is missing");
      assert.isDefined(station.attr.lg, "Attr 'lg' is missing");
      assert.isDefined(station.attr.av, "Attr 'av' is missing");
      assert.isDefined(station.attr.fr, "Attr 'fr' is missing");
    });
  },
  Transpole: async function(country, name, url) {
    const res = await fetch(url);
    const carto = (await res.json()).records;
    assert.isTrue(Array.isArray(carto));
    carto.forEach(function(station) {
      assert.isNumber(station.fields.libelle, "Libelle is missing");
      assert.isString(station.fields.etat, "Etat is missing");
      assert.isDefined(station.fields.nom, "Nom is missing");
      assert.isDefined(station.fields.adresse, "Adresse is missing");
      assert.isNumber(station.fields.geo["0"], "Lat is missing");
      assert.isNumber(station.fields.geo["1"], "Lon is missing");
      assert.isNumber(
        station.fields.nbvelosdispo,
        "nbvelosdispo is missing"
      );
      assert.isNumber(
        station.fields.nbplacesdispo,
        "nbplacesdispo is missing"
      );
    });
  },
  RennesMetropole: async function(country, name, url) {
    const res = await fetch(url);
    const carto = (await res.json()).records;
    assert.isTrue(Array.isArray(carto));
    carto.forEach(function(station) {
      assert.isNumber(
        parseInt(station.fields.idstation),
        "idstation is missing"
      );
      assert.isString(station.fields.etat, "Etat is missing");
      assert.isDefined(station.fields.nom, "Name is missing");
      if (station.fields.etat !== 'En fonctionnement') {
        return;
      }
      assert.isNumber(station.fields.coordonnees["0"], "Lat is missing");
      assert.isNumber(station.fields.coordonnees["1"], "Lon is missing");
      assert.isNumber(
        station.fields.nombrevelosdisponibles,
        "nbVelosDispo is missing"
      );
      assert.isNumber(
        station.fields.nombreemplacementsdisponibles,
        "nbPlacesDispo is missing"
      );
    });
  },
  Veloway: async function(country, name, url) {
    const res = await fetch(url);
    const carto = await res.json();
    assert.isTrue(Array.isArray(carto.stand));
    carto.stand.forEach(function(station) {
      assert.isString(station.id, "id is missing");
      assert.isDefined(station.wcom, "wcom is missing");
      assert.isString(station.lat, "Lat is missing");
      assert.isString(station.lng, "Lng is missing");
      assert.isString(station.ab, "Available bikes is missing");
      assert.isString(station.ap, "Available parking is missing");
    });
  },
  TfL: async function(country, name, url) {
    const res = await fetch(url);
    const xmlData = await res.text();
    var document = new xmldoc.XmlDocument(xmlData);
    assert.equal(document.name, "stations");
    assert.isDefined(
      document.attr.lastUpdate,
      "Attr 'lastUpdate' is missing"
    );
    document.eachChild(function(station) {
      assert.isDefined(station.childNamed("id"), "Node 'id' is missing");
      assert.isDefined(station.childNamed("name"), "Node 'name' is missing");
      assert.isDefined(station.childNamed("lat"), "Node 'lat' is missing");
      assert.isDefined(station.childNamed("long"), "Node 'long' is missing");
      assert.isDefined(
        station.childNamed("nbBikes"),
        "Node 'nbBikes' is missing"
      );
      assert.isDefined(
        station.childNamed("nbEmptyDocks"),
        "Node 'nbEmptyDocks' is missing"
      );
    });
  },
  Velobike: async function(country, name, url, done) {
    const res = await fetch(url);
    const carto = await res.json();
    assert.isTrue(Array.isArray(carto.Items));
    carto.Items.forEach(function(station) {
      assert.isString(station.Id, "Id is missing");
      assert.isDefined(station.Name, "Name is missing");
      assert.isDefined(station.Address, "Address is missing");
      assert.isNumber(station.Position.Lat, "Lat is missing");
      assert.isNumber(station.Position.Lon, "Lon is missing");
      assert.isNumber(station.TotalPlaces, "TotalPlaces is missing");
      assert.isNumber(station.FreePlaces, "FreePlaces is missing");
    });
  },
  Smovengo: async function(country, name, url, done) {
    const res = await fetch(url);
    const carto = await res.json();
    assert.isTrue(Array.isArray(carto));
    carto.forEach(function(station) {
      assert.isString(station.station.code, "station.code is missing");
      assert.isDefined(station.station.name, "station.name is missing");
      assert.isString(station.station.state, "station.state is missing");
      assert.isNumber(
        station.station.gps.latitude,
        "station.latitude is missing"
      );
      assert.isNumber(
        station.station.gps.longitude,
        "station.longitude is missing"
      );
      assert.isNumber(station.nbBike, "nbBike is missing");
      assert.isNumber(
        station.nbEbike,
        "nbEbike is missing: " + JSON.stringify(station)
      );
      assert.isNumber(station.nbFreeDock, "nbFreeDock is missing");
      assert.isNumber(station.nbFreeEDock, "nbFreeEDock is missing");
    });
  }
};
