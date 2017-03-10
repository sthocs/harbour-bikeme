#ifndef PARSERLONDON_H
#define PARSERLONDON_H

#include "../bikedataparser.h"

class ParserLondon : public BikeDataParser
{
public:
    QList<Station*> parseAllStations(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(ParserLondon)

#endif // PARSERLONDON_H
