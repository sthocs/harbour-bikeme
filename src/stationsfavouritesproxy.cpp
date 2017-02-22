#include "stationsfavouritesproxy.h"

#include "stationsmodel.h"

StationsFavouritesProxy::StationsFavouritesProxy()
{
    setDynamicSortFilter(true);
    sort(0);
}

void StationsFavouritesProxy::setFavourites(QList<int> favourites)
{
    _favourites = favourites;
    invalidateFilter();
}

bool StationsFavouritesProxy::add(int stationNumber)
{
    if (_favourites.indexOf(stationNumber) >= 0) {
        return false;
    }
    _favourites.append(stationNumber);
    invalidateFilter();
    return true;
}

void StationsFavouritesProxy::remove(int index)
{
    _favourites.removeAt(index);
    invalidateFilter();
}

void StationsFavouritesProxy::move(int from, int to)
{
    if (from < 0 || to >= _favourites.size()) {
        return;
    }
    _favourites.move(from, to);
    invalidate();
}

bool StationsFavouritesProxy::refreshStationInfo(int row)
{
    ((StationsModel*)sourceModel())->fetchStationInformation(mapToSource(index(row, 0)).row());
}

void StationsFavouritesProxy::refreshAll()
{
    QList<QModelIndex> indexes;
    for (int i = 0; i < _favourites.size(); ++i) {
        indexes.append(mapToSource(index(i, 0)));
    }
    ((StationsModel*)sourceModel())->fetchStationsInformation(indexes);
}

bool StationsFavouritesProxy::lessThan(const QModelIndex &left,
                                       const QModelIndex &right) const
{
    int l_station = sourceModel()->data(left, StationsModel::NumberRole).toInt();
    int r_station = sourceModel()->data(right, StationsModel::NumberRole).toInt();

    return _favourites.indexOf(l_station) < _favourites.indexOf(r_station);
}

bool StationsFavouritesProxy::filterAcceptsRow(int sourceRow,
                                               const QModelIndex &sourceParent) const
{
     QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
     return _favourites.contains(sourceModel()->data(index, StationsModel::NumberRole).toInt());
}
