#ifndef PARSERLONDON_H
#define PARSERLONDON_H

#include "../bikedataparser.h"

class ParserLondon : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

private:
    QString parseXmlResult(QString xmlResult);
};

#endif // PARSERLONDON_H
