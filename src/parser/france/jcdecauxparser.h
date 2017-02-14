#ifndef JCDECAUXPARSER_H
#define JCDECAUXPARSER_H

#include "../bikedataparser.h"
#include "../../citiesloader.h"

class JCDecauxParser : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

    QList<City*> parseCities(QString cities, ProviderInfo providerInfo);
    QList<Station*> parseAllStationsDetails2(QString allStationsDetails);
};
Q_DECLARE_METATYPE(JCDecauxParser)

#endif // JCDECAUXPARSER_H
