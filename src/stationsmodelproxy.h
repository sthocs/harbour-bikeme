#ifndef STATIONSMODELPROXY_H
#define STATIONSMODELPROXY_H

#include <QSortFilterProxyModel>
#include <QGeoCoordinate>

#include "stationsmodel.h"

class StationsModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool applyFilter READ applyFilter WRITE setApplyFilter)
public:
    StationsModelProxy();

    Q_INVOKABLE int sourceRow(int row);
    Q_INVOKABLE void filter(QGeoCoordinate topLeft, QGeoCoordinate bottomRight);

    bool applyFilter() { return _filter; }
    void setApplyFilter(bool applyFilter) { _filter = applyFilter; }

    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QGeoCoordinate _topLeftFilter;
    QGeoCoordinate _bottomRightFilter;
    bool _filter;
};

#endif // STATIONSMODELPROXY_H
