#ifndef PARSERVELOWAY_H
#define PARSERVELOWAY_H

#include "../bikedataparser.h"

class ParserVeloway : public BikeDataParser
{
public:
    QList<Station*> parseStationsList(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(ParserVeloway)

#endif // PARSERVELOWAY_H
