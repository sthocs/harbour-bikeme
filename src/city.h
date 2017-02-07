#ifndef CITY_H
#define CITY_H

#include <QObject>
#include <QUrl>

struct CityInfo {
    QString name;
    QString commercialName;
    QString countryCode;
    QUrl stationsInfoUrl;
    QUrl allStationsDetailsUrl;
    QUrl singleStationDetailsUrl;
};

class City : public QObject
{
    Q_OBJECT
public:
    explicit City(QObject *parent = 0);

    QString getName() const { return _info.name; }
    QString getCommercialName() const { return _info.commercialName; }
    QString getCountryCode() const { return _info.countryCode; }
    QUrl getStationsInfoUrl() const { return _info.stationsInfoUrl; }
    QUrl getAllStationsDetailsUrl() const { return _info.allStationsDetailsUrl; }
    QUrl getSingleStationDetailsUrl() const { return _info.singleStationDetailsUrl; }

    void setInfo(const CityInfo info) { _info = info; }

    void fetchStationsInfo();
    void fetchBikesAvailabilityAtStation(int stationId);
    void fetchBikesAvailability();

signals:

public slots:

protected:
    CityInfo _info;
};

#endif // CITY_H
