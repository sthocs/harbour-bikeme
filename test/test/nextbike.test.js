const fetch = require('node-fetch');
const csvparse = require('csv-parse');
const assert = require('chai').assert;

const helpers = require('../helpers');

const CITIES_LIST_URL = 'https://api.nextbike.net/maps/nextbike-live.json?list_cities=1';
const CITY_URL = 'https://api.nextbike.net/maps/nextbike-live.json?city=';

const allCities = {};

function addCity(countryCode, cityName) {
  if (!allCities[countryCode]) {
    allCities[countryCode] = [];
  }
  allCities[countryCode].push(cityName);
}

describe('Nextbike', async () => {
  after(() => {
    helpers.generateReport(allCities, 'nextbike_cities_list.txt');
  });

  let randomCityToTest = 50;
  it('should get the cities list', async () => {
    const res = await fetch(CITIES_LIST_URL);
    const nextbikeData = await res.json();
    let citiesNb = 0;
    assert.isDefined(nextbikeData.countries, `Missing 'countries' field`);
    nextbikeData.countries.forEach(country => {
      const cities = country.cities;
      assert.isDefined(cities, `Missing 'cities' field`);
      cities.forEach(city => {
        citiesNb++;
        assert.isDefined(city.uid, `Missing 'uid' field`);
        assert.isDefined(city.lat, `Missing 'lat' field`);
        assert.isDefined(city.lng, `Missing 'lng' field`);
        addCity(country.country, city.name);
      });
    });
    console.log(`Countries: ${nextbikeData.countries.length}; Cities: ${citiesNb}`);
    const randomCountry = Math.floor(
      Math.random() * nextbikeData.countries.length
    );
    const randomCity = Math.floor(
      Math.random() * nextbikeData.countries[randomCountry].cities.length
    );
    randomCityToTest =
    nextbikeData.countries[randomCountry].cities[randomCity].uid;
  });

  it('should get the stations list', async () => {
    console.log("Testing random city: " + randomCityToTest);
    const res = await fetch(`${CITY_URL}${randomCityToTest}`);
    const jsonData = await res.json();
    assert.isDefined(jsonData.countries, "Missing 'countries' field");
    let country;
    for (let i = 0; i < jsonData.countries.length; ++i) {
      if (jsonData.countries[i].cities.length == 1) {
        country = jsonData.countries[i];
      }
    }
    assert.isDefined(country, 'No valid country found');
    const city = country.cities[0];
    const places = city.places;
    places.forEach(station => {
      assert.isDefined(station.uid, `Missing 'uid' field`);
      assert.isDefined(station.lat, `Missing 'uid' field`);
      assert.isDefined(station.lng, `Missing 'uid' field`);
      assert.isDefined(station.bikes, `Missing 'bikes' field`);
      assert.isDefined(
        station.free_racks,
        `Missing 'free_racks' field`
      );
    });
  });
});
