#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QGeoCoordinate>

#include "parserlille.h"


QList<Station*> ParserLille::parseStationsList(QString allStationsDetails, bool withDetails)
{
    QList<Station*> stationsList;
    QJsonDocument doc = QJsonDocument::fromJson(allStationsDetails.toUtf8());
    QJsonObject jsonDoc = doc.object();
    QJsonArray stations = jsonDoc["records"].toArray();
    for (int i = 0; i < stations.size(); ++i) {
        QJsonObject stationJson = stations[i].toObject()["fields"].toObject();
        Station* station = new Station();
        station->number = stationJson["libelle"].toInt();
        station->name = stationJson["nom"].toString();
        station->address = stationJson["adresse"].toString();
        QJsonArray position = stationJson["geo"].toArray();
        QGeoCoordinate coord(position.at(0).toDouble(), position.at(1).toDouble());
        station->coordinates = coord;
        if (withDetails) {
            station->opened = stationJson["etat"].toString().compare("EN SERVICE") == 0;
            station->available_bike_stands = stationJson["nbplacesdispo"].toInt();
            station->available_bikes = stationJson["nbvelosdispo"].toInt();
            station->bike_stands = station->available_bike_stands + station->available_bikes;
            station->last_update = QDateTime::fromString(stations[i].toObject()["record_timestamp"].toString(), Qt::ISODate);
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
