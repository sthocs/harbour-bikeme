#ifndef LONDONXMLPARSER_H
#define LONDONXMLPARSER_H

#include "../bikedataparser.h"

class LondonXmlParser : public BikeDataParser
{
public:
    QString parseCarto(QString carto);
    QString parseStationDetails(QString stationDetails, QString url);
    QString parseAllStationsDetails(QString allStationsDetails);

private:
    QString parseXmlResult(QString xmlResult);
};

#endif // LONDONXMLPARSER_H
