#ifndef STATIONSFAVOURITESPROXY_H
#define STATIONSFAVOURITESPROXY_H

#include <QSortFilterProxyModel>
#include <QList>

class StationsFavouritesProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    StationsFavouritesProxy();

    Q_INVOKABLE void setFavourites(QList<int> favourites);
    Q_INVOKABLE bool add(int stationNumber);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE bool refreshStationInfo(int row);
    Q_INVOKABLE void refreshAll();

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QList<int> _favourites;
};

#endif // STATIONSFAVOURITESPROXY_H
