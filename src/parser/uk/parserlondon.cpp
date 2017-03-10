#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QGeoCoordinate>

#include "parserlondon.h"


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
