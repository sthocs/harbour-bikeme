#include "bikedataparserfactory.h"

#include "france/jcdecauxparser.h"
#include "france/lillexmlparser.h"
#include "france/nicejsonparser.h"
#include "uk/londonxmlparser.h"

BikeDataParserFactory::BikeDataParserFactory()
{
}

BikeDataParser* BikeDataParserFactory::getBikeDataParser(QString city)
{
    if (city.compare("Lille") == 0) {
        return new LilleXmlParser();
    }
    if (city.compare("Nice") == 0) {
        return new NiceJsonParser();
    }
    else if (city.compare("London") == 0) {
        return new LondonXmlParser();
    }
    return new JCDecauxParser();
}
