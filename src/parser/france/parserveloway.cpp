#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QGeoCoordinate>

#include "parserveloway.h"


QList<Station*> ParserVeloway::parseStationsList(QString allStations, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStations.toUtf8());
    QJsonObject jsonDoc = doc.object();
    QJsonArray stations = jsonDoc["stand"].toArray();
    for (int i = 0; i < stations.size(); ++i) {
        Station* station = new Station();
        QJsonObject stationJson = stations[i].toObject();
        station->number = stationJson["id"].toString().toInt();
        QString wcom = stationJson["wcom"].toString();
        station->name = !wcom.isEmpty() ? wcom : stationJson["name"].toString();
        QGeoCoordinate coord(stationJson["lat"].toString().toDouble(),
                stationJson["lng"].toString().toDouble());
        station->coordinates = coord;
        station->opened = stationJson["disp"].toString() == "1";
        if (withDetails) {
            station->available_bikes = stationJson["ab"].toString().toInt();
            station->available_bike_stands = stationJson["ap"].toString().toInt();
        }
        stationsList.append(station);
    }
    return stationsList;
}
