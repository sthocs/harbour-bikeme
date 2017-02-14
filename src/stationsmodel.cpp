#include "stationsmodel.h"

StationsModel::StationsModel(QObject *parent) : QAbstractListModel(parent),
    _list(QList<Station*>())
{
    connect(&_stationsLoader, SIGNAL(allStationsDetailsFetched(QList<Station*>)), this, SLOT(addStations(QList<Station*>)));
}

void StationsModel::loadAll()
{
    beginRemoveRows(QModelIndex(), 0, _list.size() - 1);
    qDeleteAll(_list);
    _list.clear();
    endRemoveRows();
    _stationsLoader.fetchAllStationsDetails(_allStationsDetailsUrl);
}

void StationsModel::addStations(QList<Station*> stations)
{
    beginInsertRows(QModelIndex(), _list.size(), _list.size() + stations.size() - 1);
    _list.append(stations);
    endInsertRows();
    updateCenter();
    emit stationsLoaded();
}

void StationsModel::updateCenter()
{
    double avgLat = 0;
    double avgLng = 0;
    int nbValidStations = 0;
    for (int i = 0; i < _list.size(); ++i) {
        avgLat += _list.at(i)->coordinates.latitude();
        avgLng += _list.at(i)->coordinates.longitude();
        if (_list.at(i)->coordinates.latitude() != 0) {
            ++nbValidStations;
        }
    }
    avgLat /= nbValidStations;
    avgLng /= nbValidStations;
    _center = QGeoCoordinate(avgLat, avgLng);
    emit centerChanged();
}

QGeoCoordinate StationsModel::getCenter() const
{
    return _center;
}

QString StationsModel::getProviderName() const
{
    return _providerName;
}

QUrl StationsModel::getAllStationsDetailsUrl() const
{
    return _allStationsDetailsUrl;
}

void StationsModel::setAllStationsDetailsUrl(QUrl allStationsDetailsUrl)
{
    _allStationsDetailsUrl = allStationsDetailsUrl;
}

void StationsModel::setProviderName(QString providerName)
{
    _providerName = providerName;
    _stationsLoader.setProvider(providerName);
}

// Virtual functions of QAbstractListModel
int StationsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _list.size();
}

QHash<int, QByteArray> StationsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NumberRole] = "number";
    roles[CoordinatesRole] = "coordinates";
    roles[BikesNbRole] = "available_bikes";
    roles[FreeSlotsNbRole] = "available_bike_stands";
    return roles;
}

QVariant StationsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _list.size())
        return QVariant();

    switch(role) {
    case NumberRole:
        return _list.at(index.row())->number;
    case CoordinatesRole:
        return QVariant::fromValue(_list.at(index.row())->coordinates);
    case BikesNbRole:
        return _list.at(index.row())->available_bikes;
    case FreeSlotsNbRole:
        return _list.at(index.row())->available_bike_stands;
    }

    return QVariant();
}
