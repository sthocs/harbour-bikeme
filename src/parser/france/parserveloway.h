#ifndef PARSERVELOWAY_H
#define PARSERVELOWAY_H

#include "../bikedataparser.h"

class ParserVeloway : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

    QList<Station*> parseAllStations(QString allStations, bool withDetails);

private:
    QString parseJsonResult(QString jsonResult);
};
Q_DECLARE_METATYPE(ParserVeloway)

#endif // PARSERVELOWAY_H
