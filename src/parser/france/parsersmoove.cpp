#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>

#include "parsersmoove.h"

QString ParserSmoove::parseCarto(QString xmlCarto) {
    return parseXmlResult(xmlCarto);
}

QString ParserSmoove::parseStationDetails(QString xmlStationDetails, QString url) {
    return xmlStationDetails;
}

QString ParserSmoove::parseAllStationsDetails(QString allStationsDetails) {
    return parseXmlResult(allStationsDetails);
}

QString ParserSmoove::parseXmlResult(QString xmlResult)
{
    QJsonDocument jsonAllStationDetails;
    QJsonArray jsonArray;

    QDomDocument xmlDetails;
    xmlDetails.setContent(xmlResult);
    QDomElement vcs = xmlDetails.documentElement();
    QDomNode sl = vcs.firstChild();
    QDomNode si_node = sl.firstChild();
    while (!si_node.isNull())
    {
        QDomElement si = si_node.toElement();
        QJsonObject jsonStation;
        QJsonObject position;

        jsonStation["number"] = si.attribute("id").toInt();
        jsonStation["name"] = si.attribute("na");
        position["lat"] = si.attribute("la").toFloat();
        position["lng"] = si.attribute("lg").toFloat();
        jsonStation["available_bikes"] = si.attribute("av").toInt();
        jsonStation["available_bike_stands"] = si.attribute("fr").toInt();

        jsonStation["position"] = position;
        jsonStation["last_update"] = -1;
        jsonArray.append(jsonStation);
        si_node = si_node.nextSibling();
    }
    jsonAllStationDetails.setArray(jsonArray);
    return jsonAllStationDetails.toJson(QJsonDocument::Compact);
}

QList<Station*> ParserSmoove::parseAllStations(QString allStationsDetails, bool withDetails)
{
    QList<Station*> stationsList;
    QDomDocument xmlDetails;
    xmlDetails.setContent(allStationsDetails);
    QDomElement vcs = xmlDetails.documentElement();
    QDomNode sl = vcs.firstChild();
    QDomNode si_node = sl.firstChild();
    while (!si_node.isNull())
    {
        QDomElement si = si_node.toElement();
        Station* station = new Station();
        station->number = si.attribute("id").toInt();
        station->name = si.attribute("na");
        QGeoCoordinate coord(si.attribute("la").toFloat(), si.attribute("lg").toFloat());
        station->coordinates = coord;
        station->opened = true;
        if (withDetails) {
            station->available_bike_stands = si.attribute("fr").toInt();
            station->available_bikes = si.attribute("av").toInt();
        }
        else {
            station->bike_stands = -1;
            station->available_bike_stands = -1;
            station->available_bikes = -1;
        }
        stationsList.append(station);
        si_node = si_node.nextSibling();
    }
    return stationsList;
}
