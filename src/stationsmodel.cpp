#include "stationsmodel.h"

#include "parser/bikedataparser.h"

StationsModel::StationsModel(QObject *parent) : QAbstractListModel(parent),
    _list(QList<Station*>())
{
    connect(&_stationsLoader, SIGNAL(stationsFetched(QList<Station*>, bool)), this, SLOT(setStations(QList<Station*>, bool)));
    connect(&_stationsLoader, SIGNAL(stationDetailsFetched(Station*)), this, SLOT(stationDetailsFetched(Station*)));
    connect(&_stationsLoader, SIGNAL(stationsRealTimeDataFetched(QString)), this, SLOT(stationsRealTimeDataFetched(QString)));
    connect(&_stationsLoader, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
}

StationsModel::~StationsModel()
{
    qDeleteAll(_list);
    _list.clear();
}

void StationsModel::loadStationsList()
{
    _stationsLoader.fetchAllStationsList();
}

void StationsModel::loadAllStationsDetails()
{
    _stationsLoader.fetchAllStationsDetails();
}

bool StationsModel::fetchStationInformation(int index)
{
    if (!_city->stationDataModes().testFlag(SingleStationData)) {
        return false;
    }
    _stationsLoader.fetchStationDetails(_list.at(index));
    return true;
}

void StationsModel::fetchStationsInformation(QList<QModelIndex> indexes)
{
    if (!_city->stationDataModes().testFlag(SingleStationData)) {
        loadAllStationsDetails();
    }
    else {
        foreach (QModelIndex index, indexes) {
            _stationsLoader.fetchStationDetails(_list.at(index.row()));
        }
    }
}

bool StationsModel::exists(int number)
{
    foreach (Station* station, _list) {
        if (station->number == number) {
            return true;
        }
    }
    return false;
}

void StationsModel::setStations(QList<Station*> stations, bool withDetails)
{
    beginResetModel();
    qDeleteAll(_list);
    _list.clear();
    _list.append(stations);
    endResetModel();
    updateCenter();
    emit countChanged();
    emit stationsLoaded(withDetails);
}

void StationsModel::stationDetailsFetched(Station* station)
{
    for (int row = 0; row < _list.size(); ++row) {
        if (_list.at(row) == station) {
            emit dataChanged(index(row), index(row));
            emit stationUpdated(station);
        }
    }
}

void StationsModel::stationsRealTimeDataFetched(QString stationsRealTimeData)
{
    int id = QMetaType::type(_city->getProviderName().toLatin1().data());
    if (id > 0) {
        beginResetModel();
        BikeDataParser *parser = static_cast<BikeDataParser*>( QMetaType::create( id ) );
        parser->parseStationsRealTimeData(stationsRealTimeData, _list);
        delete parser;
        endResetModel();
        emit stationsLoaded(true);
    }
    else {
        emit error("Real-time data: Parser not found");
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
    QGeoCoordinate newCenter(avgLat, avgLng);
    if (newCenter != _center) {
        _center = newCenter;
        emit centerChanged();
    }
}

QGeoCoordinate StationsModel::getCenter() const
{
    return _center;
}

void StationsModel::setCity(City* city)
{
    _city = city;
    _stationsLoader.setCity(city);
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
    roles[AddressRole] = "address";
    roles[OpenedRole] = "opened";
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
    case AddressRole:
        return _list.at(index.row())->address;
    case OpenedRole:
        return _list.at(index.row())->opened;
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
