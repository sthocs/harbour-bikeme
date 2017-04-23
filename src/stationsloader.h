#ifndef STATIONSLOADER_H
#define STATIONSLOADER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QHash>

#include "city.h"
#include "station.h"

class StationsLoader : public QObject
{
    Q_OBJECT
public:
    explicit StationsLoader(QObject *parent = 0);
    ~StationsLoader();

    void setCity(City* city);
    void fetchAllStationsList();
    void fetchAllStationsDetails();
    void fetchStationDetails(Station* station);

signals:
    void stationsFetched(QList<Station*> stations, bool withDetails);
    void stationDetailsFetched(Station* station);
    void stationsRealTimeDataFetched(QString stationsRealTimeData);
    void error(QString errorMsg);

public slots:
    void cityUrlsFetched();
    void stationsListFetched();
    void stationDetailsFetched();
    void stationsRealTimeDataFetched();

private:
    void parseCityUrls(QString urls);
    void parseStationsList(QString stations, bool withDetails);
    void cacheOnDisk(QString data, QString filename);
    QString cacheFileName() const;
    QString stationsUrlsFileName() const;

    QNetworkAccessManager* _networkAccessManager;

    City* _city;
    QHash<QString, Station*> _stations;
};

#endif // STATIONSLOADER_H
