#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidgetItem>

class ProTreeItem : public QTreeWidgetItem
{
public:
    enum ProTreeItemType
    {
        ITEM_ROOT,
        ITEM_PRO,
        ITEM_PIC,
        ITEM_DIR
    };
    ProTreeItem(int type, QTreeWidget *view, const QString &name, const QString &path);
    ProTreeItem(int type, QTreeWidgetItem *root, QTreeWidgetItem *parent, const QString &name, const QString &path);
    // ProTreeItem(int type, QTreeWidgetItem *parent, const QString &name, const QString &path);
    const QString& GetName() const
    {
        return name_;
    }
    const QString& GetPath() const
    {
        return path_;
    }
    void SetPrev(QTreeWidgetItem *prev)
    {
        prev_ = prev;
    }
    void SetNext(QTreeWidgetItem *next)
    {
        next_ = next;
    }
    QTreeWidgetItem* GetPrev()
    {
        return prev_;
    }
    QTreeWidgetItem* GetNext()
    {
        return next_;
    }

private:
    QString name_;
    QString path_;
    QTreeWidgetItem *root_;
    QTreeWidgetItem *prev_;
    QTreeWidgetItem *next_;
};

#endif // PROTREEITEM_H
