#ifndef PARSERLILLE_H
#define PARSERLILLE_H

#include "../bikedataparser.h"

class ParserLille : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);
};

#endif // PARSERLILLE_H
