#ifndef ECOBICIPARSER_H
#define ECOBICIPARSER_H

#include "../bikedataparser.h"

class EcoBiciParser : public BikeDataParser
{
public:
    QList<Station*> parseStationsList(QString allStations, bool withDetails);
};
Q_DECLARE_METATYPE(EcoBiciParser)

#endif // ECOBICIPARSER_H
