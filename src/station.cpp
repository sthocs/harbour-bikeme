#include "station.h"

Station::Station(QObject *parent) : QObject(parent),
    opened(true),
    bike_stands(0),
    available_bikes(0),
    available_bike_stands(0)
{
}
