const fetch = require('node-fetch');
const csvparse = require('csv-parse/lib/sync');
const assert = require('chai').assert;

const helpers = require('../helpers');

describe("NABSA", async () => {
  it("Cities should have the same format", async () => {
    const res = await fetch("https://raw.githubusercontent.com/NABSA/gbfs/master/systems.csv");
    const csvData = await res.text();
    const rows = csvparse(csvData);
    rows.splice(0, 1);
    const testedRows = new Array(50);
    for (let i = 0; i < 50; ++i) {
      testedRows[i] = rows[getRandomInt(0, rows.length)];
    }
    await Promise.all(testedRows.map(async (row) => {
      await testCity(row);
    }));
  });
});

async function testCity(csvLine) {
  const res = await fetch(csvLine[5]); // Auto-discovery URL
  try {
    const cityUrls = await res.json();
    if (!cityUrls.data) {
      assert.fail(`Data not defined for ${csvLine[5]}: ${jsonData}`);
    }
    let feeds;
    if (cityUrls.data.en) {
      feeds = cityUrls.data.en.feeds
    } else if (cityUrls.data.feeds) {
      feeds = cityUrls.data.feeds;
    } else {
      console.log('taking first key: ' + [Object.keys(cityUrls.data)[0]]);
      feeds = cityUrls.data[Object.keys(cityUrls.data)[0]].feeds;
    }
    if (!feeds) {
      assert.fail(`No feeds found for ${csvLine[5]}`);
    }
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

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min)) + min;
}
