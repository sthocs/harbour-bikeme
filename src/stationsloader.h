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
    void error(QString errorMsg);

public slots:
    void stationsListFetched();
    void stationDetailsFetched();

private:
    void parseStationsList(QString stations, bool withDetails);
    QString cacheFileName() const;

    QNetworkAccessManager* _networkAccessManager;

    City* _city;
    QHash<QString, Station*> _stations;
};

#endif // STATIONSLOADER_H
