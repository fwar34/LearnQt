#include "prodb.h"
#include "globalconfig.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include <QMap>
#include <QVector>

ProDb::ProDb() : port_(0)
{
    Init();
}

bool ProDb::Init()
{
    host_ = GlobalConfig::GetInstance().GetConfig("host");
    port_ = GlobalConfig::GetInstance().GetConfig("port").toInt();
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    // db_.setHostName(host_);
    db_.setDatabaseName("qt-album.db");
    // db_.setUserName("feng");
    // db_.setPassword("1111");

    if (!db_.open()) {
        qDebug() << "open db failed!";
        return false;
    }

    return true;
}

void ProDb::UnInit()
{
    db_.close();
}

bool ProDb::GetProDirectories(QVector<QMap<QString, QString>> &results)
{
    QSqlQuery query;
    if (!query.exec("select * from projects")) {
        qDebug() << "execute sql [select * from projects] failed!";
        return false;
    }

    QMap<QString, QString> result;
    while (query.next()) {
        for (int i = 0; i < query.record().count(); ++i) {
            result.insert(query.record().fieldName(i), query.record().value(i).toString());
        }
        results.push_back(std::move(result));
    }
    return true;
}

bool ProDb::InsertProDirectory(const QString &name, const QString &path)
{
    QString insertStatement = "insert into projects (name, path, date) vlues (?, ?, CURRENT_TIMESTAMP)";
    QSqlQuery query;
    query.prepare(insertStatement);
    query.bindValue(0, name);
    query.bindValue(1, path);
    if (!query.exec()) {
        qDebug() << QStringLiteral("insert project failed! name: %1, path: %2").arg(name, path);
        return false;
    }

    return true;
}
