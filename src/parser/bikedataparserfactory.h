#ifndef BIKEDATAPARSERFACTORY_H
#define BIKEDATAPARSERFACTORY_H

#include "bikedataparser.h"

class BikeDataParserFactory
{
public:
    static BikeDataParser* getJCDecauxParser();
    static BikeDataParser* getLilleParser();
    static BikeDataParser* getLondonParser();
    static BikeDataParser* getVelowayParser();
    static BikeDataParser* getMoscowParser();
};

#endif // BIKEDATAPARSERFACTORY_H
