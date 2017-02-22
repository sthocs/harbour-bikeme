#ifndef STATION_H
#define STATION_H

#include <QObject>
#include <QGeoCoordinate>
#include <QDateTime>

class Station : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int number READ getNumber)
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(int available_bikes READ getAvailableBikes)
    Q_PROPERTY(int available_bike_stands READ getAvailableBikeStands)
    Q_PROPERTY(QDateTime last_update READ getLastUpdate)
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

    int getNumber() const { return number; }
    QString getName() const { return name; }
    int getAvailableBikes() const { return available_bikes; }
    int getAvailableBikeStands() const { return available_bike_stands; }
    QDateTime getLastUpdate() const { return last_update; }

public slots:
};

#endif // STATION_H
