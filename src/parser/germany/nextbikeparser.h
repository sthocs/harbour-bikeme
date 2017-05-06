#ifndef NEXTBIKEPARSER_H
#define NEXTBIKEPARSER_H

#include "../bikedataparser.h"

class NextbikeParser : public BikeDataParser
{
public:
    virtual QList<City*> parseCities(QString cities, ProviderInfo& providerInfo);
    virtual QList<Station*> parseStationsList(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(NextbikeParser)

#endif // NEXTBIKEPARSER_H
