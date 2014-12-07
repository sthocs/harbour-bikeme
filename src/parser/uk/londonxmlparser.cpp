#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>

#include "londonxmlparser.h"

QString LondonXmlParser::parseCarto(QString xmlCarto) {
    return parseXmlResult(xmlCarto);
}

QString LondonXmlParser::parseStationDetails(QString xmlStationDetails, QString url) {
    return xmlStationDetails;
}

QString LondonXmlParser::parseAllStationsDetails(QString allStationsDetails) {
    return parseXmlResult(allStationsDetails);
}

QString LondonXmlParser::parseXmlResult(QString xmlResult)
{
    QJsonDocument jsonAllStationDetails;
    QJsonArray jsonArray;

    QDomDocument xmlDetails;
    xmlDetails.setContent(xmlResult);
    QDomElement stations = xmlDetails.documentElement();
    double lastUpdate = stations.attribute("lastUpdate").toDouble();
    QDomNode station = stations.firstChild();
    while (!station.isNull())
    {
        QJsonObject jsonStation;
        QJsonObject position;
        QDomNode attr = station.firstChild();
        while (!attr.isNull()) {
            QDomElement e = attr.toElement();
            if( e.tagName() == "id" ) {
                jsonStation["number"] = e.text().toInt();
            }
            else if( e.tagName() == "name" ) {
                jsonStation["name"] = e.text();
            }
            else if( e.tagName() == "lat" ) {
                position["lat"] = e.text().toFloat();
            }
            else if( e.tagName() == "long" ) {
                position["lng"] = e.text().toFloat();
            }
            else if( e.tagName() == "nbBikes" ) {
                jsonStation["available_bikes"] = e.text().toInt();
            }
            else if( e.tagName() == "nbEmptyDocks" ) {
                jsonStation["available_bike_stands"] = e.text().toInt();
            }
            attr = attr.nextSibling();
        }
        jsonStation["position"] = position;
        jsonStation["last_update"] = lastUpdate;
        jsonArray.append(jsonStation);
        station = station.nextSibling();
    }
    jsonAllStationDetails.setArray(jsonArray);
    return jsonAllStationDetails.toJson(QJsonDocument::Compact);
}
