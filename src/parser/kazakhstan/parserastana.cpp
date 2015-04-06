#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

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
