#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QString>
#include <QHash>
#include "citydata.h"

class DataProvider
{
public:
    DataProvider();

    QHash<QString, CityData> getCitiesData();
    QString getUrlForJCDecauxContracts();
    QString getCartoUrl(QString city);
    QString getAllStationsDetailsUrl(QString city);
    QString getStationDetailsUrl(QString city, QString stationNumber);
    QString getCopyright(QString city);

private:
    QString _jcdecauxApiKey;
    QHash<QString, CityData> _citiesData;
};

#endif // DATAPROVIDER_H