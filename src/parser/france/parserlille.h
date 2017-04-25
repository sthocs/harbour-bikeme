#ifndef PARSERLILLE_H
#define PARSERLILLE_H

#include "../bikedataparser.h"

class ParserLille : public BikeDataParser
{
public:
    QList<Station*> parseStationsList(QString allStations, bool withDetails);
    void parseStationDetails(QString stationDetails, Station* station);
};
Q_DECLARE_METATYPE(ParserLille)

#endif // PARSERLILLE_H
