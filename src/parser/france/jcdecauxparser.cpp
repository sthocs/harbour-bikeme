#include "jcdecauxparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QString JCDecauxParser::parseCarto(QString carto) {
    return carto;
}

QString JCDecauxParser::parseStationDetails(QString stationDetails, QString url) {
    return stationDetails;
}

QString JCDecauxParser::parseAllStationsDetails(QString allStationsDetails) {
    return allStationsDetails;
}

QList<City*> JCDecauxParser::parseCities(QString cities) {
    QList<City*> citiesList;
    QJsonDocument doc = QJsonDocument::fromJson(cities.toUtf8());
    QJsonArray citiesArray = doc.array();
    for (int i = 0; i < citiesArray.size(); ++i) {
        QJsonObject cityJson = citiesArray[i].toObject();
        CityInfo info;
        info.name = cityJson["name"].toString();
        info.commercialName = cityJson["commercial_name"].toString();
        info.countryCode = cityJson["country_code"].toString();
        City* city = new City();
        city->setInfo(info);
        citiesList.append(city);
    }
    return citiesList;
}
