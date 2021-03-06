#include "parsersregistry.h"

ParsersRegistry::ParsersRegistry()
{
}

void ParsersRegistry::registerParsers()
{
    qRegisterMetaType<JCDecauxParser>("JCDecaux");
    qRegisterMetaType<ParserSmovengo>("Smovengo");
    qRegisterMetaType<NextbikeParser>("Nextbike");
    qRegisterMetaType<ParserVeloway>("Veloway");
    qRegisterMetaType<ParserLille>("Transpole");
    qRegisterMetaType<RennesParser>("RennesMetropole");
    qRegisterMetaType<ParserSmoove>("Smoove");
    qRegisterMetaType<ParserLondon>("TfL");
    qRegisterMetaType<MontrealParser>("Bixi");
    qRegisterMetaType<ParserMoscow>("Velobike");
    qRegisterMetaType<ParserAstana>("VelobikeKz");
    qRegisterMetaType<NabsaParser>("NABSA");
    qRegisterMetaType<EcoBiciParser>("EcoBici");
    qRegisterMetaType<HslParser>("HSL");
    qRegisterMetaType<CityBikesParser>("CityBikes");
}
