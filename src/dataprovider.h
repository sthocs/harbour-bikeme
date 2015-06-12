#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QNetworkConfigurationManager>
#include <QNetworkSession>

#include "staticdataprovider.h"
#include "parser/bikedataparser.h"

class DataProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cartoJson READ cartoJson NOTIFY cartoChanged)
    Q_ENUMS(PendingAction)

public:
    DataProvider();
    QString cartoJson() const;

    Q_INVOKABLE QString getAppState();
    Q_INVOKABLE void saveAppState(QString);
    Q_INVOKABLE void getContracts(bool);
    Q_INVOKABLE void downloadCarto(QString);
    Q_INVOKABLE void getStationDetails(QString, QString);
    Q_INVOKABLE void downloadAllStationsDetails(QString);
    Q_INVOKABLE bool removeCacheFiles();

    Q_INVOKABLE bool isGetSingleStationDataSupported(QString);
    Q_INVOKABLE bool isGetAllStationsDataSupported(QString);

    Q_INVOKABLE QString getCopyright(QString);

    enum PendingAction {
        NONE, DOWNLOAD_STATIONS
    };

signals:
    void contractsUpdated(QString contracts);
    void cartoChanged();
    void networkStatusUpdated(bool connected);
    void gotStationDetails(QString stationDetails);
    void gotAllStationsDetails(QString allStationsDetails);
    void modeNotSupported();

public slots:
    void getContractsFinished();
    void replyFinished();
    void stationDetailsFinished();
    void allStationsDetailsFinished();

    void networkStatusChanged(bool);

private:
    QNetworkAccessManager* _networkAccessManager;
    QNetworkConfigurationManager* _networkConfigManager;

    StaticDataProvider _staticDataProvider;

    QString _cartoJson;
    QString _currentCity;
    QString _cacheDir;

    PendingAction _pendingAction;

};

#endif // DATAPROVIDER_H
