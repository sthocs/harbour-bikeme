#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#include "parsersmoove.h"


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
        stationsList.append(station);
        si_node = si_node.nextSibling();
    }
    return stationsList;
}
