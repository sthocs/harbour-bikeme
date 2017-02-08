#include "citiesmodelproxy.h"

CitiesModelProxy::CitiesModelProxy()
{
    setDynamicSortFilter(true);
    sort(0);
}

void CitiesModelProxy::filter(QString filter)
{
    setFilterRegExp(QRegExp(filter, Qt::CaseInsensitive, QRegExp::FixedString));
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

bool CitiesModelProxy::filterAcceptsRow(int sourceRow,
                                        const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    return sourceModel()->data(index, CitiesModel::CityNameRole).toString().contains(filterRegExp())
            || sourceModel()->data(index, CitiesModel::CommercialNameRole).toString().contains(filterRegExp());
}
