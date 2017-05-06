#ifndef PARSERSMOOVE_H
#define PARSERSMOOVE_H

#include "../bikedataparser.h"

class ParserSmoove : public BikeDataParser
{
public:
    QList<Station*> parseStationsList(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(ParserSmoove)

#endif // PARSERSMOOVE_H
