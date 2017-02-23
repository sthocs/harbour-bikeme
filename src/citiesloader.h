#ifndef CITIESLOADER_H
#define CITIESLOADER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QHash>

#include "city.h"

class CitiesLoader : public QObject
{
    Q_OBJECT

public:
    explicit CitiesLoader(QObject *parent = 0);

    void loadAll(bool fromCache = false);

signals:
    void cityAdded(City* city);
    void citiesAdded(QList<City*> cities);

private slots:
    void bikeProviderFetched();

private:
    void loadCitiesFromFile();
    bool loadCitiesFromProviders(bool fromCache);
    void parse(QString cities, ProviderInfo providerInfo);

    QNetworkAccessManager* _networkAccessManager;

    QHash<QString, ProviderInfo> _providers;

};

#endif // CITIESLOADER_H
