#ifndef CITIESMODEL_H
#define CITIESMODEL_H

#include <QAbstractListModel>

#include "city.h"
#include "citiesloader.h"

class CitiesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        CountryRole = Qt::UserRole + 1,
        CityNameRole,
        CommercialNameRole,
        FlagRole
    };

    explicit CitiesModel(QObject *parent = 0);

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE City* cityAt(int row);

    int count() const;

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
