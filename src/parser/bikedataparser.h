#ifndef MASTERPARSER_H
#define MASTERPARSER_H

#include <QString>

#include "../city.h"
#include "../station.h"

class BikeDataParser
{
public:
    virtual ~BikeDataParser() {}

    virtual QList<City*> parseCities(QString /* cities */, ProviderInfo /* providerInfo */) { return QList<City*>(); }
    virtual QList<Station*> parseAllStations(QString /* allStations */, bool /* withDetails */) { return QList<Station*>(); }
    virtual void parseStationDetails(QString /* stationDetails */, Station* /* station */) {}
};

#endif // MASTERPARSER_H
