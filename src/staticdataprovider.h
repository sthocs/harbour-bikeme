#ifndef STATICDATAPROVIDER_H
#define STATICDATAPROVIDER_H

#include <QString>
#include <QHash>
#include "citydata.h"

class StaticDataProvider
{
public:
    StaticDataProvider();

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

#endif // STATICDATAPROVIDER_H
