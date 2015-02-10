#ifndef NICEJSONPARSER_H
#define NICEJSONPARSER_H

#include "../bikedataparser.h"

class NiceJsonParser : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

private:
    QString parseJsonResult(QString jsonResult);
};

#endif // NICEJSONPARSER_H
