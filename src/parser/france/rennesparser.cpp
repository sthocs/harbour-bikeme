#include "rennesparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>
#include <QDebug>


QList<Station*> RennesParser::parseStationsList(QString allStationsDetails, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStationsDetails.toUtf8());
    QJsonObject jsonDoc = doc.object();
    QJsonArray stations = jsonDoc["records"].toArray();
    for (int i = 0; i < stations.size(); ++i) {
        QJsonObject stationJson = stations[i].toObject()["fields"].toObject();
        Station* station = new Station();
        station->number = stationJson["idstation"].toString().toInt();
        station->name = stationJson["nom"].toString();
        QJsonArray position = stationJson["coordonnees"].toArray();
        QGeoCoordinate coord(position.at(0).toDouble(), position.at(1).toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->opened = stationJson["etat"].toString().compare("En fonctionnement") == 0;
            station->available_bike_stands = stationJson["nombreemplacementsdisponibles"].toInt();
            station->available_bikes = stationJson["nombrevelosdisponibles"].toInt();
            station->bike_stands = station->available_bike_stands + station->available_bikes;
            station->last_update = QDateTime::fromString(stationJson["lastupdate"].toString(), Qt::ISODate);
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
