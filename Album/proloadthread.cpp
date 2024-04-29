#include "proloadthread.h"
#include "protreewidget.h"
#include "protreeitem.h"
#include <QMap>
#include <QString>
#include <QDebug>
#include <QDir>

ProLoadThread::ProLoadThread(QTreeWidget *treeWidget, QObject *parent) : QObject(parent), treeWidget_(treeWidget)
{
}

ProLoadThread::~ProLoadThread()
{
    loadThread_.quit();
    loadThread_.wait();
}

void ProLoadThread::Init()
{
    moveToThread(&loadThread_);
    loadThread_.start();
    qDebug() << "------------------- load thread start";

    ProTreeWidget *proWidget = dynamic_cast<ProTreeWidget *>(treeWidget_);
    if (!proWidget) {
        qDebug() << "cast to ProTreeWidget failed!";
        return;
    }
    // A. 去数据库读取项目信息
    QMetaObject::invokeMethod(proWidget->GetProDbQuery(), "GetProDirectories");
}

// A. 项目信息回调槽
void ProLoadThread::SlotQueryResult(QSharedPointer<DbResult> results)
{
    for (const auto &result : *results) {
        const QString &name = result["name"];
        const QString &path = result["path"];
        auto *proTreeItem = new ProTreeItem(ProTreeItem::ITEM_PRO, treeWidget_, name, path);
        proTreeItem->setData(0, Qt::DisplayRole, name);
        proTreeItem->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
        proTreeItem->setData(0, Qt::ToolTipRole, path);
        LoadProject(name, path, proTreeItem, proTreeItem, nullptr);
    }
}

void ProLoadThread::LoadProject(const QString &name,
                                const QString &path,
                                QTreeWidgetItem *root,
                                QTreeWidgetItem *parent,
                                QTreeWidgetItem *prev)
{
    QDir dirPath(path);
    if (!dirPath.exists()) {
        qDebug() << QStringLiteral("dirPath[%1] not exist").arg(path);
        return;
    }

    dirPath.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    dirPath.setSorting(QDir::Name);
    for (const auto& entryInfo : dirPath.entryInfoList()) {
        if (entryInfo.isDir()) {
            auto *item = new ProTreeItem(ProTreeItem::ITEM_DIR, root, parent, entryInfo.fileName(), entryInfo.path());
            item->setData(0, Qt::DisplayRole, entryInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, entryInfo.filePath());
            item->setExpanded(true);
            LoadProject(entryInfo.fileName(), entryInfo.filePath(), root, item, nullptr);
        } else if (entryInfo.isFile()) {
            QString suffix = entryInfo.suffix();
            if (suffix != "bmp" && suffix != "png") {
                continue;
            }
            auto *item = new ProTreeItem(ProTreeItem::ITEM_PIC, root, parent, entryInfo.fileName(), entryInfo.filePath());
            item->setData(0, Qt::DisplayRole, entryInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, entryInfo.filePath());
            if (prev) {
                ProTreeItem *pitem = dynamic_cast<ProTreeItem*>(prev);
                pitem->SetNext(item);
            }
            item->SetPrev(prev);
        }
    }
}
