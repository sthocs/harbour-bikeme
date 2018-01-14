#ifndef STATIONSFAVOURITESPROXY_H
#define STATIONSFAVOURITESPROXY_H

#include <QSortFilterProxyModel>
#include <QList>

class StationsFavouritesProxy : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    StationsFavouritesProxy();

    Q_INVOKABLE void setFavourites(QList<int> favourites);
    Q_INVOKABLE bool add(int stationNumber);
    Q_INVOKABLE void remove(int stationNumber);
    Q_INVOKABLE void removeAt(int index);
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE bool refreshStationInfo(int row);
    Q_INVOKABLE void refreshAll();

    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:
    void countChanged();

private:
    QList<int> _favourites;
};

#endif // STATIONSFAVOURITESPROXY_H
