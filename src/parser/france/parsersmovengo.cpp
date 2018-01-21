#include "parsersmovengo.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>


QList<Station*> ParserSmovengo::parseStationsList(QString allStationsDetails, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStationsDetails.toUtf8());
    QJsonArray stations = doc.array();
    for (int i = 0; i < stations.size(); ++i) {
        QJsonObject stationJson = stations[i].toObject()["station"].toObject();
        Station* station = new Station();
        station->number = stationJson["code"].toString().toInt();
        station->name = stationJson["name"].toString();
        QJsonObject position = stationJson["gps"].toObject();
        QGeoCoordinate coord(position["latitude"].toDouble(), position["longitude"].toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->opened = stationJson["state"].toString().compare("Operative") == 0;
            station->available_bike_stands = stations[i].toObject()["nbFreeDock"].toInt() +
                    stations[i].toObject()["nbFreeEDock"].toInt();
            station->available_bikes = stations[i].toObject()["nbBike"].toInt();
            station->available_electric_bikes = stations[i].toObject()["nbEbike"].toInt();
            station->bike_stands = station->available_bike_stands + station->available_bikes;
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
