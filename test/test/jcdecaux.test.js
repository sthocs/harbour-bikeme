const fetch = require('node-fetch');
const csvparse = require('csv-parse');
const assert = require('chai').assert;

const helpers = require('../helpers');

const JCDECAUX_API_KEY = '992db598da3d81a8993aead9922b81222e4261cf';

const allCities = {};

function addCity(countryCode, cityName) {
  if (!allCities[countryCode]) {
    allCities[countryCode] = [];
  }
  allCities[countryCode].push(cityName);
}

describe('JCDecaux', async () => {
  after(() => {
    helpers.generateReport(allCities, 'jcdecaux_cities_list.txt');
  });

  let randomContract = 'seville';
  it('should get the cities list', async () => {
    const res =
      await fetch(`https://api.jcdecaux.com/vls/v1/contracts?apiKey=${JCDECAUX_API_KEY}`);
    const cities = await res.json();
    cities.forEach(city => {
      assert.isDefined(city.name, `Missing 'name' field`);
      assert.isDefined(
        city.commercial_name,
        `Missing 'commercial_name' field`
      );
      assert.isDefined(
        city.country_code,
        `Missing 'country_code' field`
      );
      addCity(city.country_code, city.name);
    });
    const randomCity = Math.floor(Math.random() * cities.length);
    randomContract = cities[randomCity].name;
  });

  it('should get the stations list', async () => {
    console.log('Testing random city: ' + randomContract);
    const res =
      await fetch(`https://api.jcdecaux.com/vls/v1/stations?contract=${randomContract}&apiKey=${JCDECAUX_API_KEY}`);
    const jsonData = await res.json();
    assert.isDefined(jsonData[0].number, `Missing 'number' field`);
    assert.isDefined(jsonData[0].name, `Missing 'name' field`);
    assert.isDefined(jsonData[0].address, `Missing 'address' field`);
    assert.isDefined(jsonData[0].position.lat, `Missing 'lat' field`);
    assert.isDefined(jsonData[0].position.lng, `Missing 'longitude' field`);
  });
});
