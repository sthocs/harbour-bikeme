#ifndef STATIONSLOADER_H
#define STATIONSLOADER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "station.h"

class StationsLoader : public QObject
{
    Q_OBJECT
public:
    explicit StationsLoader(QObject *parent = 0);

    void setProvider(QString providerName);
    void fetchAllStationsDetails(QUrl allStationsDetailsUrl);

signals:
    void allStationsDetailsFetched(QList<Station*> stations);

public slots:
    void allStationsDetailsFetched();

private:
    QNetworkAccessManager* _networkAccessManager;

    QString _providerName;
};

#endif // STATIONSLOADER_H
