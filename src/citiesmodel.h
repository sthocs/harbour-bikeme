#ifndef CITIESMODEL_H
#define CITIESMODEL_H

#include <QAbstractListModel>

#include "city.h"
#include "citiesloader.h"

class CitiesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        CountryRole = Qt::UserRole + 1,
        CityNameRole,
        CommercialNameRole,
        FlagRole
    };

    explicit CitiesModel(QObject *parent = 0);

    Q_INVOKABLE void loadAll();

    // pure virtuals of QAbstractListModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private slots:
    void addCities(QList<City*> cities);

private:
    QList<City*> _list;

    CitiesLoader citiesLoader;
};

#endif // CITIESMODEL_H
