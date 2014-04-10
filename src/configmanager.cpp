#include "configmanager.h"

#include <QSettings>

ConfigManager::ConfigManager()
{
}

void ConfigManager::saveSetting(QString key, QString value)
{
    _settings.setValue(key, value);
}

void ConfigManager::savePage(int depth, QString pageName)
{
    _settings.beginWriteArray("pages");
    _settings.setArrayIndex(depth);
    _settings.setValue("name", pageName);
    _settings.endArray();
}

void ConfigManager::popPage()
{
    QList<QVariant> list;
    int size = _settings.beginReadArray("pages");
    for (int i = 0; i < size - 1; ++i) {
        _settings.setArrayIndex(i);
        list.append(_settings.value("name"));
    }
    _settings.endArray();
    _settings.remove("pages");
    _settings.beginWriteArray("pages");
    int i = 0;
    foreach (QVariant entry, list) {
        _settings.setArrayIndex(i++);
        _settings.setValue("name", entry);
    }
    _settings.endArray();
}

QVariant ConfigManager::getSetting(QString key)
{
    return _settings.value(key);
}

QList<QString> ConfigManager::getPages()
{
    int size = _settings.beginReadArray("pages");
    QList<QString> res;
    for (int i = 0; i < size; ++i) {
        _settings.setArrayIndex(i);
        res.append(_settings.value("name").toString());
    }
    _settings.endArray();
    return res;
}
