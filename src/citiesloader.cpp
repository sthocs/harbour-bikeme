#include "citiesloader.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "sailfishapp.h"
#include "parser/bikedataparser.h"
#include "configmanager.h"

CitiesLoader::CitiesLoader(QObject *parent) : QObject(parent)
{
    _networkAccessManager = new QNetworkAccessManager(this);
}

CitiesLoader::~CitiesLoader()
{
    delete _networkAccessManager;
}

void CitiesLoader::loadAll(bool fromCache)
{
    emit errorMsgChanged(QString());
    ConfigManager config;
    int dataSource = config.getSetting("dataSource").toInt();
    QString filePath = dataSource == 0 ? "data/bikesproviders.json" : "data/citybikes.json";
    if (loadCitiesFromProviders(filePath, fromCache) && dataSource == 0) {
        loadCitiesFromFile();
    }
}

bool CitiesLoader::loadCitiesFromProviders(QString filePath, bool fromCache)
{
    bool res = true;
    QFile providersFile(SailfishApp::pathTo(filePath).toLocalFile());

    if (!providersFile.open(QIODevice::ReadOnly))
        return false;

    QByteArray json = providersFile.readAll();
    providersFile.close();

    _pendingRequests = 0;
    _fetchedProvidersCount = 0;
    _errorsCount = 0;
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonArray providersArray = doc.array();
    emit providersCountChanged(providersArray.size());
    for (int i = 0; i < providersArray.size(); ++i) {
        QJsonObject providerJson = providersArray[i].toObject();
        QString apiKey(providerJson["apiKey"].toString());

        ProviderInfo provider;
        provider.name = providerJson["name"].toString();
        provider.url = providerJson["url"].toString();
        provider.singleStationDetailsUrlTemplate = providerJson["stationDetailsUrl"].toString();
        provider.allStationsDetailsUrl = providerJson["allStationsDetailsUrl"].toString();
        provider.autoDiscoveryUrl = providerJson["autoDiscoveryUrl"].toString();
        if (!apiKey.isEmpty()) {
            provider.url = provider.url.arg(apiKey);
            provider.singleStationDetailsUrlTemplate = provider.singleStationDetailsUrlTemplate.arg(apiKey);
            provider.allStationsDetailsUrl = provider.allStationsDetailsUrl.arg(apiKey);
        }

        if (fromCache) {
            res &= loadProviderFromCache(provider);
        }
        else {
            ++_pendingRequests;
            QString url = !provider.autoDiscoveryUrl.isEmpty() ?
                        provider.autoDiscoveryUrl : provider.url;
            _providers[url] = provider;
            QNetworkRequest request(url);
            QNetworkReply *reply = _networkAccessManager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(bikeProviderFetched()));
            res &= true;
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
        info.hasElectricBikes = cityJson["hasElectricBikes"].toBool();
        info.stationsInfoUrl = QUrl(cityJson["stationsListUrl"].toString());
        info.allStationsDetailsUrl = QUrl(cityJson["allStationsDetailsUrl"].toString());
        info.singleStationDetailsUrlTemplate = cityJson["stationDetailsUrl"].toString();
        info.stationsDataModes |= !info.stationsInfoUrl.isEmpty() ? StationsListOnly : NoMode;
        info.stationsDataModes |= !info.allStationsDetailsUrl.isEmpty() ? StationsListAndData : NoMode;
        info.stationsDataModes |= !info.singleStationDetailsUrlTemplate.isEmpty() ? SingleStationData : NoMode;
        info.zoom = cityJson["zoom"].toInt();
        info.copyright = cityJson["copyright"].toString();
        City* city = new City();
        city->setInfo(info);
        citiesList.append(city);
    }
    emit citiesAdded(citiesList);
}

bool CitiesLoader::loadProviderFromCache(ProviderInfo& provider)
{
    QFile citiesFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                   + QDir::separator() + provider.name);
    if (citiesFile.open(QIODevice::ReadOnly)) {
        QByteArray savedData = citiesFile.readAll();
        parse(savedData, provider);
        citiesFile.close();
        return true;
    }
    return false;
}

void CitiesLoader::bikeProviderFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    ProviderInfo& providerInfo = _providers[reply->url().toString()];

    if (reply->error() != QNetworkReply::NoError) {
        emit errorMsgChanged(reply->errorString());
        fetchedProvidersCountChanged(_fetchedProvidersCount, ++_errorsCount);
        if (--_pendingRequests == 0) {
            emit finished();
        }
        reply->deleteLater();
        loadProviderFromCache(providerInfo);
        return;
    }
    QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (!redirectUrl.isEmpty())
    {
        _providers[redirectUrl.toString()] = providerInfo;
        QNetworkRequest req(redirectUrl);
        QNetworkReply *rep = _networkAccessManager->get(req);
        connect(rep, SIGNAL(finished()), this, SLOT(bikeProviderFetched()));
        reply->deleteLater();
        return;
    }

    QString citiesString = QString::fromUtf8(reply->readAll());
    QFile citiesFile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                   + QDir::separator() + providerInfo.name);
    if (!citiesFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open" << providerInfo.name;
    }
    citiesFile.write(citiesString.toUtf8());
    citiesFile.close();
    parse(citiesString, providerInfo);
    fetchedProvidersCountChanged(++_fetchedProvidersCount, _errorsCount);
    if (--_pendingRequests == 0) {
        emit finished();
    }
    reply->deleteLater();
}

void CitiesLoader::parse(QString citiesString, ProviderInfo& providerInfo)
{
    int id = QMetaType::type(providerInfo.name.toLatin1().data());
    if (id > 0) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        QList<City*> cities = parser->parseCities(citiesString, providerInfo);
        delete parser;
        emit citiesAdded(cities);
    }
}
