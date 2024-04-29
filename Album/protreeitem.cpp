#include "protreeitem.h"

ProTreeItem::ProTreeItem(int type, QTreeWidget *view, const QString &name, const QString &path) :
    QTreeWidgetItem(view, type), name_(name), path_(path), root_(this), prev_(nullptr), next_(nullptr)
{
}

ProTreeItem::ProTreeItem(int type, QTreeWidgetItem *root, QTreeWidgetItem *parent, const QString &name, const QString &path) :
    QTreeWidgetItem(parent, type), name_(name), path_(path), root_(root), prev_(nullptr), next_(nullptr)
{
}

// ProTreeItem::ProTreeItem(int type, QTreeWidgetItem *parent, const QString &name, const QString &path) :
//     QTreeWidgetItem(parent, type), name_(name), path_(path)
// {
// }
