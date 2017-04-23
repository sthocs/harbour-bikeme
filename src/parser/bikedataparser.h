#ifndef MASTERPARSER_H
#define MASTERPARSER_H

#include <QString>

#include "../city.h"
#include "../station.h"

class BikeDataParser
{
public:
    virtual ~BikeDataParser() {}

    virtual QList<City*> parseCities(QString /* cities */, ProviderInfo& /* providerInfo */) { return QList<City*>(); }
    virtual QList<Station*> parseAllStations(QString /* allStations */, bool /* withDetails */) { return QList<Station*>(); }
    virtual void parseStationDetails(QString /* stationDetails */, Station* /* station */) {}

    virtual void parseCityUrls(QString /* cityUrls */, City* /* city */) {}
    virtual QList<Station*> parseStationsList(QString stationsList ) { return parseAllStations(stationsList, false); }
    virtual void parseStationsRealTimeData(QString /* realTimeData */, QList<Station*> /* stations */) {}
};

#endif // MASTERPARSER_H
