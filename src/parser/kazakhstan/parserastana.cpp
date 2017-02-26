#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QGeoCoordinate>

#include "parserastana.h"

QString ParserAstana::parseCarto(QString carto)
{
    return parseJsonResult(carto);
}

QString ParserAstana::parseStationDetails(QString stationDetails, QString url)
{
    return stationDetails;
}

QString ParserAstana::parseAllStationsDetails(QString allStationsDetails)
{
    return parseJsonResult(allStationsDetails);
}

QString ParserAstana::parseJsonResult(QString jsonResult)
{
    QJsonDocument jsonAllStationDetails;
    QJsonArray jsonArray;

    QJsonDocument doc = QJsonDocument::fromJson(jsonResult.toUtf8());
    QJsonObject jsonDoc = doc.object();
    QJsonArray stations = jsonDoc["data"].toArray();
    for (int i = 0; i < stations.size(); ++i) {
        QJsonObject jsonStation;
        QJsonObject position;
        QJsonObject station = stations[i].toObject();
        jsonStation["number"] = station["id"].toInt();
        jsonStation["name"] = station["name"];
        position["lat"] = station["lat"].toString().toDouble();
        position["lng"] = station["lng"].toString().toDouble();
        jsonStation["position"] = position;
        jsonStation["available_bikes"] = station["avl_bikes"].toInt();
        jsonStation["available_bike_stands"] = station["free_slots"].toInt();
        jsonStation["last_update"] = -1;
        jsonArray.append(jsonStation);
    }
    jsonAllStationDetails.setArray(jsonArray);
    return jsonAllStationDetails.toJson(QJsonDocument::Compact);
}

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
