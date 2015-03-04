#ifndef CITYDATA_H
#define CITYDATA_H

#include <QString>
#include "src/parser/bikedataparser.h"

class CityData
{
public:
    CityData();
    CityData(QString name, QString commercialName, QString countryCode,
             QString urlCarto, QString urlAllStationDetails, QString urlStationDetails,
             BikeDataParser* (*ptr)());

    QString getName() const;
    QString getCommercialName() const;
    QString getCountryCode() const;

    QString getUrlCarto() const;
    QString getUrlAllStationsDetails() const;
    QString getUrlStationDetails() const;

    BikeDataParser* getBikeDataParser();

private:
    QString _name;
    QString _commercialName;
    QString _countryCode;

    QString _urlCarto;
    QString _urlAllStationsDetails;
    QString _urlStationDetails;

    BikeDataParser* (*_getParserFunction)();
};

#endif // CITYDATA_H
