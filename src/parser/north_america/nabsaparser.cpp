#include "nabsaparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>
#include <QRegExp>

const QString NabsaParser::CSV_DELIMITER = QString("csvSux");

QList<City*> NabsaParser::parseCities(QString systemsList, ProviderInfo& providerInfo)
{
    QList<City*> citiesList;
    updateCsvDelimiters(systemsList);
    QList<QString> lines = systemsList.split('\n');
    QList<QString> format = lines.first().split(CSV_DELIMITER);
    int countryIndex = format.indexOf("Country Code");
    int nameIndex = format.indexOf("Name");
    int locationIndex = format.indexOf("Location");
    int urlIndex = format.indexOf("Auto-Discovery URL");
    lines.removeAt(0);

    foreach (QString line, lines) {
        if (line.isEmpty()) {
            break;
        }
        QStringList csvCity = line.split(CSV_DELIMITER);
        CityInfo info;
        info.stationsDataModes = StationsListOnly | RealTimeDataOnly;
        info.providerName = providerInfo.name;
        info.name = csvCity.at(locationIndex);
        info.commercialName = csvCity.at(nameIndex);
        info.countryCode = csvCity.at(countryIndex);
        info.autoDiscoveryUrl = csvCity.at(urlIndex);
        City* city = new City();
        city->setInfo(info);
        citiesList.append(city);
    }
    return citiesList;
}

void NabsaParser::parseCityUrls(QString cityUrls, City* city)
{
    QJsonDocument doc = QJsonDocument::fromJson(cityUrls.toUtf8());
    QJsonObject data = doc.object()["data"].toObject();
    QJsonArray feedsArray;
    if (data.contains("en")) {
        feedsArray = data["en"].toObject()["feeds"].toArray();
    } else if (data.contains("feeds")) {
        feedsArray = data["feeds"].toArray();
    } else {
        QStringList languages = data.keys();
        feedsArray = data[languages[0]].toObject()["feeds"].toArray();
    }
    for (int i = 0; i < feedsArray.size(); ++i) {
        QJsonObject feed = feedsArray[i].toObject();
        if (feed["name"].toString() == "station_information") {
            city->setStationsListUrl(QUrl(feed["url"].toString()));
        }
        else if (feed["name"].toString() == "station_status") {
            city->setStationsStatusUrl(QUrl(feed["url"].toString()));
        }
    }
}

QList<Station*> NabsaParser::parseStationsList(QString allStations, bool withDetails)
{
    Q_UNUSED(withDetails)
    QList<Station*> stationsList;
    QRegExp stationIdRegex(".*(\\d+).*");
    QJsonDocument doc = QJsonDocument::fromJson(allStations.toUtf8());
    QJsonArray stationsArray = doc.object()["data"].toObject()["stations"].toArray();
    for (int i = 0; i < stationsArray.size(); ++i) {
        QJsonObject stationJson = stationsArray[i].toObject();
        Station* station = new Station();
        if (stationJson["station_id"].isString()) {
            stationIdRegex.indexIn(stationJson["station_id"].toString());
            station->number = stationIdRegex.cap(1).toInt();
        }
        else {
            station->number = stationJson["station_id"].toInt();
        }
        station->name = stationJson["name"].toString();
        station->address = stationJson["address"].toString();
        QGeoCoordinate coord(stationJson["lat"].toDouble(), stationJson["lon"].toDouble());
        station->coordinates = coord;
        stationsList.append(station);
    }
    return stationsList;
}

void NabsaParser::parseStationsRealTimeData(QString realTimeData, QList<Station *> stations)
{
    QRegExp stationIdRegex(".*(\\d+).*");
    QJsonDocument doc = QJsonDocument::fromJson(realTimeData.toUtf8());
    QJsonArray stationsArray = doc.object()["data"].toObject()["stations"].toArray();
    for (int i = 0; i < stationsArray.size() && i < stations.length(); ++i) {
        QJsonObject stationJson = stationsArray[i].toObject();
        int stationNumber;
        if (stationJson["station_id"].isString()) {
            stationIdRegex.indexIn(stationJson["station_id"].toString());
            stationNumber = stationIdRegex.cap(1).toInt();
        }
        else {
            stationNumber = stationJson["station_id"].toInt();
        }
        Station* station = stations.at(i); // normally, stations info and data are in the same order
        if (station->number != stationNumber) { // if it's not the case, search it
            foreach (Station* s, stations) {
                if (s->number == stationNumber) {
                    station = s;
                    break;
                }
            }
        }
        station->available_bikes = stationJson["num_bikes_available"].toInt();
        station->available_bike_stands = stationJson["num_docks_available"].toInt();
        station->opened = stationJson["is_renting"].toInt() == 1;
        station->last_update = QDateTime::fromTime_t(stationJson["last_reported"].toDouble());
    }
}

/*
 * Implementing my own simple CSV parser ; first step:
 * Update CSV delimiters with one that can't be mixed up with actual data
 * and remove the quotes around quoted values.
 */
void NabsaParser::updateCsvDelimiters(QString& csvData)
{
    bool inQuotedValue = false;
    for (int i = 0; i < csvData.length(); ++i) {
        if (csvData.at(i) == '"' && (i == 0 || csvData.at(i - 1) != '\\')) {
            inQuotedValue = !inQuotedValue;
            csvData.remove(i--, 1);
            continue;
        }
        if (csvData.at(i) == ',' && !inQuotedValue) {
            csvData.replace(i, 1, CSV_DELIMITER);
            i += CSV_DELIMITER.length() - 1;
        }
    }
}
