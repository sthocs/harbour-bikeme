#include "citybikesparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>

QList<City*> CityBikesParser::parseCities(QString cities, ProviderInfo& providerInfo)
{
    QList<City*> citiesList;
    QJsonDocument doc = QJsonDocument::fromJson(cities.toUtf8());
    QJsonArray networksArray = doc.object()["networks"].toArray();
    for (int i = 0; i < networksArray.size(); ++i) {
        QJsonObject cityJson = networksArray[i].toObject();
        CityInfo info;
        info.id_str = cityJson["id"].toString();
        info.name = cityJson["location"].toObject()["city"].toString();
        info.commercialName = cityJson["name"].toString();
        info.countryCode = cityJson["location"].toObject()["country"].toString();
        info.countryCode.truncate(2);
        if (info.countryCode == "UK") {
            info.countryCode = "GB"; // correct ISO code
        }
        info.allStationsDetailsUrl = QUrl(providerInfo.allStationsDetailsUrl.arg(info.id_str));
        info.providerName = providerInfo.name;
        info.stationsDataModes = StationsListAndData;
        City* city = new City();
        city->setInfo(info);
        citiesList.append(city);
    }
    return citiesList;
}

QList<Station*> CityBikesParser::parseStationsList(QString allStations, bool withDetails)
{
    QList<Station*> stationsList;
    QRegExp stationIdRegex(".*(\\d+).*");
    QJsonDocument doc = QJsonDocument::fromJson(allStations.toUtf8());
    QJsonArray stationsArray = doc.object()["network"].toObject()["stations"].toArray();
    for (int i = 0; i < stationsArray.size(); ++i) {
        QJsonObject stationJson = stationsArray[i].toObject();
        Station* station = new Station();
        if (stationJson["extra"].toObject()["uid"].isString()) {
            stationIdRegex.indexIn(stationJson["extra"].toObject()["uid"].toString());
            station->number = stationIdRegex.cap(1).toInt();
        }
        else {
            station->number = stationJson["extra"].toObject()["uid"].toInt();
        }
        station->name = stationJson["name"].toString();
        QGeoCoordinate coord(stationJson["latitude"].toDouble(), stationJson["longitude"].toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->available_bike_stands = stationJson["empty_slots"].toInt();
            station->available_bikes = stationJson["free_bikes"].toInt();
            station->bike_stands = station->available_bikes + station->available_bike_stands;
            if (stationJson["extra"].toObject().contains("status")) {
                station->opened = stationJson["extra"].toObject()["status"].toString() == "OPEN";
            }
            if (stationJson["extra"].toObject().contains("last_updated")) {
                station->last_update = QDateTime::fromTime_t(stationJson["extra"].toObject()["last_updated"].toInt());
            }
        }
        else {
            station->bike_stands = -1;
            station->available_bike_stands = -1;
            station->available_bikes = -1;
        }
        stationsList.append(station);
    }
    return stationsList;
}
