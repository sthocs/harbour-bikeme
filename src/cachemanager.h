#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QNetworkConfigurationManager>
#include <QNetworkSession>

class CacheManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cartoJson READ cartoJson NOTIFY cartoChanged)
    Q_ENUMS(PendingAction)

public:
    CacheManager();
    QString cartoJson() const;

    Q_INVOKABLE QString getAppState();
    Q_INVOKABLE void saveAppState(QString);
    Q_INVOKABLE void getContracts(bool);
    Q_INVOKABLE void downloadCarto(QString);
    Q_INVOKABLE void getStationDetails(QString, QString);
    Q_INVOKABLE bool removeCacheDir();

    enum PendingAction {
        NONE, DOWNLOAD_STATIONS
    };

signals:
    void contractsUpdated(QString contracts);
    void cartoChanged();
    void networkStatusUpdated(bool connected);
    void gotStationDetails(QString stationDetails);

public slots:
    void getContractsFinished();
    void replyFinished();
    void stationDetailsFinished();

    void networkStatusChanged(bool);

private:
    QNetworkAccessManager* _networkAccessManager;
    QNetworkConfigurationManager* _networkConfigManager;

    QString _cartoJson;
    QString _currentCity;
    QString _cacheDir;

    PendingAction _pendingAction;

};

#endif // CACHEMANAGER_H
