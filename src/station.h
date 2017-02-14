#ifndef STATION_H
#define STATION_H

#include <QObject>
#include <QGeoCoordinate>
#include <QDateTime>

class Station : public QObject
{
    Q_OBJECT
public:
    explicit Station(QObject *parent = 0);

    int number;
    QString name;
    QString address;
    QGeoCoordinate coordinates;
    bool opened;
    int bike_stands;
    int available_bike_stands;
    int available_bikes;
    QDateTime last_update;

signals:

public slots:
};

#endif // STATION_H
