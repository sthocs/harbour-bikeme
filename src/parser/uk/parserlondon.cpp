#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>
#include <QGeoCoordinate>

#include "parserlondon.h"

QString ParserLondon::parseCarto(QString xmlCarto) {
    return parseXmlResult(xmlCarto);
}

QString ParserLondon::parseStationDetails(QString xmlStationDetails, QString url) {
    Q_UNUSED(url);
    return xmlStationDetails;
}

QString ParserLondon::parseAllStationsDetails(QString allStationsDetails) {
    return parseXmlResult(allStationsDetails);
}

QString ParserLondon::parseXmlResult(QString xmlResult)
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

QList<Station*> ParserLondon::parseAllStations(QString allStations, bool withDetails)
{
    QList<Station*> stationsList;
    QDomDocument xmlDetails;
    xmlDetails.setContent(allStations);
    QDomElement stations = xmlDetails.documentElement();
    double lastUpdate = stations.attribute("lastUpdate").toDouble();
    QDomNode stationXml = stations.firstChild();
    while (!stationXml.isNull())
    {
        Station* station = new Station();
        QGeoCoordinate coord;
        QDomNode attr = stationXml.firstChild();
        while (!attr.isNull()) {
            QDomElement e = attr.toElement();
            if( e.tagName() == "id" ) {
                station->number = e.text().toInt();
            }
            else if( e.tagName() == "name" ) {
                station->name = e.text();
            }
            else if( e.tagName() == "lat" ) {
                coord.setLatitude(e.text().toFloat());
            }
            else if( e.tagName() == "long" ) {
                coord.setLongitude(e.text().toFloat());
            }
            else if( e.tagName() == "nbBikes" && withDetails ) {
                station->available_bikes = e.text().toInt();
            }
            else if( e.tagName() == "nbEmptyDocks" && withDetails ) {
                station->available_bike_stands = e.text().toInt();
            }
            else if (e.tagName() == "nbDocks") {
                station->bike_stands = e.text().toInt();
            }
            else if (e.tagName() == "locked") {
                station->opened = e.text() == "false";
            }
            attr = attr.nextSibling();
        }
        station->coordinates = coord;
        station->last_update = QDateTime::fromMSecsSinceEpoch(lastUpdate);
        stationsList.append(station);
        stationXml = stationXml.nextSibling();
    }
    return stationsList;
}
