#ifndef PRODB_H
#define PRODB_H

#include "Singleton.h"
#include <QSqlDatabase>
#include <QString>

class ProDb : public Singleton<ProDb>
{
    friend class Singleton<ProDb>;
public:
    bool Init();
    void UnInit();
    bool GetProDirectories(QVector<QMap<QString, QString>> &result);
    bool InsertProDirectory(const QString &name, const QString &path);

private:
    ProDb();
    bool Execute(const QString &query);

    QSqlDatabase db_;
    QString host_;
    uint16_t port_;
};

#endif // PRODB_H
