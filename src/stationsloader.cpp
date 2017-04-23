#include "stationsloader.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include "parser/bikedataparser.h"

StationsLoader::StationsLoader(QObject *parent) : QObject(parent)
{
    _networkAccessManager = new QNetworkAccessManager(this);
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
    if (!_city->getAutoDiscoveryUrl().isEmpty() && _city->getStationsListUrl().isEmpty()) {
        QFile urlsFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
                       QDir::separator() + stationsUrlsFileName());
        if (urlsFile.open(QIODevice::ReadOnly)) {
            QByteArray savedData = urlsFile.readAll();
            parseCityUrls(savedData);
        }
        else {
            QNetworkRequest request(_city->getAutoDiscoveryUrl());
            QNetworkReply *reply = _networkAccessManager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(cityUrlsFetched()));
            return;
        }
    }

    QFile stationsFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                       + QDir::separator() + cacheFileName());
    if (stationsFile.open(QIODevice::ReadOnly)) {
        QByteArray savedData = stationsFile.readAll();
        parseStationsList(savedData, false);
    }
    else {
        QNetworkRequest request(_city->getStationsListUrl().isEmpty() ?
                                    _city->getAllStationsDetailsUrl() :
                                    _city->getStationsListUrl());
        QNetworkReply *reply = _networkAccessManager->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(stationsListFetched()));
    }
}

void StationsLoader::fetchAllStationsDetails()
{
    if (!_city->getAllStationsDetailsUrl().isEmpty()) {
        QNetworkRequest request(_city->getAllStationsDetailsUrl());
        QNetworkReply *reply = _networkAccessManager->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(stationsListFetched()));
    }
    else if (!_city->getStationsStatusUrl().isEmpty()) {
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
    parseCityUrls(cityUrls);
    reply->deleteLater();
    fetchAllStationsList();
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
        if (_city->getStationsListUrl().isEmpty()) {
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
    }

    parseStationsList(stationsString, _city->getStationsListUrl().isEmpty());
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
        QList<Station*> stations = parser->parseAllStations(stationsString, withDetails);
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
        }
    }
}
