#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>

#include "lillexmlparser.h"

QString LilleXmlParser::parseCarto(QString xmlCarto) {
    QJsonDocument jsonCarto;
    QJsonArray jsonArray;

    QDomDocument xmlDoc;
    xmlDoc.setContent(xmlCarto);
    QDomElement markers = xmlDoc.documentElement();
    QDomNode attr = markers.firstChild();
    while (!attr.isNull())
    {
        QJsonObject station;
        QJsonObject position;
        QDomElement e = attr.toElement();
        station["number"] = e.attribute("id").toFloat();
        position["lat"] = e.attribute("lat").toFloat();
        position["lng"] = e.attribute("lng").toFloat();
        station["position"] = position;
        station["name"] = e.attribute("name");

        jsonArray.append(station);

        attr = attr.nextSibling();
    }
    jsonCarto.setArray(jsonArray);
    return jsonCarto.toJson(QJsonDocument::Compact);
}

QString LilleXmlParser::parseStationDetails(QString xmlStationDetails, QString url) {
    QJsonDocument jsonStationDetails;
    QJsonObject jsonObject;
    
    QStringList parsed = url.split("=");
    jsonObject["number"] = parsed.last().toInt();

    QDomDocument xmlDetails;
    xmlDetails.setContent(xmlStationDetails);
    QDomElement station = xmlDetails.documentElement();
    QDomNode attr = station.firstChild();
    while (!attr.isNull())
    {
        QDomElement e = attr.toElement();
        if( e.tagName() == "adress" ) {
            jsonObject["address"] = e.text();
            jsonObject["name"] = e.text();
        }
        else if( e.tagName() == "bikes" ) {
            jsonObject["available_bikes"] = e.text().toInt();
        }
        else if( e.tagName() == "attachs" ) {
            jsonObject["available_bike_stands"] = e.text().toInt();
        }
        else if( e.tagName() == "lastupd" ) {
            jsonObject["last_update"] = e.text();
        }

        attr = attr.nextSibling();
    }
    jsonStationDetails.setObject(jsonObject);
    return jsonStationDetails.toJson(QJsonDocument::Compact);
}

QString LilleXmlParser::parseAllStationsDetails(QString allStationsDetails) {
    return allStationsDetails;
}
