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
    int id;
    QString name;
    QString commercialName;
    QString providerName;
    QString countryCode;
    QUrl stationsInfoUrl;
    QUrl allStationsDetailsUrl;
    QString singleStationDetailsUrlTemplate;
    int zoom;
    QString copyright;
};

class City : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString identifier READ identifier)
    Q_PROPERTY(QString providerName READ getProviderName NOTIFY providerNameChanged)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QUrl stationsListUrl READ getStationsListUrl NOTIFY stationsListUrlChanged)
    Q_PROPERTY(QUrl allStationsDetailsUrl READ getAllStationsDetailsUrl NOTIFY allStationsDetailsUrlChanged)
    Q_PROPERTY(QString singleStationDetailsUrlTemplate READ getSingleStationDetailsUrlTemplate NOTIFY singleStationDetailsUrlTemplateChanged)
    Q_PROPERTY(int zoom READ zoom)
    Q_PROPERTY(QString copyright READ copyright NOTIFY copyrightChanged)
public:
    explicit City(QObject *parent = 0);

    QString identifier() const { return _info.name + "_" + _info.countryCode; }
    QString getProviderName() const { return _info.providerName; }
    QString getName() const { return _info.name; }
    QString getCommercialName() const { return _info.commercialName; }
    QString getCountryCode() const { return _info.countryCode; }
    QUrl getStationsListUrl() const { return _info.stationsInfoUrl; }
    QUrl getAllStationsDetailsUrl() const { return _info.allStationsDetailsUrl; }
    QString getSingleStationDetailsUrlTemplate() const { return _info.singleStationDetailsUrlTemplate; }
    int zoom() const { return _info.zoom; }
    QString copyright() const { return _info.copyright; }

    Q_INVOKABLE bool isSingleStationModeSupported() const { return !_info.singleStationDetailsUrlTemplate.isEmpty(); }
    Q_INVOKABLE bool isAllStationModeSupported() const { return !_info.allStationsDetailsUrl.isEmpty(); }

    void setInfo(const CityInfo info) { _info = info; }
    void setStationsListUrl(QUrl url) { _info.stationsInfoUrl = url; }
    void setAllStationsDetailsUrl(QUrl url) { _info.allStationsDetailsUrl = url; }

signals:
    void nameChanged();
    void providerNameChanged();
    void stationsListUrlChanged();
    void allStationsDetailsUrlChanged();
    void singleStationDetailsUrlTemplateChanged();
    void copyrightChanged();

protected:
    CityInfo _info;
};

#endif // CITY_H
