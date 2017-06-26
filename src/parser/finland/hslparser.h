#ifndef HSLPARSER_H
#define HSLPARSER_H

#include "../bikedataparser.h"

class HslParser : public BikeDataParser
{
public:
    QList<Station*> parseStationsList(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(HslParser)

#endif // HSLPARSER_H
