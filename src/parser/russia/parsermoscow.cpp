#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "parsermoscow.h"

QString ParserMoscow::parseCarto(QString carto)
{
    return parseJsonResult(carto);
}

QString ParserMoscow::parseStationDetails(QString stationDetails, QString url)
{
    return stationDetails;
}

QString ParserMoscow::parseAllStationsDetails(QString allStationsDetails)
{
    return parseJsonResult(allStationsDetails);
}

QString ParserMoscow::parseJsonResult(QString jsonResult)
{
    QJsonDocument jsonAllStationDetails;
    QJsonArray jsonArray;

    QJsonDocument doc = QJsonDocument::fromJson(jsonResult.toUtf8());
    QJsonObject jsonDoc = doc.object();
    QJsonArray stations = jsonDoc["Items"].toArray();
    for (int i = 0; i < stations.size(); ++i) {
        QJsonObject jsonStation;
        QJsonObject position;
        QJsonObject station = stations[i].toObject();
        jsonStation["number"] = station["Id"].toString().toInt();
        jsonStation["name"] = station["Address"];
        QJsonObject srcPosition = station["Position"].toObject();
        position["lat"] = srcPosition["Lat"].toDouble();
        position["lng"] = srcPosition["Lon"].toDouble();
        jsonStation["position"] = position;
        int freePlaces = 0;
        int totalPlaces = 0;
        if (!station["IsLocked"].toBool()) {
            freePlaces = station["FreePlaces"].toInt();
            totalPlaces = station["TotalPlaces"].toInt();
        }
        jsonStation["available_bikes"] = totalPlaces - freePlaces;
        jsonStation["available_bike_stands"] = freePlaces;
        jsonStation["last_update"] = -1;
        jsonArray.append(jsonStation);
    }
    jsonAllStationDetails.setArray(jsonArray);
    return jsonAllStationDetails.toJson(QJsonDocument::Compact);
}

