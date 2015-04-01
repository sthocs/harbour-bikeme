#ifndef PARSERSMOOVE_H
#define PARSERSMOOVE_H

#include "../bikedataparser.h"

class ParserSmoove : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

private:
    QString parseXmlResult(QString xmlResult);
};

#endif // PARSERSMOOVE_H
