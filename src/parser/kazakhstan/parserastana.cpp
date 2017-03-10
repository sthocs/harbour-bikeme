#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QGeoCoordinate>

#include "parserastana.h"


QList<Station*> ParserAstana::parseAllStations(QString allStations, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStations.toUtf8());
    QJsonObject jsonDoc = doc.object();
    QJsonArray stations = jsonDoc["data"].toArray();
    for (int i = 0; i < stations.size(); ++i) {
        Station* station = new Station();
        QJsonObject stationJson = stations[i].toObject();
        station->number = stationJson["id"].toInt();
        station->name = stationJson["name"].toString();
        QGeoCoordinate coord(stationJson["lat"].toString().toDouble(),
                stationJson["lng"].toString().toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->available_bikes = stationJson["avl_bikes"].toInt();
            station->available_bike_stands = stationJson["free_slots"].toInt();
        }
        stationsList.append(station);
    }
    return stationsList;
}
