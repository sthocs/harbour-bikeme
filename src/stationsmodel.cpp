#include "stationsmodel.h"

StationsModel::StationsModel(QObject *parent) : QAbstractListModel(parent),
    _list(QList<Station*>())
{
    connect(&_stationsLoader, SIGNAL(allStationsDetailsFetched(QList<Station*>)), this, SLOT(addStations(QList<Station*>)));
    connect(&_stationsLoader, SIGNAL(allStationsListFetched(QList<Station*>)), this, SLOT(setStationsList(QList<Station*>)));
    connect(&_stationsLoader, SIGNAL(stationDetailsFetched(Station*)), this, SLOT(emitDataChanged(Station*)));
}

void StationsModel::loadStationsList()
{
    beginRemoveRows(QModelIndex(), 0, _list.size() - 1);
    qDeleteAll(_list);
    _list.clear();
    endRemoveRows();
    _stationsLoader.fetchAllStationsList(_allStationsDetailsUrl);
}

void StationsModel::loadAll()
{
    beginRemoveRows(QModelIndex(), 0, _list.size() - 1);
    qDeleteAll(_list);
    _list.clear();
    endRemoveRows();
    _stationsLoader.fetchAllStationsDetails(_allStationsDetailsUrl);
}

bool StationsModel::fetchStationInformation(QModelIndex index)
{
    if (_singleStationDetailsUrlTemplate.isEmpty()) {
        return false;
    }
    _stationsLoader.fetchStationDetails(_list.at(index.row()), _singleStationDetailsUrlTemplate);
    return true;
}

void StationsModel::fetchStationsInformation(QList<QModelIndex> indexes)
{
    if (_singleStationDetailsUrlTemplate.isEmpty()) {
        loadAll();
    }
    else {
        foreach (QModelIndex index, indexes) {
            _stationsLoader.fetchStationDetails(_list.at(index.row()), _singleStationDetailsUrlTemplate);
        }
    }
}

void StationsModel::setStationsList(QList<Station*> stations)
{
    beginInsertRows(QModelIndex(), _list.size(), _list.size() + stations.size() - 1);
    _list.append(stations);
    endInsertRows();
    updateCenter();
    emit stationsLoaded(false);
}

void StationsModel::addStations(QList<Station*> stations)
{
    beginInsertRows(QModelIndex(), _list.size(), _list.size() + stations.size() - 1);
    _list.append(stations);
    endInsertRows();
    updateCenter();
    emit stationsLoaded(true);
}

void StationsModel::emitDataChanged(Station* station)
{
    for (int row = 0; row < _list.size(); ++row) {
        if (_list.at(row) == station) {
            emit dataChanged(index(row), index(row));
        }
    }
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

QString StationsModel::getCityName() const
{
    return _cityName;
}

QUrl StationsModel::getAllStationsDetailsUrl() const
{
    return _allStationsDetailsUrl;
}

QString StationsModel::getStationDetailsUrlTemplate() const
{
    return _singleStationDetailsUrlTemplate;
}

void StationsModel::setAllStationsDetailsUrl(QUrl allStationsDetailsUrl)
{
    _allStationsDetailsUrl = allStationsDetailsUrl;
}

void StationsModel::setStationDetailsUrlTemplate(QString stationDetailsUrlTemplate)
{
    _singleStationDetailsUrlTemplate = stationDetailsUrlTemplate;
}

void StationsModel::setProviderName(QString providerName)
{
    _providerName = providerName;
    _stationsLoader.setProvider(_providerName);
}

void StationsModel::setCityName(QString cityName)
{
    _cityName = cityName;
    _stationsLoader.setCity(_cityName);
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
    roles[NameRole] = "name";
    roles[CoordinatesRole] = "coordinates";
    roles[BikesNbRole] = "available_bikes";
    roles[FreeSlotsNbRole] = "available_bike_stands";
    roles[LastUpdateRole] = "last_update";
    return roles;
}

QVariant StationsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _list.size())
        return QVariant();

    switch(role) {
    case NumberRole:
        return _list.at(index.row())->number;
    case NameRole:
        return _list.at(index.row())->name;
    case CoordinatesRole:
        return QVariant::fromValue(_list.at(index.row())->coordinates);
    case BikesNbRole:
        return _list.at(index.row())->available_bikes;
    case FreeSlotsNbRole:
        return _list.at(index.row())->available_bike_stands;
    case LastUpdateRole:
        return _list.at(index.row())->last_update;
    }

    return QVariant();
}
