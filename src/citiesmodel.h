#ifndef CITIESMODEL_H
#define CITIESMODEL_H

#include <QAbstractListModel>

#include "city.h"
#include "citiesloader.h"

class CitiesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        CountryRole,
        CityNameRole,
        CommercialNameRole,
        FlagRole
    };

    explicit CitiesModel(QObject *parent = 0);
    ~CitiesModel();

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE City* cityAt(int row);
    Q_INVOKABLE City* get(QString identifier);

    // pure virtuals of QAbstractListModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
    void countChanged();

private slots:
    void addCities(QList<City*> cities);

private:
    QList<City*> _list;

    CitiesLoader citiesLoader;
};

#endif // CITIESMODEL_H
