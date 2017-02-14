#ifndef STATIONSMODELPROXY_H
#define STATIONSMODELPROXY_H

#include <QSortFilterProxyModel>
#include <QGeoCoordinate>

#include "stationsmodel.h"

class StationsModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    StationsModelProxy();

    Q_INVOKABLE void filter(QGeoCoordinate topLeft, QGeoCoordinate bottomRight);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QGeoCoordinate _topLeftFilter;
    QGeoCoordinate _bottomRightFilter;
    bool _filter;
};

#endif // STATIONSMODELPROXY_H
