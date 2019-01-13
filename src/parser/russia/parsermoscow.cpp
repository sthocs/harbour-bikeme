#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QGeoCoordinate>

#include "parsermoscow.h"


QList<Station*> ParserMoscow::parseStationsList(QString allStations, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStations.toUtf8());
    QJsonArray stationsArray = doc.object()["Items"].toArray();
    for (int i = 0; i < stationsArray.size(); ++i) {
        QJsonObject stationJson = stationsArray[i].toObject();
        Station* station = new Station();
        station->number = stationJson["Id"].toString().toInt();
        station->name = stationJson["Address"].toString(); // stationJson["Name"].toString();
        station->address = stationJson["Address"].toString();
        QJsonObject position = stationJson["Position"].toObject();
        QGeoCoordinate coord(position["Lat"].toDouble(), position["Lon"].toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->opened = !stationJson["IsLocked"].toBool();
            station->bike_stands = stationJson["TotalPlaces"].toInt();
            station->available_bike_stands = stationJson["FreePlaces"].toInt();
            station->available_bikes = station->bike_stands - station->available_bike_stands;
        }
        stationsList.append(station);
    }
    return stationsList;
}
