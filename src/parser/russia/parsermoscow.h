#ifndef PARSERMOSCOW_H
#define PARSERMOSCOW_H

#include "../bikedataparser.h"

class ParserMoscow : public BikeDataParser
{
public:
    QList<Station*> parseStationsList(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(ParserMoscow)

#endif // PARSERMOSCOW_H
