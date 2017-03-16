#ifndef CITIESMODEL_H
#define CITIESMODEL_H

#include <QAbstractListModel>

#include "city.h"
#include "citiesloader.h"

class CitiesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int providersCount READ providersCount NOTIFY providersCountChanged)
    Q_PROPERTY(int fetchedProvidersCount READ fetchedProvidersCount NOTIFY fetchedProvidersCountChanged)
    Q_PROPERTY(int errorsCount READ errorsCount NOTIFY errorsCountChanged)
    Q_PROPERTY(QString errorMsg READ errorMsg NOTIFY errorMsgChanged)

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

    int providersCount() const;
    int fetchedProvidersCount() const;
    int errorsCount() const;
    QString errorMsg() const;

    // pure virtuals of QAbstractListModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
    void countChanged();
    void providersCountChanged();
    void fetchedProvidersCountChanged();
    void errorsCountChanged();
    void errorMsgChanged();
    void fetchFinished();

private slots:
    void addCities(QList<City*> cities);
    void setProvidersCount(int count);
    void setFetchedProvidersCount(int count, int errorsCount);
    void setErrorMsg(QString errorMsg);

private:
    QList<City*> _list;

    CitiesLoader citiesLoader;
    int _providersCount;
    int _fetchedProvidersCount;
    int _errorsCount;
    QString _errorMsg;
};

#endif // CITIESMODEL_H
