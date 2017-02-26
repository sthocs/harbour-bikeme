#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QGeoCoordinate>

#include "parserveloway.h"

QString ParserVeloway::parseCarto(QString carto)
{
    return parseJsonResult(carto);
}

QString ParserVeloway::parseStationDetails(QString stationDetails, QString url)
{
    return stationDetails;
}

QString ParserVeloway::parseAllStationsDetails(QString allStationsDetails)
{
    return parseJsonResult(allStationsDetails);
}

QString ParserVeloway::parseJsonResult(QString jsonResult)
{
    QJsonDocument jsonAllStationDetails;
    QJsonArray jsonArray;

    QJsonDocument doc = QJsonDocument::fromJson(jsonResult.toUtf8());
    QJsonObject jsonDoc = doc.object();
    QJsonArray stations = jsonDoc["stand"].toArray();
    for (int i = 0; i < stations.size(); ++i) {
        QJsonObject jsonStation;
        QJsonObject position;
        QJsonObject station = stations[i].toObject();
        jsonStation["number"] = station["id"].toString().toInt();
        jsonStation["name"] = station["name"];
        position["lat"] = station["lat"].toString().toDouble();
        position["lng"] = station["lng"].toString().toDouble();
        jsonStation["position"] = position;
        jsonStation["available_bikes"] = station["ab"].toString().toInt();
        jsonStation["available_bike_stands"] = station["ap"].toString().toInt();
        jsonStation["last_update"] = -1;
        jsonArray.append(jsonStation);
    }
    jsonAllStationDetails.setArray(jsonArray);
    return jsonAllStationDetails.toJson(QJsonDocument::Compact);
}

QList<Station*> ParserVeloway::parseAllStations(QString allStations, bool withDetails)
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
