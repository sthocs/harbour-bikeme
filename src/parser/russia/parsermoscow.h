#ifndef PARSERMOSCOW_H
#define PARSERMOSCOW_H

#include "../bikedataparser.h"

class ParserMoscow : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

    QList<Station*> parseAllStations(QString allStations, bool withDetails);

private:
    QString parseJsonResult(QString jsonResult);
};
Q_DECLARE_METATYPE(ParserMoscow)

#endif // PARSERMOSCOW_H
