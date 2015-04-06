#include <QDebug>

#include "staticdataprovider.h"
#include "src/parser/bikedataparserfactory.h"

StaticDataProvider::StaticDataProvider()
{
    _jcdecauxApiKey = "";

    CityData lille("Lille", "V'Lille", "FR",
                   "http://vlille.fr/stations/xml-stations.aspx",
                   NULL, "http://vlille.fr/stations/xml-station.aspx?borne=${number}",
                   BikeDataParserFactory::getLilleParser);
    _citiesData["Lille"] = lille;
    CityData nice("Nice", "Vélo Bleu", "FR",
                  "http://www.velobleu.org/cartoV2/libProxyCarto.asp",
                  "http://www.velobleu.org/cartoV2/libProxyCarto.asp", NULL,
                  BikeDataParserFactory::getVelowayParser);
    _citiesData["Nice"] = nice;
    CityData london("London", "Barclays Cycle Hire", "UK",
                    "http://www.tfl.gov.uk/tfl/syndication/feeds/cycle-hire/livecyclehireupdates.xml",
                    "http://www.tfl.gov.uk/tfl/syndication/feeds/cycle-hire/livecyclehireupdates.xml", NULL,
                    BikeDataParserFactory::getLondonParser);
    _citiesData["London"] = london;

    // Smoove systems
    CityData montpellier("Montpellier", "Vélomagg'", "FR",
                         "http://cli-velo-montpellier.gir.fr/tvcstations.xml",
                         "http://cli-velo-montpellier.gir.fr/tvcstations.xml",
                         NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Montpellier"] = montpellier;
    CityData lorient("Lorient", "Vélo", "FR",
                     "http://www.lorient-velo.fr/tvcstations.xml", "http://www.lorient-velo.fr/tvcstations.xml",
                     NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Lorient"] = lorient;
    CityData grand_chalon("Grand Chalon", "Réflex", "FR",
                          "http://cli-velo-chalon.gir.fr/tvcstations.xml",
                          "http://cli-velo-chalon.gir.fr/tvcstations.xml",
                          NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Grand Chalon"] = grand_chalon;
    CityData avignon("Avignon", "Vélopop", "FR",
                     "http://www.velopop.fr/tvcstations.xml", "http://www.velopop.fr/tvcstations.xml",
                     NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Avignon"] = avignon;
    CityData saint_etienne("Saint-Etienne", "VéliVert", "FR",
               "http://www.velivert.fr/tvcstations.xml", "http://www.velivert.fr/tvcstations.xml",
               NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Saint-Etienne"] = saint_etienne;
    CityData valence("Valence", "Libélo", "FR",
                     "http://www.velo-libelo.fr/tvcstations.xml", "http://www.velo-libelo.fr/tvcstations.xml",
                     NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Valence"] = valence;
    CityData strasbourg("Strasbourg", "Vélhop", "FR",
                        "http://velhop.strasbourg.eu/tvcstations.xml", "http://velhop.strasbourg.eu/tvcstations.xml",
                        NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Strasbourg"] = strasbourg;
    CityData belfort("Belfort", "", "FR",
               "http://cli-velo-belfort.gir.fr/tvcstations.xml", "http://cli-velo-belfort.gir.fr/tvcstations.xml",
               NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Belfort"] = belfort;
    CityData clermont_ferrand("Clermont-Ferrand", "C.vélo", "FR",
                              "http://cli-velo-clermont.gir.fr/tvcstations.xml",
                              "http://cli-velo-clermont.gir.fr/tvcstations.xml",
                              NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Clermont-Ferrand"] = clermont_ferrand;
    CityData grenoble("Grenoble", "Métrovélo", "FR",
                      "http://vms.metrovelo.fr/tvcstations.xml", "http://vms.metrovelo.fr/tvcstations.xml",
                      NULL, BikeDataParserFactory::getSmooveParser);
    _citiesData["Grenoble"] = grenoble;
    CityData moscow("Moscow", "Velobike", "RU",
                    "http://velobike.ru/proxy/parkings/", "http://velobike.ru/proxy/parkings/",
                    NULL, BikeDataParserFactory::getMoscowParser);
    _citiesData["Moscow"] = moscow;
    CityData astana("Astana", "Astana Bike", "KZ",
                    "http://www.velobike.kz/rest/app.php/stations", "http://www.velobike.kz/rest/app.php/stations",
                    NULL, BikeDataParserFactory::getAstanaParser);
    _citiesData["Astana"] = astana;
}

QHash<QString, CityData> StaticDataProvider::getCitiesData()
{
    return _citiesData;
}

QString StaticDataProvider::getUrlForJCDecauxContracts()
{
    // https://developer.jcdecaux.com/rest/vls/contracts
    return "https://api.jcdecaux.com/vls/v1/contracts?apiKey=" + _jcdecauxApiKey;
}

QString StaticDataProvider::getCartoUrl(QString city)
{
    if (_citiesData.contains(city)) {
        return _citiesData.value(city).getUrlCarto();
    }
    return "https://developer.jcdecaux.com/rest/vls/stations/" + city + ".json";
}

QString StaticDataProvider::getAllStationsDetailsUrl(QString city)
{
    if (_citiesData.contains(city)) {
        QString url = _citiesData.value(city).getUrlAllStationsDetails();
        qDebug() << "all stations for" << city << url;
        return url != NULL ? url.replace("${city}", city) : NULL;
    }
    return "https://api.jcdecaux.com/vls/v1/stations?contract=" + city + "&apiKey=" + _jcdecauxApiKey;
}

QString StaticDataProvider::getStationDetailsUrl(QString city, QString stationNumber)
{
    if (_citiesData.contains(city)) {
        QString url = _citiesData.value(city).getUrlStationDetails();
        qDebug() << "station details for " << city << url;
        return url != NULL ? url.replace("${city}", city).replace("${number}", stationNumber) : NULL;
    }
    return "https://api.jcdecaux.com/vls/v1/stations/" + stationNumber +
            "?contract=" + city + "&apiKey=" + _jcdecauxApiKey;
}

QString StaticDataProvider::getCopyright(QString city)
{
    if (city.compare("London") == 0) {
        return "Powered by TfL Open Data";
    }
    return "";
}

BikeDataParser* StaticDataProvider::getBikeDataParser(QString city)
{
    if (_citiesData.contains(city)) {
        CityData tmp = _citiesData.value(city);
        return tmp.getBikeDataParser();
    }
    return BikeDataParserFactory::getJCDecauxParser();
}
