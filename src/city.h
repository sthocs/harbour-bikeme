#ifndef CITY_H
#define CITY_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

enum StationsDataMode {
    NoMode = 0,
    StationsListOnly = 1,
    RealTimeDataOnly = 2,
    StationsListAndData = 4,
    SingleStationData = 8
};
Q_DECLARE_FLAGS(StationsDataModes, StationsDataMode)
Q_DECLARE_OPERATORS_FOR_FLAGS(StationsDataModes)

struct ProviderInfo {
    QString name;
    QString autoDiscoveryUrl;
    QString url;
    QString allStationsDetailsUrl;
    QString singleStationDetailsUrlTemplate;
    StationsDataModes stationsDataModes;
};

struct CityInfo {
    CityInfo() : id(0), hasElectricBikes(false), zoom(0), copyright(QString()), stationsDataModes(NoMode) {}
    int id;
    QString id_str;
    QString name;
    QString commercialName;
    QString providerName;
    QString countryCode;
    bool hasElectricBikes;
    QUrl autoDiscoveryUrl;
    QUrl stationsInfoUrl;
    QUrl allStationsDetailsUrl;
    QUrl stationsStatusUrl;
    QString singleStationDetailsUrlTemplate;
    int zoom;
    QString copyright;
    StationsDataModes stationsDataModes;
};

class City : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString identifier READ identifier)
    Q_PROPERTY(QString providerName READ getProviderName NOTIFY providerNameChanged)
    Q_PROPERTY(QString countryCode READ getCountryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString commercialName READ getCommercialName NOTIFY commercialNameChanged)
    Q_PROPERTY(bool hasElectricBikes READ hasElectricBikes NOTIFY hasElectricBikesChanged)
    Q_PROPERTY(QUrl stationsListUrl READ getStationsListUrl NOTIFY stationsListUrlChanged)
    Q_PROPERTY(QUrl allStationsDetailsUrl READ getAllStationsDetailsUrl NOTIFY allStationsDetailsUrlChanged)
    Q_PROPERTY(QString singleStationDetailsUrlTemplate READ getSingleStationDetailsUrlTemplate NOTIFY singleStationDetailsUrlTemplateChanged)
    Q_PROPERTY(int zoom READ zoom)
    Q_PROPERTY(QString copyright READ copyright NOTIFY copyrightChanged)
public:
    explicit City(QObject *parent = 0);

    QString identifier() const { return _info.name + "_" + _info.countryCode + "_" + _info.commercialName; }
    QString getProviderName() const { return _info.providerName; }
    QString getName() const { return _info.name; }
    QString getCommercialName() const { return _info.commercialName; }
    QString getCountryCode() const { return _info.countryCode; }
    bool hasElectricBikes() const { return _info.hasElectricBikes; }
    QUrl getAutoDiscoveryUrl() const { return _info.autoDiscoveryUrl; }
    QUrl getStationsListUrl() const { return _info.stationsInfoUrl; }
    QUrl getAllStationsDetailsUrl() const { return _info.allStationsDetailsUrl; }
    QUrl getStationsStatusUrl() const { return _info.stationsStatusUrl; }
    QString getSingleStationDetailsUrlTemplate() const { return _info.singleStationDetailsUrlTemplate; }
    int zoom() const { return _info.zoom; }
    QString copyright() const { return _info.copyright; }
    StationsDataModes stationDataModes() const { return _info.stationsDataModes; }

    Q_INVOKABLE bool isSingleStationModeSupported() const { return _info.stationsDataModes.testFlag(SingleStationData); }
    Q_INVOKABLE bool isAllStationModeSupported() const { return _info.stationsDataModes.testFlag(StationsListAndData) || _info.stationsDataModes.testFlag(RealTimeDataOnly); }

    void setInfo(const CityInfo info) { _info = info; }
    void setAutoDiscoveryUrl(QUrl url) { _info.autoDiscoveryUrl = url; }
    void setStationsListUrl(QUrl url) { _info.stationsInfoUrl = url; }
    void setStationsStatusUrl(QUrl url) { _info.stationsStatusUrl = url; }
    void setAllStationsDetailsUrl(QUrl url) { _info.allStationsDetailsUrl = url; }

signals:
    void nameChanged();
    void commercialNameChanged();
    void countryCodeChanged();
    void providerNameChanged();
    void hasElectricBikesChanged();
    void stationsListUrlChanged();
    void allStationsDetailsUrlChanged();
    void singleStationDetailsUrlTemplateChanged();
    void copyrightChanged();

protected:
    CityInfo _info;
};

#endif // CITY_H
