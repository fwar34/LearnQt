#include "globalconfig.h"
#include "CommonDefine.h"
#include <QDebug>
#include <QDir>

GlobalConfig::GlobalConfig()
{
    Init();
}

void GlobalConfig::Init()
{
    // qDebug() << "before read config.ini, currentPath: " << QDir::currentPath();
    if (!QFile::exists("./config.ini")) {
        qDebug() << "config.ini not exist!";
        return;
    }

    QSettings settings("./config.ini", QSettings::IniFormat);
    // settings.beginGroup("Application");
    // qDebug() << settings.allKeys();
    // if (settings.contains(PROJECT_DIR)) {
    //     qDebug() << "test ok!";
    // }
    // settings.endGroup();
    // settings_.reset(new QSettings("config.ini", QSettings::IniFormat));
    settings_ = std::move(std::make_unique<QSettings>("./config.ini", QSettings::IniFormat));

    QStringList sections = settings_->childGroups();
    for (const QString &section : sections) {
        settings_->beginGroup(section);
        QStringList keys = settings_->childKeys();
        for (const QString &key : keys) {
            configs_.insert(key, settings_->value(key, QDir::currentPath()).toString());
            if (key == PROJECT_DIR && configs_[key].isEmpty()) {
                configs_[key] = QDir::currentPath();
            }
            qDebug() << QStringLiteral("insert %1:%2").arg(key, configs_[key]);
        }
        settings_->endGroup();
    }
}

void GlobalConfig::SetConfig(const QString &key, const QString &value)
{
    configs_[key] = value;
}

QString GlobalConfig::GetConfig(const QString &key)
{
    auto it = configs_.find(key);
    if (it == configs_.end()) {
        return "";
    }

    return it.value();
}
