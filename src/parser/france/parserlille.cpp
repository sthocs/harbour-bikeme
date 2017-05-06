#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QGeoCoordinate>

#include "parserlille.h"


QList<Station*> ParserLille::parseStationsList(QString allStationsDetails, bool withDetails)
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

void ParserLille::parseStationDetails(QString stationDetails, Station* station)
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
