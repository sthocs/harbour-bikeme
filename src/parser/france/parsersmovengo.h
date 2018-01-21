#ifndef PARSERSMOVENGO_H
#define PARSERSMOVENGO_H


#include "../bikedataparser.h"

class ParserSmovengo : public BikeDataParser
{
public:
    QList<Station*> parseStationsList(QString allStationsDetails, bool withDetails);
};
Q_DECLARE_METATYPE(ParserSmovengo)

#endif // PARSERSMOVENGO_H
