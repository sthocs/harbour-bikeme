#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QGeoCoordinate>

#include "ecobiciparser.h"

QList<Station*> EcoBiciParser::parseStationsList(QString allStations, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStations.toUtf8());
    QJsonArray stationsArray = doc.array();
    for (int i = 0; i < stationsArray.size(); ++i) {
        QJsonObject stationJson = stationsArray[i].toObject();
        Station* station = new Station();
        station->number = stationJson["id"].toString().toInt();
        station->name = stationJson["name"].toString();
        station->address = stationJson["address"].toString();
        QGeoCoordinate coord(stationJson["lat"].toString().toDouble(),
                stationJson["lon"].toString().toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->opened = stationJson["status"].toString().compare("OPN") == 0;
            station->available_bikes = stationJson["bikes"].toString().toInt();
            station->available_bike_stands = stationJson["slots"].toString().toInt();
            station->bike_stands = station->available_bikes + station->available_bike_stands;
        }
        stationsList.append(station);
    }
    return stationsList;
}
