#include <QDebug>

#include "citiesmodelproxy.h"

CitiesModelProxy::CitiesModelProxy()
{
    setDynamicSortFilter(true);
    sort(0);
}

bool CitiesModelProxy::lessThan(const QModelIndex &left,
                                const QModelIndex &right) const
{
    QString l_country = sourceModel()->data(left, CitiesModel::CountryRole).toString();
    QString r_country = sourceModel()->data(right, CitiesModel::CountryRole).toString();
    if (l_country.compare(r_country) == 0) {
        QString l_name = sourceModel()->data(left, CitiesModel::CityNameRole).toString();
        QString r_name = sourceModel()->data(right, CitiesModel::CityNameRole).toString();
        return l_name.compare(r_name) < 0;
    }
    return l_country.compare(r_country) < 0;
}
