#include "stationsmodelproxy.h"

StationsModelProxy::StationsModelProxy() : _filter(true)
{

}

void StationsModelProxy::filter(QGeoCoordinate topLeft, QGeoCoordinate bottomRight)
{
    _topLeftFilter = topLeft;
    _bottomRightFilter = bottomRight;
    invalidateFilter();
}

bool StationsModelProxy::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &sourceParent) const
{
    if (!_filter) { return true; }
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    QGeoCoordinate coordinates = sourceModel()->data(index, StationsModel::CoordinatesRole).value<QGeoCoordinate>();
    return coordinates.latitude() < _topLeftFilter.latitude() &&
            coordinates.latitude() > _bottomRightFilter.latitude() &&
            coordinates.longitude() > _topLeftFilter.longitude() &&
            coordinates.longitude() < _bottomRightFilter.longitude();
}

int StationsModelProxy::sourceRow(int row)
{
    return mapToSource(index(row, 0)).row();
}
