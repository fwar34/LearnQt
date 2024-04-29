#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H
#include "Singleton.h"
#include <QString>
#include <QSettings>
#include <QMap>

class GlobalConfig : public Singleton<GlobalConfig>
{
    friend class Singleton<GlobalConfig>;
public:
    void SetConfig(const QString& key, const QString& value);
    QString GetConfig(const QString& key);

private:
    GlobalConfig();
    void Init();
    QMap<QString, QString> configs_;
    std::unique_ptr<QSettings> settings_;
};

#endif // GLOBALCONFIG_H
