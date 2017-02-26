#ifndef STATIONSLOADER_H
#define STATIONSLOADER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QHash>

#include "station.h"

class StationsLoader : public QObject
{
    Q_OBJECT
public:
    explicit StationsLoader(QObject *parent = 0);

    void setProvider(QString providerName);
    void setCity(QString cityName);
    void fetchAllStationsList(QUrl allStationsListUrl);
    void fetchAllStationsDetails(QUrl allStationsDetailsUrl);
    void fetchStationDetails(Station* station, QString urlTemplate);

signals:
    void allStationsListFetched(QList<Station*> stations);
    void allStationsDetailsFetched(QList<Station*> stations);
    void stationDetailsFetched(Station* station);

public slots:
    void allStationsListFetched();
    void allStationsDetailsFetched();
    void stationDetailsFetched();

private:
    void parseStationsList(QString stations);
    QString cacheFileName() const;

    QNetworkAccessManager* _networkAccessManager;

    QString _providerName;
    QString _cityName;
    QHash<QString, Station*> _stations;
};

#endif // STATIONSLOADER_H
