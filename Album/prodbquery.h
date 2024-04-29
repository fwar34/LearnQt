#ifndef PRODBQUERY_H
#define PRODBQUERY_H

#include "CommonDefine.h"
#include <QSqlDatabase>
#include <QString>
#include <QList>
#include <QThread>
#include <QObject>
#include <QSharedPointer>

class ProLoadThread;
/**
 * @brief 每个查询都是在线程池的线程中执行
 */
class ProDbQuery : public QObject
{
    Q_OBJECT
public:
    // template<typename T>
    // using ResultType = QList<QMap<QString, QString>>;
    using CallBack = void (ProLoadThread::*)(QSharedPointer<DbResult> result);
    ProDbQuery(QObject *parent = nullptr);
    ~ProDbQuery();
    // template<typename T>
    bool Init(ProLoadThread *receiver, CallBack callBack);
    void InsertProDirectory(const QString &name, const QString &path);

public slots:
    void GetProDirectories();

private:
    void executeQuery(const QString &query, const QVariantList &params, QSharedPointer<DbResult> results);

    QSqlDatabase db_;
    QList<QString> tasks_;
    QThread dbWorkThread_;

signals:
    // void SigQueryDone(std::unique_ptr<QList<QMap<QString, QString>>> &result);
    void SigQueryDone(QSharedPointer<DbResult> results);
};


#endif // PRODBQUERY_H
