#ifndef STATIONSMODEL_H
#define STATIONSMODEL_H

#include <QAbstractListModel>
#include <QGeoCoordinate>

#include "station.h"
#include "stationsloader.h"

class StationsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate center READ getCenter NOTIFY centerChanged)
    Q_PROPERTY(QString providerName READ getProviderName WRITE setProviderName)
    Q_PROPERTY(QUrl allStationsDetailsUrl READ getAllStationsDetailsUrl WRITE setAllStationsDetailsUrl)

public:
    enum Roles {
        NumberRole = Qt::UserRole + 1,
        CoordinatesRole,
        BikesNbRole,
        FreeSlotsNbRole
    };

    explicit StationsModel(QObject *parent = 0);

    Q_INVOKABLE void loadAll();

    QGeoCoordinate getCenter() const;
    QString getProviderName() const;
    QUrl getAllStationsDetailsUrl() const;

    void setProviderName(QString providerName);
    void setAllStationsDetailsUrl(QUrl allStationsDetailsUrl);

    // pure virtuals of QAbstractListModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
    void centerChanged();
    void stationsLoaded();

private slots:
    void addStations(QList<Station*> stations);

private:
    QList<Station*> _list;
    QGeoCoordinate _center;
    QString _providerName;
    QUrl _allStationsDetailsUrl;

    StationsLoader _stationsLoader;

    void updateCenter();
};

#endif // STATIONSMODEL_H
