#include "bikedataparserfactory.h"

#include "france/jcdecauxparser.h"
#include "france/lillexmlparser.h"

BikeDataParserFactory::BikeDataParserFactory()
{
}

BikeDataParser* BikeDataParserFactory::getBikeDataParser(QString city)
{
    if (city.compare("Lille") == 0) {
        return new LilleXmlParser();
    }
    return new JCDecauxParser();
}
