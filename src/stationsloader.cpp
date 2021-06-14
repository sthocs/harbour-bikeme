#include "stationsloader.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include "configmanager.h"
#include "parser/bikedataparser.h"

StationsLoader::StationsLoader(QObject *parent) : QObject(parent)
{
    _networkAccessManager = new QNetworkAccessManager(this);
    connect(_networkAccessManager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
            this,
            SLOT(handleSslErrors(QNetworkReply*, QList<QSslError>)));
}

void StationsLoader::handleSslErrors(QNetworkReply* reply, QList<QSslError> list) {
    ConfigManager config;
    qDebug() << reply->error();
    qDebug() << list;
    if (config.getSetting("ignoreSslErrors") == "true") {
        qDebug() << "ignoreSslErrors is enabled, ignoring error...";
        reply->ignoreSslErrors();
    }
}

StationsLoader::~StationsLoader()
{
    delete _networkAccessManager;
}

void StationsLoader::setCity(City* city)
{
    _city = city;
}

QString StationsLoader::cacheFileName() const
{
    return _city->getProviderName() + _city->getName();
}

QString StationsLoader::stationsUrlsFileName() const
{
    return cacheFileName() + "Urls";
}

void StationsLoader::fetchAllStationsList()
{
    // For systems providing an auto-discovery url, we first need to fetch the various urls
    if (!_city->getAutoDiscoveryUrl().isEmpty() && _city->getStationsListUrl().isEmpty()) {
        QFile urlsFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
                       QDir::separator() + stationsUrlsFileName());
        if (urlsFile.open(QIODevice::ReadOnly)) {
            QByteArray savedData = urlsFile.readAll();
            parseCityUrls(savedData);
            urlsFile.close();
        }
        _refreshUrlsInBackground = mustRefreshCache(urlsFile.fileName());
        if (!urlsFile.exists() || _refreshUrlsInBackground) {
            QNetworkRequest request(_city->getAutoDiscoveryUrl());
            QNetworkReply *reply = _networkAccessManager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(cityUrlsFetched()));
            if (!_refreshUrlsInBackground) {
                return;
            }
            // Otherwise, let the file refresh in background and continue refreshing the cities list
            qDebug() << "Will refresh urls in background for " << _city->getName();
        }
    }

    QFile stationsFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                       + QDir::separator() + cacheFileName());
    if (stationsFile.open(QIODevice::ReadOnly)) {
        QByteArray savedData = stationsFile.readAll();
        parseStationsList(savedData, false);
        stationsFile.close();

        if (!mustRefreshCache(stationsFile.fileName()) || _city->stationDataModes().testFlag(StationsListAndData)) {
            return;
        }
        else {
            qDebug() << "Will refresh stations list in background for " << _city->getName();
            _refreshStationsInBackground = true;
        }
    }
    QNetworkRequest request(_city->stationDataModes().testFlag(StationsListAndData) ?
                                _city->getAllStationsDetailsUrl() :
                                _city->getStationsListUrl());
    QNetworkReply *reply = _networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(stationsListFetched()));
}

void StationsLoader::fetchAllStationsDetails()
{
    if (_city->stationDataModes().testFlag(StationsListAndData)) {
        QNetworkRequest request(_city->getAllStationsDetailsUrl());
        QNetworkReply *reply = _networkAccessManager->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(stationsListFetched()));
    }
    else if (_city->stationDataModes().testFlag(RealTimeDataOnly)) {
        QNetworkRequest request(_city->getStationsStatusUrl());
        QNetworkReply *reply = _networkAccessManager->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(stationsRealTimeDataFetched()));
    }
}

void StationsLoader::fetchStationDetails(Station *station)
{
    QString url(_city->getSingleStationDetailsUrlTemplate().arg(station->number));
    _stations[url] = station;

    QNetworkRequest request(url);
    QNetworkReply *reply = _networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(stationDetailsFetched()));
}

void StationsLoader::cityUrlsFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        emit error(reply->errorString());
        reply->deleteLater();
        return;
    }
    QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if(!redirectUrl.isEmpty()) {
        _city->setAutoDiscoveryUrl(redirectUrl);
        fetchAllStationsList();
        reply->deleteLater();
        return;
    }

    QString cityUrls = QString::fromUtf8(reply->readAll());
    cacheOnDisk(cityUrls, stationsUrlsFileName());
    if (!_refreshUrlsInBackground) {
        parseCityUrls(cityUrls);
        fetchAllStationsList();
    }
    reply->deleteLater();
}

void StationsLoader::stationsListFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        emit error(reply->errorString());
        reply->deleteLater();
        return;
    }
    QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if(!redirectUrl.isEmpty())
    {
        if (_city->stationDataModes().testFlag(StationsListAndData)) {
            _city->setAllStationsDetailsUrl(redirectUrl);
        }
        else {
            _city->setStationsListUrl(redirectUrl);
        }
        fetchAllStationsList();
        reply->deleteLater();
        return;
    }

    QString stationsString = QString::fromUtf8(reply->readAll());
    if (!cacheFileName().isEmpty()) {
        QFile stationsFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                           + QDir::separator() + cacheFileName());
        if (!stationsFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Couldn't open" << cacheFileName();
        }
        stationsFile.write(stationsString.toUtf8());
        stationsFile.close();
    }

    if (!_refreshStationsInBackground) {
        parseStationsList(stationsString, _city->stationDataModes().testFlag(StationsListAndData));
    }
    reply->deleteLater();
}

void StationsLoader::stationDetailsFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    //TODO handle redirections
    if (reply->error() != QNetworkReply::NoError) {
        emit error(reply->errorString());
        reply->deleteLater();
        return;
    }

    Station* station = _stations[reply->url().toString()];
    int id = QMetaType::type(_city->getProviderName().toLatin1().data());
    if (id > 0) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        parser->parseStationDetails(QString::fromUtf8(reply->readAll()), station);
        delete parser;
        emit stationDetailsFetched(station);
    }
    reply->deleteLater();
}

void StationsLoader::stationsRealTimeDataFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        emit error(reply->errorString());
        reply->deleteLater();
        return;
    }
    QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if(!redirectUrl.isEmpty())
    {
        _city->setStationsStatusUrl(redirectUrl);
        fetchAllStationsDetails();
        reply->deleteLater();
        return;
    }

    QString stationsRealTimeDataString = QString::fromUtf8(reply->readAll());
    emit stationsRealTimeDataFetched(stationsRealTimeDataString);
    reply->deleteLater();
}

void StationsLoader::parseCityUrls(QString urls)
{
    int id = QMetaType::type(_city->getProviderName().toLatin1().data());
    if (id > 0) {
        BikeDataParser* parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        parser->parseCityUrls(urls, _city);
        delete parser;
    }
}

void StationsLoader::parseStationsList(QString stationsString, bool withDetails)
{
    int id = QMetaType::type(_city->getProviderName().toLatin1().data());
    if (id > 0) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        QList<Station*> stations = parser->parseStationsList(stationsString, withDetails);
        delete parser;
        emit stationsFetched(stations, withDetails);
    }
}

void StationsLoader::cacheOnDisk(QString data, QString filename)
{
    if (!filename.isEmpty()) {
        QFile file(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
                   QDir::separator() + filename);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Couldn't open" << filename;
        }
        else {
            file.write(data.toUtf8());
            file.close();
        }
    }
}

// Returns true if the file passed in parameters is older than 30 days
bool StationsLoader::mustRefreshCache(QString filepath)
{
    QFileInfo fileInfo(filepath);
    qint64 fileAge = fileInfo.created().toMSecsSinceEpoch();
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 dayInMillis = 1000 * 60 * 60 * 24;

    return fileInfo.exists() && ((now - fileAge) / dayInMillis) > 30;
}
