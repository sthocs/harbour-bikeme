#include "cachemanager.h"

#include <QStandardPaths>
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QSslConfiguration>
#include <QSslSocket>


CacheManager::CacheManager()
{
    _networkAccessManager = new QNetworkAccessManager(this);
    _cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/";

    _pendingAction = NONE;
    _networkConfigManager = new QNetworkConfigurationManager(this);
    connect(_networkConfigManager,SIGNAL(onlineStateChanged(bool)),
            this,SLOT(networkStatusChanged(bool)));
}

void CacheManager::networkStatusChanged(bool connected) {
    qDebug() << "Network status changed: ";
    qDebug() << connected;
    if (connected) {
        switch (_pendingAction) {
        case DOWNLOAD_STATIONS:
            networkStatusUpdated(connected);
            downloadCarto(_currentCity);
            break;
        default:
            break;
        }
    }
}


QString CacheManager::cartoJson() const
{
    return _cartoJson;
}

void CacheManager::getContracts(bool forceRefresh)
{
    QFile file(_cacheDir + "contracts.json");
    if (!forceRefresh && file.exists())
    {
        qDebug() << "Contracts exists, not downloading";
        file.open(QIODevice::ReadOnly);
        QString contracts = QString::fromUtf8(file.readAll());
        file.close();
        emit contractsUpdated(contracts);
    }
    else if (forceRefresh)
    {
        QNetworkRequest req(QUrl("https://developer.jcdecaux.com/rest/vls/contracts"));
        QSslConfiguration config = req.sslConfiguration();
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        req.setSslConfiguration(config);
        QNetworkReply *reply = _networkAccessManager->get(req);
        connect(reply, SIGNAL(finished()), this, SLOT(getContractsFinished()));
    }
}

void CacheManager::getStationDetails(QString city, QString stationNumber)
{
    //qDebug() << "Getting station details...";
    QNetworkRequest req(QUrl("https://api.jcdecaux.com/vls/v1/stations/" + stationNumber +
                             "?contract=" + city + "&apiKey=changeme"));
    QSslConfiguration config = req.sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    req.setSslConfiguration(config);
    QNetworkReply *reply = _networkAccessManager->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(stationDetailsFinished()));
}

void CacheManager::downloadCarto(QString city)
{
    _currentCity = city;
    QFile file(_cacheDir + _currentCity + ".json");

    if (file.exists())
    {
        QFileInfo fileInfo(_cacheDir + _currentCity + ".json");
        qint64 fileAge = fileInfo.created().toMSecsSinceEpoch();
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        qint64 dayInMillis = 1000 * 60 * 60 * 24;

        if (((now - fileAge) / dayInMillis) > 14) // If file is older than 2 weeks, refresh it.
        {
            qDebug() << "Removing current cached " + _currentCity + ".json";
            file.remove();
        }
    }
    if (file.exists())
    {
        qDebug() << "File exists, not downloading";
        file.open(QIODevice::ReadOnly);
        _cartoJson = QString::fromUtf8(file.readAll());
        file.close();
        emit cartoChanged();
    }
    else
    {        
        qDebug() << "Downloading carto to: " + _cacheDir;
        //QNetworkRequest req(QUrl("https://api.jcdecaux.com/vls/v1/stations?contract=paris&apiKey=changeme"));
        QNetworkRequest req(QUrl("https://developer.jcdecaux.com/rest/vls/stations/" + _currentCity + ".json"));
        QSslConfiguration config = req.sslConfiguration();
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        req.setSslConfiguration(config);
        QNetworkReply *reply = _networkAccessManager->get(req);
        connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
    }
}

void CacheManager::replyFinished()
{
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    if (pReply->error() != QNetworkReply::NoError)
    {
        if (!_networkConfigManager->isOnline()) {
            _pendingAction = DOWNLOAD_STATIONS;
        }
        qDebug() << "Error while downloading carto";
        //qDebug() << pReply->errorString();
        if (!_networkConfigManager->isOnline()) {
            emit networkStatusUpdated(false);
        }
        pReply->deleteLater();
        return;
    }

    _pendingAction = NONE;

    QByteArray data = pReply->readAll();
    _cartoJson = QString::fromUtf8(data);

    QDir cacheDir(_cacheDir);
    if (!cacheDir.exists())
    {
        cacheDir.mkpath(_cacheDir);
    }
    QFile file(cacheDir.filePath(_currentCity + ".json"));
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();
    emit cartoChanged();
    pReply->deleteLater();
}

void CacheManager::stationDetailsFinished()
{
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    if (pReply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error while getting stations details";
        QString error = _networkConfigManager->isOnline() ? pReply->errorString() :
                                                            "Please connect to Internet and retry.";
        emit gotStationDetails(error);
        pReply->deleteLater();
        return;
    }

    QByteArray data = pReply->readAll();
    QString stationDetails = QString::fromUtf8(data);
    emit gotStationDetails(stationDetails);
    pReply->deleteLater();
}

void CacheManager::getContractsFinished()
{
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    if (pReply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error while getting cities list";
        //qDebug() << pReply->errorString();
        QString error = _networkConfigManager->isOnline() ? pReply->errorString() :
                                                            "Please connect to Internet and retry.";
        emit contractsUpdated(error);
        pReply->deleteLater();
        return;
    }

    QByteArray data = pReply->readAll();
    QString contracts = QString::fromUtf8(data);
    QDir cacheDir(_cacheDir);
    if (!cacheDir.exists())
    {
        cacheDir.mkpath(_cacheDir);
    }
    QFile file(cacheDir.filePath("contracts.json"));
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();

    emit contractsUpdated(contracts);
    pReply->deleteLater();
}

bool CacheManager::removeCacheDir()
{
    bool result = true;
    QDir dir(_cacheDir);

    if (dir.exists(_cacheDir)) {
        Q_FOREACH(QFileInfo info,
                  dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System |
                                    QDir::Hidden  | QDir::AllDirs |
                                    QDir::Files, QDir::DirsFirst)) {
            result = QFile::remove(info.absoluteFilePath());

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(_cacheDir);
    }
    return result;
}

QString CacheManager::getAppState()
{
    QFile file(_cacheDir + "app_state");
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
        QString state = QString::fromUtf8(file.readAll());
        file.close();
        return state;
    }
    return "";
}

void CacheManager::saveAppState(QString appState)
{
    QDir cacheDir(_cacheDir);
    if (!cacheDir.exists())
    {
        cacheDir.mkpath(_cacheDir);
    }
    QFile file(cacheDir.filePath("app_state"));
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << appState;
    file.close();
}
