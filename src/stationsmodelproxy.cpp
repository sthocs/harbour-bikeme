#include "stationsmodelproxy.h"

StationsModelProxy::StationsModelProxy() : _filter(false)
{

}

void StationsModelProxy::filter(QGeoCoordinate topLeft, QGeoCoordinate bottomRight)
{
    _filter = true;
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
