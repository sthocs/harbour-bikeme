#include "parsersregistry.h"

ParsersRegistry::ParsersRegistry()
{
}

void ParsersRegistry::registerParsers()
{
    qRegisterMetaType<JCDecauxParser>("JCDecaux");
    qRegisterMetaType<ParserLille>("Transpole");
    qRegisterMetaType<ParserSmoove>("Smoove");
    qRegisterMetaType<ParserMoscow>("Velobike");
}
