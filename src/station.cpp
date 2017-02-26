#include "station.h"

Station::Station(QObject *parent) : QObject(parent),
    opened(true),
    bike_stands(-1),
    available_bikes(-1),
    available_bike_stands(-1)
{
}
