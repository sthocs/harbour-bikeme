#ifndef RENNESPARSER_H
#define RENNESPARSER_H

#include "../bikedataparser.h"

class RennesParser : public BikeDataParser
{
public:
    QList<Station*> parseStationsList(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(RennesParser)

#endif // RENNESPARSER_H
