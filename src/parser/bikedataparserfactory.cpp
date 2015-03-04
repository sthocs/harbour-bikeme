#include "bikedataparserfactory.h"

#include "france/jcdecauxparser.h"
#include "france/parserlille.h"
#include "france/parserveloway.h"
#include "uk/parserlondon.h"
#include "russia/parsermoscow.h"


BikeDataParser* BikeDataParserFactory::getJCDecauxParser()
{
   return new JCDecauxParser();
}

BikeDataParser* BikeDataParserFactory::getLilleParser()
{
    return new ParserLille();
}

BikeDataParser* BikeDataParserFactory::getLondonParser()
{
    return new ParserLondon();
}

BikeDataParser* BikeDataParserFactory::getVelowayParser()
{
    return new ParserVeloway();
}

BikeDataParser *BikeDataParserFactory::getMoscowParser()
{
    return new ParserMoscow();
}
