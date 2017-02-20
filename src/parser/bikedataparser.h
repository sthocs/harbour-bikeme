#ifndef MASTERPARSER_H
#define MASTERPARSER_H

#include <QString>

#include "../city.h"
#include "../station.h"

class BikeDataParser
{
public:
    virtual QString parseCarto(QString carto) = 0;
    virtual QString parseStationDetails(QString stationDetails, QString url) = 0;
    virtual QString parseAllStationsDetails(QString allStationsDetails) = 0;

    virtual QList<City*> parseCities(QString cities, ProviderInfo providerInfo) { return QList<City*>(); }
    virtual QList<Station*> parseAllStations(QString allStations, bool withDetails) { return QList<Station*>(); }
    virtual void parseStationDetails2(QString stationDetails, Station* station) {}
};

#endif // MASTERPARSER_H
