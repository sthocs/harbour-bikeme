#ifndef PARSERASTANA_H
#define PARSERASTANA_H

#include "../bikedataparser.h"

class ParserAstana : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

    QList<Station*> parseAllStations(QString allStations, bool withDetails);

private:
    QString parseJsonResult(QString jsonResult);
};
Q_DECLARE_METATYPE(ParserAstana)

#endif // PARSERASTANA_H
