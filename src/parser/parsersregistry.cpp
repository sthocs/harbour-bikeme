#include "parsersregistry.h"

ParsersRegistry::ParsersRegistry()
{
}

void ParsersRegistry::registerParsers()
{
    qRegisterMetaType<JCDecauxParser>("JCDecaux");
    qRegisterMetaType<NextbikeParser>("Nextbike");
    qRegisterMetaType<ParserVeloway>("Veloway");
    qRegisterMetaType<ParserLille>("Transpole");
    qRegisterMetaType<ParserSmoove>("Smoove");
    qRegisterMetaType<ParserLondon>("TfL");
    qRegisterMetaType<ParserMoscow>("Velobike");
    qRegisterMetaType<ParserAstana>("VelobikeKz");
}
