#ifndef CITIESMODELPROXY_H
#define CITIESMODELPROXY_H

#include <QSortFilterProxyModel>

#include "citiesmodel.h"

class CitiesModelProxy : public QSortFilterProxyModel
{
public:
    CitiesModelProxy();
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    CitiesModel model;
};

#endif // CITIESMODELPROXY_H
