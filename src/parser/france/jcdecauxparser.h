#ifndef JCDECAUXPARSER_H
#define JCDECAUXPARSER_H

#include "../bikedataparser.h"

class JCDecauxParser : public BikeDataParser
{
public:
    QList<City*> parseCities(QString cities, ProviderInfo providerInfo);
    QList<Station*> parseAllStations(QString allStations, bool withDetails);
    void parseStationDetails(QString stationDetails, Station* station);
};
Q_DECLARE_METATYPE(JCDecauxParser)

#endif // JCDECAUXPARSER_H
