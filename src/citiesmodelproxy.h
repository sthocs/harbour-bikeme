#ifndef CITIESMODELPROXY_H
#define CITIESMODELPROXY_H

#include <QSortFilterProxyModel>

#include "citiesmodel.h"

class CitiesModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CitiesModelProxy();

    Q_INVOKABLE int realIndex(int index) const;
    Q_INVOKABLE void filter(QString filter);

    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

};

#endif // CITIESMODELPROXY_H
