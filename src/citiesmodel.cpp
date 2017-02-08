#include "citiesmodel.h"

CitiesModel::CitiesModel(QObject *parent) : QAbstractListModel(parent),
    _list(QList<City*>())
{
}

void CitiesModel::loadAll()
{
    beginRemoveRows(QModelIndex(), 0, _list.size() - 1);
    qDeleteAll(_list);
    _list.clear();
    endRemoveRows();
    citiesLoader.loadAll();
    connect(&citiesLoader, SIGNAL(citiesAdded(QList<City*>)), this, SLOT(addCities(QList<City*>)));
}

void CitiesModel::addCities(QList<City*> cities)
{
    beginInsertRows(QModelIndex(), _list.size(), _list.size() + cities.size() - 1);
    _list.append(cities);
    endInsertRows();
    emit countChanged();
}

int CitiesModel::count() const
{
    return rowCount();
}

// Virtual functions of QAbstractListModel
int CitiesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _list.size();
}

QHash<int, QByteArray> CitiesModel::roleNames() const {
    QHash<int, QByteArray> roles;
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
    case CountryRole:
        return _list.at(index.row())->getCountryCode();
    case CityNameRole:
        return _list.at(index.row())->getName();
    case CommercialNameRole:
        return _list.at(index.row())->getCommercialName();
    }

    return QVariant();
}
