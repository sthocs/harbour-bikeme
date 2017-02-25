#ifndef PARSERSMOOVE_H
#define PARSERSMOOVE_H

#include "../bikedataparser.h"

class ParserSmoove : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

    QList<Station*> parseAllStations(QString allStations, bool withDetails);

private:
    QString parseXmlResult(QString xmlResult);
};
Q_DECLARE_METATYPE(ParserSmoove)

#endif // PARSERSMOOVE_H
