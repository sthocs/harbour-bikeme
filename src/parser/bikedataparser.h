#ifndef MASTERPARSER_H
#define MASTERPARSER_H

#include <QString>

class BikeDataParser
{
public:
    virtual QString parseCarto(QString carto) = 0;
    virtual QString parseStationDetails(QString stationDetails, QString url) = 0;
    virtual QString parseAllStationsDetails(QString allStationsDetails) = 0;
};

#endif // MASTERPARSER_H
