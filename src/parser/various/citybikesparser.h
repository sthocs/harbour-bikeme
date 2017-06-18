#ifndef CITYBIKESPARSER_H
#define CITYBIKESPARSER_H

#include "../bikedataparser.h"

class CityBikesParser : public BikeDataParser
{
public:
    virtual QList<City*> parseCities(QString cities, ProviderInfo& providerInfo);
    virtual QList<Station*> parseStationsList(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(CityBikesParser)

#endif // CITYBIKESPARSER_H
