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
    Q_PROPERTY(QString address READ getAddress)
    Q_PROPERTY(bool opened READ isOpened)
    Q_PROPERTY(int available_bikes READ getAvailableBikes)
    Q_PROPERTY(int available_electric_bikes READ getAvailableElectricBikes)
    Q_PROPERTY(int available_bike_stands READ getAvailableBikeStands)
    Q_PROPERTY(QDateTime last_update READ getLastUpdate)
public:
    explicit Station(QObject *parent = 0);

    double number;
    QString name;
    QString address;
    QGeoCoordinate coordinates;
    bool opened;
    int bike_stands;
    int available_bikes;
    int available_electric_bikes;
    int available_bike_stands;
    QDateTime last_update;

    double getNumber() const { return number; }
    QString getName() const { return name; }
    QString getAddress() const { return address; }
    bool isOpened() const { return opened; }
    int getAvailableBikes() const { return available_bikes; }
    int getAvailableElectricBikes() const { return available_electric_bikes; }
    int getAvailableBikeStands() const { return available_bike_stands; }
    QDateTime getLastUpdate() const { return last_update; }

public slots:
};

#endif // STATION_H
