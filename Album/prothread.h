#ifndef PROTHREAD_H
#define PROTHREAD_H

#include <QThread>
#include <QTreeWidget>
#include <QProgressDialog>

class ProThread : public QThread
{
    Q_OBJECT
public:
    ProThread(const QString &srcPath, const QString &dstPath,
        QTreeWidgetItem *parentItem, QTreeWidget *self, QTreeWidgetItem *root, QObject *parent, QProgressDialog *dialog) :
        QThread(parent), srcPath_(srcPath), dstPath_(dstPath),
        parentItem_(parentItem), self_(self), root_(root), stop_(false), progressDialog_(dialog)
    {
    }
    void Stop();

protected:
    void run() override;

private:
    void CreateProTree(const QString &src, const QString &dst, QTreeWidgetItem *root, QTreeWidgetItem* parent, QTreeWidgetItem *prev);
    QString srcPath_;
    QString dstPath_;
    size_t fileCount_;
    QTreeWidgetItem *parentItem_; // 右键点击的 ProTreeItem 节点
    QTreeWidget *self_; // 这个传递的是 ProTreeWidget
    QTreeWidgetItem *root_; // 右键点击的 ProTreeItem 节点
    bool stop_;
    QProgressDialog *progressDialog_;

signals:
    void SigProThreadProgess(size_t fileCount);
    void SigProThreadFinish();
    void SigExpandItem(QTreeWidgetItem *item);
};

#endif // PROTHREAD_H
