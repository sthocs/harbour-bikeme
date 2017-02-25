#include "parsersregistry.h"

ParsersRegistry::ParsersRegistry()
{
}

void ParsersRegistry::registerParsers()
{
    qRegisterMetaType<JCDecauxParser>("JCDecaux");
    qRegisterMetaType<ParserSmoove>("Smoove");
    qRegisterMetaType<ParserMoscow>("Velobike");
}
