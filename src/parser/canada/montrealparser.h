#ifndef MONTREALPARSER_H
#define MONTREALPARSER_H

#include "../bikedataparser.h"

class MontrealParser : public BikeDataParser
{
public:
    QList<Station*> parseAllStations(QString allStations, bool withDetails);

};
Q_DECLARE_METATYPE(MontrealParser)

#endif // MONTREALPARSER_H
