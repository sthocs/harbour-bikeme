#ifndef CITY_H
#define CITY_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

struct ProviderInfo {
    QString name;
    QString url;
    QString allStationsDetailsUrl;
    QString singleStationDetailsUrlTemplate;
};

struct CityInfo {
    QString name;
    QString commercialName;
    QString providerName;
    QString countryCode;
    QUrl stationsInfoUrl;
    QUrl allStationsDetailsUrl;
    QString singleStationDetailsUrlTemplate;
};

class City : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString identifier READ identifier)
    Q_PROPERTY(QString providerName READ getProviderName NOTIFY providerNameChanged)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QUrl allStationsDetailsUrl READ getAllStationsDetailsUrl NOTIFY allStationsDetailsUrlChanged)
    Q_PROPERTY(QString singleStationDetailsUrlTemplate READ getSingleStationDetailsUrlTemplate NOTIFY singleStationDetailsUrlTemplateChanged)
public:
    explicit City(QObject *parent = 0);

    QString identifier() const { return _info.name + "_" + _info.countryCode; }
    QString getProviderName() const { return _info.providerName; }
    QString getName() const { return _info.name; }
    QString getCommercialName() const { return _info.commercialName; }
    QString getCountryCode() const { return _info.countryCode; }
    QUrl getStationsInfoUrl() const { return _info.stationsInfoUrl; }
    QUrl getAllStationsDetailsUrl() const { return _info.allStationsDetailsUrl; }
    QString getSingleStationDetailsUrlTemplate() const { return _info.singleStationDetailsUrlTemplate; }

    Q_INVOKABLE bool isSingleStationSupported() const { return !_info.singleStationDetailsUrlTemplate.isEmpty(); }

    void setInfo(const CityInfo info) { _info = info; }

signals:
    void nameChanged();
    void providerNameChanged();
    void allStationsDetailsUrlChanged();
    void singleStationDetailsUrlTemplateChanged();

protected:
    CityInfo _info;
};

#endif // CITY_H
