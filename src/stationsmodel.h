#ifndef STATIONSMODEL_H
#define STATIONSMODEL_H

#include <QAbstractListModel>
#include <QGeoCoordinate>

#include "station.h"
#include "stationsloader.h"

class StationsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(QGeoCoordinate center READ getCenter NOTIFY centerChanged)
    Q_PROPERTY(QString providerName READ getProviderName WRITE setProviderName)
    Q_PROPERTY(QString cityName READ getCityName WRITE setCityName)
    Q_PROPERTY(QUrl allStationsDetailsUrl READ getAllStationsDetailsUrl WRITE setAllStationsDetailsUrl)
    Q_PROPERTY(QString stationDetailsUrlTemplate READ getStationDetailsUrlTemplate WRITE setStationDetailsUrlTemplate)

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

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE bool loadStationsList();
    Q_INVOKABLE bool fetchStationInformation(int index);
    void fetchStationsInformation(QList<QModelIndex> indexes);

    QGeoCoordinate getCenter() const;
    QString getProviderName() const;
    QString getCityName() const;
    QUrl getAllStationsDetailsUrl() const;
    QString getStationDetailsUrlTemplate() const;

    void setProviderName(QString providerName);
    void setCityName(QString cityName);
    void setAllStationsDetailsUrl(QUrl allStationsDetailsUrl);
    void setStationDetailsUrlTemplate(QString stationDetailsUrlTemplate);

    // pure virtuals of QAbstractListModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
    void countChanged();
    void centerChanged();
    void stationUpdated(Station* station);
    void stationsLoaded(bool withDetails);

private slots:
    void setStationsList(QList<Station*> stations);
    void addStations(QList<Station*> stations);
    void stationDetailsFetched(Station* station);

private:
    QList<Station*> _list;
    QGeoCoordinate _center;
    QString _providerName;
    QString _cityName;
    QUrl _allStationsDetailsUrl;
    QString _singleStationDetailsUrlTemplate;

    StationsLoader _stationsLoader;

    void updateCenter();
};

#endif // STATIONSMODEL_H
