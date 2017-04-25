#include "nextbikeparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>


QList<City*> NextbikeParser::parseCities(QString cities, ProviderInfo& providerInfo)
{
    QList<City*> citiesList;
    QJsonDocument doc = QJsonDocument::fromJson(cities.toUtf8());
    QJsonArray countriesArray = doc.object()["countries"].toArray();
    for (int i = 0; i < countriesArray.size(); ++i) {
        /*if (!countriesArray[i].toObject()["show_free_racks"].toBool()) {
            continue;
        }*/
        QJsonArray citiesArray = countriesArray[i].toObject()["cities"].toArray();
        for (int j = 0; j < citiesArray.size(); ++j) {
            QJsonObject cityJson = citiesArray[j].toObject();
            CityInfo info;
            info.stationsDataModes = StationsListAndData;
            info.id = cityJson["uid"].toInt();
            info.name = cityJson["name"].toString();
            info.commercialName = countriesArray[i].toObject()["name"].toString();
            info.countryCode = countriesArray[i].toObject()["country"].toString();
            info.zoom = cityJson["zoom"].toInt();
            info.allStationsDetailsUrl = QUrl(providerInfo.allStationsDetailsUrl.arg(info.id));
            info.providerName = providerInfo.name;
            City* city = new City();
            city->setInfo(info);
            citiesList.append(city);
        }
    }
    return citiesList;
}

QList<Station*> NextbikeParser::parseStationsList(QString allStations, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStations.toUtf8());
    //TODO sometimes the API returns more than 1 country
    QJsonObject country = doc.object()["countries"].toArray()[0].toObject();
    //bool showFreeRacks = country["show_free_racks"].toBool();
    QJsonObject city = country["cities"].toArray()[0].toObject();
    QJsonArray stationsArray = city["places"].toArray();
    for (int i = 0; i < stationsArray.size(); ++i) {
        QJsonObject stationJson = stationsArray[i].toObject();
        Station* station = new Station();
        station->number = stationJson["uid"].toInt();
        station->name = stationJson["name"].toString();
        QGeoCoordinate coord(stationJson["lat"].toDouble(), stationJson["lng"].toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->bike_stands = stationJson["bike_racks"].toInt();
            station->available_bike_stands = stationJson["free_racks"].toInt();
            station->available_bikes = stationJson["bikes"].toInt();
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
