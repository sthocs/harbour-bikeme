#ifndef JCDECAUXPARSER_H
#define JCDECAUXPARSER_H

#include "../bikedataparser.h"

class JCDecauxParser : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);
};

#endif // JCDECAUXPARSER_H
