#include "citiesloader.h"

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

void CitiesLoader::loadAll()
{
    loadCitiesFromProviders();
}

void CitiesLoader::loadCitiesFromProviders()
{
    QFile providersFile(SailfishApp::pathTo("data/bikesproviders.json").toLocalFile());

    if (!providersFile.open(QIODevice::ReadOnly))
        return;

    QByteArray json = providersFile.readAll();
    providersFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonArray providersArray = doc.array();
    for (int i = 0; i < providersArray.size(); ++i) {
        QJsonObject providerJson = providersArray[i].toObject();
        QString apiKey(providerJson["apiKey"].toString());

        ProviderInfo provider;
        provider.name = providerJson["name"].toString();
        provider.url = providerJson["url"].toString().replace("{apiKey}", apiKey);
        provider.singleStationDetailsUrlTemplate = providerJson["stationDetailsUrl"].toString().replace("{apiKey}", apiKey);
        provider.allStationsDetailsUrl = providerJson["allStationsDetailsUrl"].toString().replace("{apiKey}", apiKey);
        _providers[provider.url] = provider;

        QNetworkRequest request(provider.url);
        QNetworkReply *reply = _networkAccessManager->get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(bikeProviderFetched()));
    }
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

    ProviderInfo providerInfo = _providers[reply->url().toString()];
    int id = QMetaType::type(providerInfo.name.toLatin1().data());
    if (id != -1) {
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        QList<City*> cities = parser->parseCities(QString::fromUtf8(reply->readAll()), providerInfo);
        delete parser;
        emit citiesAdded(cities);
    }
    reply->deleteLater();
}
