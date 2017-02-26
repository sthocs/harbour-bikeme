#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>
#include <QGeoCoordinate>

#include "parserlille.h"

QString ParserLille::parseCarto(QString xmlCarto) {
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

QString ParserLille::parseStationDetails(QString xmlStationDetails, QString url) {
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

QString ParserLille::parseAllStationsDetails(QString allStationsDetails) {
    return allStationsDetails;
}

QList<Station*> ParserLille::parseAllStations(QString allStationsDetails, bool withDetails)
{
    Q_UNUSED(withDetails);
    QList<Station*> stationsList;
    QDomDocument xmlDoc;
    xmlDoc.setContent(allStationsDetails);
    QDomElement markers = xmlDoc.documentElement();
    QDomNode attr = markers.firstChild();
    while (!attr.isNull())
    {
        Station* station = new Station();
        QDomElement e = attr.toElement();
        station->number = e.attribute("id").toFloat();
        station->name = e.attribute("name");
        QGeoCoordinate coord(e.attribute("lat").toFloat(), e.attribute("lng").toFloat());
        station->coordinates = coord;

        stationsList.append(station);

        attr = attr.nextSibling();
    }
    return stationsList;
}

void ParserLille::parseStationDetails2(QString stationDetails, Station* station)
{
    QDomDocument xmlDetails;
    xmlDetails.setContent(stationDetails);
    QDomElement stationElement = xmlDetails.documentElement();
    QDomNode attr = stationElement.firstChild();
    while (!attr.isNull())
    {
        QDomElement e = attr.toElement();
        if( e.tagName() == "adress" ) {
            station->address = e.text();
            station->name = e.text();
        }
        else if( e.tagName() == "status" ) {
            station->opened = e.text().compare("0") == 0;
        }
        else if( e.tagName() == "bikes" ) {
            station->available_bikes = e.text().toInt();
        }
        else if( e.tagName() == "attachs" ) {
            station->available_bike_stands = e.text().toInt();
        }
        else if( e.tagName() == "lastupd" ) {
            QRegExp seconds("(\\d+) secondes");
            int pos = seconds.indexIn(e.text());
            if (pos > -1) {
                QString secs = seconds.cap(1);
                QDateTime now = QDateTime::currentDateTime();
                QDateTime updated = now.addSecs(-secs.toInt());
                station->last_update = updated;
            }
        }
        attr = attr.nextSibling();
    }
}
