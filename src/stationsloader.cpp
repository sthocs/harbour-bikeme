#include "stationsloader.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include "parser/bikedataparser.h"

StationsLoader::StationsLoader(QObject *parent) : QObject(parent)
{
    _networkAccessManager = new QNetworkAccessManager(this);
}

void StationsLoader::setProvider(QString providerName)
{
    _providerName = providerName;
}

void StationsLoader::setCity(QString cityName)
{
    _cityName = cityName;
}

QString StationsLoader::cacheFileName() const
{
    return _providerName + _cityName;
}

bool StationsLoader::fetchAllStationsList(QUrl allStationsListUrl)
{
    QFile stationsFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                       + QDir::separator() + cacheFileName());
    if (stationsFile.open(QIODevice::ReadOnly)) {
        QByteArray savedData = stationsFile.readAll();
        parseStationsList(savedData);
        return true;
    }
    else {
        QNetworkRequest request(allStationsListUrl);
        QNetworkReply *reply = _networkAccessManager->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(allStationsListFetched()));
    }
    return false;
}

void StationsLoader::fetchAllStationsDetails(QUrl allStationsDetailsUrl)
{
    QNetworkRequest request(allStationsDetailsUrl);
    QNetworkReply *reply = _networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(allStationsDetailsFetched()));
}

void StationsLoader::fetchStationDetails(Station *station, QString urlTemplate)
{
    QString url(urlTemplate.arg(station->number));
    _stations[url] = station;

    QNetworkRequest request(url);
    QNetworkReply *reply = _networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(stationDetailsFetched()));
}

void StationsLoader::allStationsListFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    //TODO handle redirections
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }

    QString stationsString = QString::fromUtf8(reply->readAll());
    if (!_cityName.isEmpty()) {
        QFile stationsFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                           + QDir::separator() + cacheFileName());
        if (!stationsFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Couldn't open" << cacheFileName();
        }
        stationsFile.write(stationsString.toUtf8());
    }

    parseStationsList(stationsString);
    reply->deleteLater();
}

void StationsLoader::allStationsDetailsFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    //TODO handle redirections
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }

    QString stationsString = QString::fromUtf8(reply->readAll());
    if (!_cityName.isEmpty()) {
        QFile stationsFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                           + QDir::separator() + cacheFileName());
        if (!stationsFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Couldn't open" << cacheFileName();
        }
        stationsFile.write(stationsString.toUtf8());
    }

    int id = QMetaType::type(_providerName.toLatin1().data());
    if (id != -1) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        QList<Station*> stations = parser->parseAllStations(stationsString, true);
        delete parser;
        emit allStationsDetailsFetched(stations);
    }
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
    int id = QMetaType::type(_providerName.toLatin1().data());
    if (id != -1) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        parser->parseStationDetails2(QString::fromUtf8(reply->readAll()), station);
        delete parser;
        emit stationDetailsFetched(station);
    }
    reply->deleteLater();
}

void StationsLoader::parseStationsList(QString stationsString)
{
    int id = QMetaType::type(_providerName.toLatin1().data());
    if (id != -1) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        QList<Station*> stations = parser->parseAllStations(stationsString, false);
        delete parser;
        emit allStationsListFetched(stations);
    }
}
