#include "citydata.h"

CityData::CityData() {}

CityData::CityData(QString name, QString commercialName, QString countryCode,
                   QString urlCarto, QString urlAllStationDetails, QString urlStationDetails,
                   BikeDataParser* (*ptr)())
{
    _name = name;
    _commercialName = commercialName;
    _countryCode = countryCode;
    _urlCarto = urlCarto;
    _urlAllStationsDetails = urlAllStationDetails;
    _urlStationDetails = urlStationDetails;
    _getParserFunction = ptr;
}

QString CityData::getName() const
{
    return _name;
}

QString CityData::getCommercialName() const
{
    return _commercialName;
}

QString CityData::getCountryCode() const
{
    return _countryCode;
}

QString CityData::getUrlCarto() const
{
    return _urlCarto;
}

QString CityData::getUrlAllStationsDetails() const
{
    return _urlAllStationsDetails;
}

QString CityData::getUrlStationDetails() const
{
    return _urlStationDetails;
}

BikeDataParser* CityData::getBikeDataParser()
{
    return _getParserFunction();
}
