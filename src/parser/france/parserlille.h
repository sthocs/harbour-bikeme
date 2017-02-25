#ifndef PARSERLILLE_H
#define PARSERLILLE_H

#include "../bikedataparser.h"

class ParserLille : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

    QList<Station*> parseAllStations(QString allStations, bool withDetails);
    void parseStationDetails2(QString stationDetails, Station* station);
};
Q_DECLARE_METATYPE(ParserLille)

#endif // PARSERLILLE_H
