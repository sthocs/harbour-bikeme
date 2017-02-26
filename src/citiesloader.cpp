#include "citiesloader.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "sailfishapp.h"
#include "parser/bikedataparser.h"

CitiesLoader::CitiesLoader(QObject *parent) : QObject(parent)
{
    _networkAccessManager = new QNetworkAccessManager(this);
}

void CitiesLoader::loadAll(bool fromCache)
{
    if (loadCitiesFromProviders(fromCache)) {
        loadCitiesFromFile();
    }
}

bool CitiesLoader::loadCitiesFromProviders(bool fromCache)
{
    bool res = false;
    QFile providersFile(SailfishApp::pathTo("data/bikesproviders.json").toLocalFile());

    if (!providersFile.open(QIODevice::ReadOnly))
        return false;

    QByteArray json = providersFile.readAll();
    providersFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonArray providersArray = doc.array();
    for (int i = 0; i < providersArray.size(); ++i) {
        QJsonObject providerJson = providersArray[i].toObject();
        QString apiKey(providerJson["apiKey"].toString());

        ProviderInfo provider;
        provider.name = providerJson["name"].toString();
        provider.url = providerJson["url"].toString().arg(apiKey);
        provider.singleStationDetailsUrlTemplate = providerJson["stationDetailsUrl"].toString().arg(apiKey);
        provider.allStationsDetailsUrl = providerJson["allStationsDetailsUrl"].toString().arg(apiKey);
        _providers[provider.url] = provider;

        if (fromCache) {
            QFile citiesFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                           + QDir::separator() + provider.name);
            if (citiesFile.open(QIODevice::ReadOnly)) {
                QByteArray savedData = citiesFile.readAll();
                parse(savedData, provider);
                res = true;
            }
        }
        else {
            QNetworkRequest request(provider.url);
            QNetworkReply *reply = _networkAccessManager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(bikeProviderFetched()));
            res = true;
        }
    }
    return res;
}

void CitiesLoader::loadCitiesFromFile()
{
    QFile citiesFile(SailfishApp::pathTo("data/cities.json").toLocalFile());
    if (!citiesFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QList<City*> citiesList;
    QByteArray json = citiesFile.readAll();
    citiesFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonArray providersArray = doc.array();
    for (int i = 0; i < providersArray.size(); ++i) {
        QJsonObject cityJson = providersArray[i].toObject();
        CityInfo info;
        info.providerName = cityJson["provider"].toString();
        info.name = cityJson["name"].toString();
        info.commercialName = cityJson["commercial_name"].toString();
        info.countryCode = cityJson["country_code"].toString();
        info.stationsInfoUrl = QUrl(cityJson["stationsListUrl"].toString());
        info.allStationsDetailsUrl = QUrl(cityJson["allStationsDetailsUrl"].toString());
        info.singleStationDetailsUrlTemplate = cityJson["stationDetailsUrl"].toString();
        City* city = new City();
        city->setInfo(info);
        citiesList.append(city);
    }
    emit citiesAdded(citiesList);
}

void CitiesLoader::bikeProviderFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    //TODO handle redirections
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }
    QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if(!redirectUrl.isEmpty())
    {
        _providers[redirectUrl.toString()] = _providers[reply->url().toString()];
        QNetworkRequest req(redirectUrl);
        QNetworkReply *rep = _networkAccessManager->get(req);
        connect(rep, SIGNAL(finished()), this, SLOT(bikeProviderFetched()));
        reply->deleteLater();
        return;
    }

    ProviderInfo providerInfo = _providers[reply->url().toString()];
    QString citiesString = QString::fromUtf8(reply->readAll());
    QFile citiesFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                   + QDir::separator() + providerInfo.name);
    if (!citiesFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open" << providerInfo.name;
    }
    citiesFile.write(citiesString.toUtf8());
    parse(citiesString, providerInfo);
    reply->deleteLater();
}

void CitiesLoader::parse(QString citiesString, ProviderInfo providerInfo)
{
    int id = QMetaType::type(providerInfo.name.toLatin1().data());
    if (id != -1) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        QList<City*> cities = parser->parseCities(citiesString, providerInfo);
        delete parser;
        emit citiesAdded(cities);
    }
}
