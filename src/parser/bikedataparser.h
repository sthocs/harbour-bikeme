#ifndef MASTERPARSER_H
#define MASTERPARSER_H

#include <QString>

#include "../city.h"

class BikeDataParser
{
public:
    virtual QString parseCarto(QString carto) = 0;
    virtual QString parseStationDetails(QString stationDetails, QString url) = 0;
    virtual QString parseAllStationsDetails(QString allStationsDetails) = 0;

    virtual QList<City*> parseCities(QString cities) { return QList<City*>(); }
};

#endif // MASTERPARSER_H
