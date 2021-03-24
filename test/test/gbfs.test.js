const fetch = require('node-fetch');
const csvparse = require('csv-parse/lib/sync');
const assert = require('chai').assert;

const helpers = require('../helpers');

describe("NABSA", async () => {
  it("All city should have the same format", async () => {
    const res = await fetch("https://raw.githubusercontent.com/NABSA/gbfs/master/systems.csv");
    const csvData = await res.text();
    const rows = csvparse(csvData);
    rows.splice(0, 1);
    for (const row of rows) {
      console.log(`testing ${row}`);
      await testCity(row);
    }
  });
});

async function testCity(csvLine) {
  const res = await fetch(csvLine[5]); // Auto-discovery URL
  try {
    const cityUrls = await res.json();
    console.log(cityUrls);
    if (!cityUrls.data) {
      assert.fail(`Data not defined for ${csvLine[5]}: ${jsonData}`);
    }
    if (!cityUrls.data.en) {
      assert.fail(`City ${csvLine[5]} is missing 'en' field`);
    }
    const feeds = cityUrls.data.en.feeds;
    await Promise.all(feeds.map(async (feed) => {
      if (feed.name == "station_information") {
        const res = await fetch(feed.url);
        const stations = (await res.json()).data.stations;
        if (stations.length === 0) {
          console.log(`No stations for ${feed.url}`);
          return;
        }
        const randomStationIndex = Math.floor(
          Math.random() * stations.length
        );
        const idRegex = /.*\d+.*/g;
        assert.isTrue(
          idRegex.test(
            stations[randomStationIndex].station_id
          )
        );
        assert.isString(stations[randomStationIndex].name);
        assert.isNumber(stations[randomStationIndex].lat);
        assert.isNumber(stations[randomStationIndex].lon);
      }
      if (feed.name == "station_status") {
        const res = await fetch(feed.url);
        const stations = (await res.json()).data.stations;
        if (stations.length === 0) {
          console.log(`No stations for ${feed.url}`);
          return;
        }
        const randomStationIndex = Math.floor(
          Math.random() * stations.length
        );
        const idRegex = /.*\d+.*/g;
        assert.isTrue(
          idRegex.test(
            stations[randomStationIndex].station_id
          )
        );
        assert.isNumber(stations[randomStationIndex].num_bikes_available);
        assert.isNumber(stations[randomStationIndex].num_docks_available);
        assert.isNumber(stations[randomStationIndex].is_renting);
        assert.isNumber(stations[randomStationIndex].last_reported);
      }
    }));
  } catch (e) {
    console.log(e);
    assert.fail(`Error while parsing json for ${csvLine[5]}: ${e}`);
  }
}
