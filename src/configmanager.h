#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QSettings>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    ConfigManager();

    Q_INVOKABLE void saveSetting(QString, QString);
    Q_INVOKABLE void savePage(int, QString);
    Q_INVOKABLE void popPage();
    Q_INVOKABLE QList<QString> getPages();
    Q_INVOKABLE QVariant getSetting(QString);
    Q_INVOKABLE bool removeCacheFiles();

private:
    QSettings _settings;
};

#endif // CONFIGMANAGER_H
