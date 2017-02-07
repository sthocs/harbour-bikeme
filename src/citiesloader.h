#ifndef CITIESLOADER_H
#define CITIESLOADER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QHash>

#include "city.h"

struct ProviderInfo {
    QString name;
    QUrl url;
    QUrl stationDetailsUrl;
    QUrl allStationsDetailsUrl;
};

class CitiesLoader : public QObject
{
    Q_OBJECT

public:
    explicit CitiesLoader(QObject *parent = 0);

    void loadAll();

signals:
    void cityAdded(City* city);
    void citiesAdded(QList<City*> cities);

private slots:
    void bikeProviderFetched();

private:
    void loadCitiesFromFile();
    void loadCitiesFromProviders();

    QNetworkAccessManager* _networkAccessManager;

    QHash<QString, ProviderInfo> _providers;

};

#endif // CITIESLOADER_H
