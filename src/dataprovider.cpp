#include <QDebug>

#include "dataprovider.h"

DataProvider::DataProvider()
{
    _jcdecauxApiKey = "changeme";

    CityData lille("Lille", "V'Lille", "FR",
                   "http://vlille.fr/stations/xml-stations.aspx",
                   NULL, "http://vlille.fr/stations/xml-station.aspx?borne=${number}");
    _citiesData["Lille"] = lille;
    CityData london("London", "Barclays Cycle Hire", "UK",
                    "http://www.tfl.gov.uk/tfl/syndication/feeds/cycle-hire/livecyclehireupdates.xml",
                    "http://www.tfl.gov.uk/tfl/syndication/feeds/cycle-hire/livecyclehireupdates.xml", NULL);
    _citiesData["London"] = london;
}

QHash<QString, CityData> DataProvider::getCitiesData()
{
    return _citiesData;
}

QString DataProvider::getUrlForJCDecauxContracts()
{
    // https://developer.jcdecaux.com/rest/vls/contracts
    return "https://api.jcdecaux.com/vls/v1/contracts?apiKey=" + _jcdecauxApiKey;
}

QString DataProvider::getCartoUrl(QString city)
{
    if (_citiesData.contains(city)) {
        return _citiesData.value(city).getUrlCarto();
    }
    return "https://developer.jcdecaux.com/rest/vls/stations/" + city + ".json";
}

QString DataProvider::getAllStationsDetailsUrl(QString city)
{
    if (_citiesData.contains(city)) {
        QString url = _citiesData.value(city).getUrlAllStationsDetails();
        qDebug() << "all stations for" << city << url;
        return url != NULL ? url.replace("${city}", city) : NULL;
    }
    return "https://api.jcdecaux.com/vls/v1/stations?contract=" + city + "&apiKey=" + _jcdecauxApiKey;
}

QString DataProvider::getStationDetailsUrl(QString city, QString stationNumber)
{
    if (_citiesData.contains(city)) {
        QString url = _citiesData.value(city).getUrlStationDetails();
        qDebug() << "station details for " << city << url;
        return url != NULL ? url.replace("${city}", city).replace("${number}", stationNumber) : NULL;
    }
    return "https://api.jcdecaux.com/vls/v1/stations/" + stationNumber +
            "?contract=" + city + "&apiKey=" + _jcdecauxApiKey;
}

QString DataProvider::getCopyright(QString city)
{
    if (city.compare("London") == 0) {
        return "Powered by TfL Open Data";
    }
    return "";
}
