#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QGeoCoordinate>

#include "hslparser.h"

QList<Station*> HslParser::parseStationsList(QString allStations, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStations.toUtf8());
    QJsonArray stationsArray = doc.object()["stations"].toArray();
    for (int i = 0; i < stationsArray.size(); ++i) {
        QJsonObject stationJson = stationsArray[i].toObject();
        Station* station = new Station();
        station->number = stationJson["id"].toString().toInt();
        station->name = stationJson["name"].toString();
        QGeoCoordinate coord(stationJson["y"].toDouble(),
                stationJson["x"].toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->opened = stationJson["allowDropoff"].toBool();
            station->available_bikes = stationJson["bikesAvailable"].toInt();
            station->available_bike_stands = stationJson["spacesAvailable"].toInt();
            station->bike_stands = station->available_bikes + station->available_bike_stands;
        }
        stationsList.append(station);
    }
    return stationsList;
}
