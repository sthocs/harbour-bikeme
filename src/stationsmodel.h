#ifndef STATIONSMODEL_H
#define STATIONSMODEL_H

#include <QAbstractListModel>
#include <QGeoCoordinate>

#include "city.h"
#include "station.h"
#include "stationsloader.h"

class StationsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(QGeoCoordinate center READ getCenter NOTIFY centerChanged)
    Q_PROPERTY(City* city MEMBER _city WRITE setCity)

public:
    enum Roles {
        NumberRole = Qt::UserRole + 1,
        NameRole,
        OpenedRole,
        CoordinatesRole,
        BikesNbRole,
        FreeSlotsNbRole,
        LastUpdateRole
    };

    explicit StationsModel(QObject *parent = 0);
    ~StationsModel();

    Q_INVOKABLE void loadStationsList();
    Q_INVOKABLE void loadAllStationsDetails();
    Q_INVOKABLE bool fetchStationInformation(int index);
    Q_INVOKABLE bool exists(int number);
    void fetchStationsInformation(QList<QModelIndex> indexes);

    QGeoCoordinate getCenter() const;

    void setCity(City* city);

    // pure virtuals of QAbstractListModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
    void countChanged();
    void centerChanged();
    void stationUpdated(Station* station);
    void stationsLoaded(bool withDetails);
    void error(QString errorMsg);

private slots:
    void setStations(QList<Station*> stations, bool withDetails);
    void stationDetailsFetched(Station* station);

private:
    QList<Station*> _list;
    QGeoCoordinate _center;
    City* _city;

    StationsLoader _stationsLoader;

    void updateCenter();
};

#endif // STATIONSMODEL_H
