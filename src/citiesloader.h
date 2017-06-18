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
    ~CitiesLoader();

    void loadAll(bool fromCache = false);

signals:
    void cityAdded(City* city);
    void citiesAdded(QList<City*> cities);
    void providersCountChanged(int);
    void fetchedProvidersCountChanged(int, int);
    void errorMsgChanged(QString);
    void finished();

private slots:
    void bikeProviderFetched();

private:
    void loadCitiesFromFile();
    bool loadCitiesFromProviders(QString filePath, bool fromCache);
    bool loadProviderFromCache(ProviderInfo& provider);
    void parse(QString cities, ProviderInfo& providerInfo);

    QNetworkAccessManager* _networkAccessManager;

    QHash<QString, ProviderInfo> _providers;
    int _fetchedProvidersCount;
    int _pendingRequests;
    int _errorsCount;

};

#endif // CITIESLOADER_H
