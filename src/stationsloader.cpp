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

void StationsLoader::fetchAllStationsList()
{
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
    QNetworkRequest request(_city->getAllStationsDetailsUrl());
    QNetworkReply *reply = _networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(stationsListFetched()));
}

void StationsLoader::fetchStationDetails(Station *station)
{
    QString url(_city->getSingleStationDetailsUrlTemplate().arg(station->number));
    _stations[url] = station;

    QNetworkRequest request(url);
    QNetworkReply *reply = _networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(stationDetailsFetched()));
}

void StationsLoader::stationsListFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
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
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }

    Station* station = _stations[reply->url().toString()];
    int id = QMetaType::type(_city->getProviderName().toLatin1().data());
    if (id != -1) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        parser->parseStationDetails2(QString::fromUtf8(reply->readAll()), station);
        delete parser;
        emit stationDetailsFetched(station);
    }
    reply->deleteLater();
}

void StationsLoader::parseStationsList(QString stationsString, bool withDetails)
{
    int id = QMetaType::type(_city->getProviderName().toLatin1().data());
    if (id != -1) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        QList<Station*> stations = parser->parseAllStations(stationsString, withDetails);
        delete parser;
        emit stationsFetched(stations, withDetails);
    }
}
