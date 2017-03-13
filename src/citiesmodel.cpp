#include "citiesmodel.h"

CitiesModel::CitiesModel(QObject *parent) : QAbstractListModel(parent),
    _list(QList<City*>())
{
    connect(&citiesLoader, SIGNAL(citiesAdded(QList<City*>)), this, SLOT(addCities(QList<City*>)));
    connect(&citiesLoader, SIGNAL(providersCountChanged(int)), this, SLOT(setProvidersCount(int)));
    connect(&citiesLoader, SIGNAL(fetchedProvidersCountChanged(int, int)), this, SLOT(setFetchedProvidersCount(int, int)));
    connect(&citiesLoader, SIGNAL(errorMsgChanged(QString)), this, SLOT(setErrorMsg(QString)));
    connect(&citiesLoader, SIGNAL(finished()), this, SIGNAL(fetchFinished()));
    // Load cities from cache
    citiesLoader.loadAll(true);
}

CitiesModel::~CitiesModel()
{
    qDeleteAll(_list);
    _list.clear();
}

void CitiesModel::loadAll()
{
    setFetchedProvidersCount(0, 0);
    beginResetModel();
    qDeleteAll(_list);
    _list.clear();
    endResetModel();
    citiesLoader.loadAll();
}

void CitiesModel::addCities(QList<City*> cities)
{
    foreach (City* city, cities) {
        city->setParent(this); // Don't let QML destroy it
    }
    beginInsertRows(QModelIndex(), _list.size(), _list.size() + cities.size() - 1);
    _list.append(cities);
    endInsertRows();
    emit countChanged();
}

City* CitiesModel::cityAt(int row)
{
    if (row < 0 || row >= _list.size()) {
        return NULL;
    }

    return _list.at(row);
}

City* CitiesModel::get(QString identifier)
{
    foreach (City* city, _list) {
        if (city->identifier().compare(identifier) == 0) {
            return city;
        }
    }
    return NULL;
}

int CitiesModel::providersCount() const
{
    return _providersCount;
}

int CitiesModel::fetchedProvidersCount() const
{
    return _fetchedProvidersCount;
}

int CitiesModel::errorsCount() const
{
    return _errorsCount;
}

QString CitiesModel::errorMsg() const
{
    return _errorMsg;
}

// SLOTS
void CitiesModel::setProvidersCount(int count)
{
    _providersCount = count;
    emit providersCountChanged();
}

void CitiesModel::setFetchedProvidersCount(int count, int errorsCount)
{
    _fetchedProvidersCount = count;
    emit fetchedProvidersCountChanged();
    if (_errorsCount != errorsCount) {
        _errorsCount = errorsCount;
        emit errorsCountChanged();
    }
}

void CitiesModel::setErrorMsg(QString errorMsg)
{
    _errorMsg = errorMsg;
    emit errorMsgChanged();
}

// Virtual functions of QAbstractListModel
int CitiesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _list.size();
}

QHash<int, QByteArray> CitiesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "identifier";
    roles[CountryRole] = "country_code";
    roles[CityNameRole] = "name";
    roles[CommercialNameRole] = "commercial_name";
    return roles;
}

QVariant CitiesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _list.size())
        return QVariant();

    switch(role) {
    case IdRole:
        return _list.at(index.row())->identifier();
    case CountryRole:
        return _list.at(index.row())->getCountryCode();
    case CityNameRole:
        return _list.at(index.row())->getName();
    case CommercialNameRole:
        return _list.at(index.row())->getCommercialName();
    }

    return QVariant();
}
// End Virtual functions of QAbstractListModel
