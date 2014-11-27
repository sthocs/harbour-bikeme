#ifndef CITYDATA_H
#define CITYDATA_H

#include <QString>

class CityData
{
public:
    CityData();
    CityData(QString name, QString commercialName, QString countryCode,
             QString urlCarto, QString urlAllStationDetails, QString urlStationDetails);

    QString getName() const;
    QString getCommercialName() const;
    QString getCountryCode() const;

    QString getUrlCarto() const;
    QString getUrlAllStationsDetails() const;
    QString getUrlStationDetails() const;

private:
    QString _name;
    QString _commercialName;
    QString _countryCode;

    QString _urlCarto;
    QString _urlAllStationsDetails;
    QString _urlStationDetails;
};

#endif // CITYDATA_H
