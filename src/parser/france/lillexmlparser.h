#ifndef LILLEXMLPARSER_H
#define LILLEXMLPARSER_H

#include "../bikedataparser.h"

class LilleXmlParser : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);
};

#endif // LILLEXMLPARSER_H
