#ifndef PROLOADTHREAD_H
#define PROLOADTHREAD_H

#include "CommonDefine.h"
#include <QThread>
#include <QString>
#include <QTreeWidgetItem>
#include <QSharedPointer>

class ProLoadThread : public QObject
{
public:
    explicit ProLoadThread(QTreeWidget *treeWidget, QObject *parent = nullptr);
    ~ProLoadThread();
    void Init();

public slots:
    void SlotQueryResult(QSharedPointer<DbResult> results);

private:
    void LoadProject(const QString &name, const QString &path, QTreeWidgetItem *root,
                     QTreeWidgetItem *parent, QTreeWidgetItem *prev);
    QTreeWidget *treeWidget_ = nullptr;
    QThread loadThread_;
};

#endif // PROLOADTHREAD_H
