#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "nicejsonparser.h"

QString NiceJsonParser::parseCarto(QString carto)
{
    return parseJsonResult(carto);
}

QString NiceJsonParser::parseStationDetails(QString stationDetails, QString url)
{
    return stationDetails;
}

QString NiceJsonParser::parseAllStationsDetails(QString allStationsDetails)
{
    return parseJsonResult(allStationsDetails);
}

QString NiceJsonParser::parseJsonResult(QString jsonResult)
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
        jsonStation["name"] = station["wcom"];
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
