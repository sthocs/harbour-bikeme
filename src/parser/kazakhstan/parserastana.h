#ifndef PARSERASTANA_H
#define PARSERASTANA_H

#include "../bikedataparser.h"

class ParserAstana : public BikeDataParser
{
public:
    QList<Station*> parseStationsList(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(ParserAstana)

#endif // PARSERASTANA_H
