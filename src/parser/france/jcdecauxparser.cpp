#include "jcdecauxparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>


QList<City*> JCDecauxParser::parseCities(QString cities, ProviderInfo& providerInfo) {
    QList<City*> citiesList;
    QJsonDocument doc = QJsonDocument::fromJson(cities.toUtf8());
    QJsonArray citiesArray = doc.array();
    for (int i = 0; i < citiesArray.size(); ++i) {
        QJsonObject cityJson = citiesArray[i].toObject();
        CityInfo info;
        info.stationsDataModes = StationsListAndData | SingleStationData;
        info.name = cityJson["name"].toString();
        info.commercialName = cityJson["commercial_name"].toString();
        info.countryCode = cityJson["country_code"].toString();
        info.singleStationDetailsUrlTemplate = providerInfo.singleStationDetailsUrlTemplate.arg(info.name);
        info.allStationsDetailsUrl = QUrl(providerInfo.allStationsDetailsUrl.arg(info.name));
        info.providerName = providerInfo.name;
        City* city = new City();
        city->setInfo(info);
        citiesList.append(city);
    }
    return citiesList;
}

QList<Station*> JCDecauxParser::parseStationsList(QString allStationsDetails, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStationsDetails.toUtf8());
    QJsonArray stationsArray = doc.array();
    for (int i = 0; i < stationsArray.size(); ++i) {
        QJsonObject stationJson = stationsArray[i].toObject();
        Station* station = new Station();
        station->number = stationJson["number"].toInt();
        station->name = stationJson["name"].toString();
        station->address = stationJson["address"].toString();
        QJsonObject position = stationJson["position"].toObject();
        QGeoCoordinate coord(position["lat"].toDouble(), position["lng"].toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->opened = stationJson["status"].toString().compare("OPEN") == 0;
            station->bike_stands = stationJson["bike_stands"].toInt();
            station->available_bike_stands = stationJson["available_bike_stands"].toInt();
            station->available_bikes = stationJson["available_bikes"].toInt();
            station->last_update = QDateTime::fromMSecsSinceEpoch(stationJson["last_update"].toDouble());
        }
        else {
            station->opened = true;
            station->bike_stands = -1;
            station->available_bike_stands = -1;
            station->available_bikes = -1;
        }
        stationsList.append(station);
    }
    return stationsList;
}

void JCDecauxParser::parseStationDetails(QString stationDetails, Station* station)
{
    QJsonDocument doc = QJsonDocument::fromJson(stationDetails.toUtf8());
    QJsonObject stationJson = doc.object();
    station->number = stationJson["number"].toInt();
    station->name = stationJson["name"].toString();
    station->address = stationJson["address"].toString();
    QJsonObject position = stationJson["position"].toObject();
    QGeoCoordinate coord(position["lat"].toDouble(), position["lng"].toDouble());
    station->coordinates = coord;
    station->opened = stationJson["status"].toString().compare("OPEN") == 0;
    station->bike_stands = stationJson["bike_stands"].toInt();
    station->available_bike_stands = stationJson["available_bike_stands"].toInt();
    station->available_bikes = stationJson["available_bikes"].toInt();
    station->last_update = QDateTime::fromMSecsSinceEpoch(stationJson["last_update"].toDouble());
}
