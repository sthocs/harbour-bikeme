#ifndef PARSERVELOWAY_H
#define PARSERVELOWAY_H

#include "../bikedataparser.h"

class ParserVeloway : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

private:
    QString parseJsonResult(QString jsonResult);
};

#endif // PARSERVELOWAY_H
