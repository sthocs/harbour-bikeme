#ifndef PARSERMOSCOW_H
#define PARSERMOSCOW_H

#include "../bikedataparser.h"

class ParserMoscow : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

private:
    QString parseJsonResult(QString jsonResult);
};

#endif // PARSERMOSCOW_H
