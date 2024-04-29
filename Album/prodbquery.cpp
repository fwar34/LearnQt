#include "prodbquery.h"
#include "CommonDefine.h"
#include "proloadthread.h"
#include <QVariantList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QThread>
#include <QSqlError>

ProDbQuery::ProDbQuery(QObject *parent) : QObject(parent), db_(QSqlDatabase::addDatabase("QSQLITE"))
{
}

ProDbQuery::~ProDbQuery()
{
    dbWorkThread_.quit();
    dbWorkThread_.wait();
    db_.close();
}

bool ProDbQuery::Init(ProLoadThread *receiver, CallBack callBack)
{
    QString dbName = "qt-album.db";
    db_.setDatabaseName(dbName);
    if (!db_.open()) {
        qDebug() << QStringLiteral("open sqlite[%1] failed!").arg(dbName);
        return false;
    }
    connect(this, &ProDbQuery::SigQueryDone, receiver, callBack);

    moveToThread(&dbWorkThread_);
    dbWorkThread_.start();

    return true;
}

void ProDbQuery::GetProDirectories()
{
    QString sql = "select * from projects";
    QSharedPointer<DbResult> results(new DbResult);
    executeQuery(sql, QVariantList{}, results);
    emit SigQueryDone(results);
    qDebug() << "GetProDirectories called!";
}

void ProDbQuery::InsertProDirectory(const QString &name, const QString &path)
{
    QString sql = "insert into projects (name, path, date) values (?, ?, CURRENT_TIMESTAMP)";
    QVariantList values{name, path};
    executeQuery(sql, values, nullptr);
    qDebug() << QStringLiteral("insert values name(%1) path(%2)").arg(name, path);
}

void ProDbQuery::executeQuery(const QString &query, const QVariantList &params, QSharedPointer<DbResult> result)
{
    QSqlQuery sqlQuery(db_);
    sqlQuery.prepare(query);

    for (int i = 0; i < params.count(); ++i) {
        sqlQuery.addBindValue(params[i]);
    }

    if (!sqlQuery.exec()) {
        qDebug() << "SQL query failed!" << sqlQuery.lastError().text();
        return;
    }

    if (result) {
        QMap<QString, QString> record;
        while (sqlQuery.next()) {
            for (int i = 0; i < sqlQuery.record().count(); ++i) {
                record.insert(sqlQuery.record().fieldName(i), sqlQuery.record().value(i).toString());
            }
            result->push_back(std::move(record));
        }
    }
}

