#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "montrealparser.h"

// See http://donnees.ville.montreal.qc.ca/dataset/bixi-etat-des-stations/resource/b1ddc812-f5c8-48e8-94c5-57efb12ddd6a
QList<Station*> MontrealParser::parseAllStations(QString allStations, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStations.toUtf8());
    QJsonArray stationsArray = doc.object()["stations"].toArray();
    for (int i = 0; i < stationsArray.size(); ++i) {
        QJsonObject stationJson = stationsArray[i].toObject();
        Station* station = new Station();
        station->number = stationJson["id"].toInt();
        station->name = stationJson["s"].toString();
        QGeoCoordinate coord(stationJson["la"].toDouble(), stationJson["lo"].toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->opened = !stationJson["b"].toBool() && !stationJson["su"].toBool();
            station->bike_stands = stationJson["da"].toInt() + stationJson["dx"].toInt();
            station->available_bike_stands = stationJson["da"].toInt();
            station->available_bikes = stationJson["ba"].toInt();
            station->last_update = QDateTime::fromMSecsSinceEpoch(stationJson["lu"].toDouble());
        }
        stationsList.append(station);
    }
    return stationsList;
}
