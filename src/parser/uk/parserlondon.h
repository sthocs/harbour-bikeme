#ifndef PARSERLONDON_H
#define PARSERLONDON_H

#include "../bikedataparser.h"

class ParserLondon : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

    QList<Station*> parseAllStations(QString allStations, bool withDetails);

private:
    QString parseXmlResult(QString xmlResult);
};
Q_DECLARE_METATYPE(ParserLondon)

#endif // PARSERLONDON_H
