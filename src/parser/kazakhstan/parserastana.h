#ifndef PARSERASTANA_H
#define PARSERASTANA_H

#include "../bikedataparser.h"

class ParserAstana : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

private:
    QString parseJsonResult(QString jsonResult);
};

#endif // PARSERASTANA_H
