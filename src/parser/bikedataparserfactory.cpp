#include "bikedataparserfactory.h"

#include "france/jcdecauxparser.h"
#include "france/parserlille.h"
#include "france/parserveloway.h"
#include "uk/parserlondon.h"

BikeDataParserFactory::BikeDataParserFactory()
{
}

BikeDataParser* BikeDataParserFactory::getBikeDataParser(QString city)
{
    if (city.compare("Lille") == 0) {
        return new ParserLille();
    }
    if (city.compare("Nice") == 0) {
        return new ParserVeloway();
    }
    else if (city.compare("London") == 0) {
        return new ParserLondon();
    }
    return new JCDecauxParser();
}
