#ifndef CITIESMODELPROXY_H
#define CITIESMODELPROXY_H

#include <QSortFilterProxyModel>

#include "citiesmodel.h"

class CitiesModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CitiesModelProxy();
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

    Q_INVOKABLE void filter(QString filter);

private:
    CitiesModel model;
};

#endif // CITIESMODELPROXY_H
