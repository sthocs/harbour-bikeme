#ifndef BIKEDATAPARSERFACTORY_H
#define BIKEDATAPARSERFACTORY_H

#include "bikedataparser.h"

class BikeDataParserFactory
{
public:
    BikeDataParserFactory();

    BikeDataParser* getBikeDataParser(QString city);
};

#endif // BIKEDATAPARSERFACTORY_H
