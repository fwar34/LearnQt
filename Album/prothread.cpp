#include "prothread.h"
#include "protreeitem.h"
#include <QDir>

void ProThread::run()
{
    CreateProTree(srcPath_, dstPath_, root_, parentItem_, nullptr);
    if (stop_) {
        // 如果线程停止
        auto index = self_->indexOfTopLevelItem(root_);
        delete self_->takeTopLevelItem(index);
        QDir dstDir(dstPath_);
        dstDir.removeRecursively();
        // progressDialog_->reject(); 这里不能在非主线程操作 gui 组件
        QMetaObject::invokeMethod(progressDialog_, "reject");
        return;
    }

    qDebug() << "ProThread exit... fileCount_: " << fileCount_;
    // progressDialog_->accept(); 这里不能在非主线程操作 gui 组件
    QMetaObject::invokeMethod(progressDialog_, "accept");
    qDebug() << "ProThread exit2... fileCount_: " << fileCount_;
    emit SigProThreadFinish();
    qDebug() << "ProThread exit3... fileCount_: " << fileCount_;
}

void ProThread::Stop()
{
    stop_ = true;
}

/**
 * @brief ProThread::CreateProTree 递归的去拷贝文件夹
 * @param src 原文件夹路径
 * @param dst 拷贝到目标文件夹路径
 * @param parent 父节点
 * @param prev 前一个 item 节点
 * @param column 显示的列
 */
void ProThread::CreateProTree(const QString &src, const QString &dst, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *prev)
{
    if (stop_) {
        return;
    }

    QDir srcDir(src);
    if (!srcDir.exists()) {
        qDebug() << QStringLiteral("path[%1] not exist!").arg(src);
        return;
    }

    if (src == dst) {
        return;
    }

    srcDir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    srcDir.setSorting(QDir::Name);
    QFileInfoList files = srcDir.entryInfoList();
    for (auto& fileInfo : files) {
        if (stop_) {
            return;
        }
        // 发送信号更新进度条（目录也包含在内）
        emit SigProThreadProgess(++fileCount_);

        if (fileInfo.isDir()) {
            QDir dstSubDir(dst);
            QString dstSubPath = dstSubDir.absoluteFilePath(fileInfo.fileName());
            QDir dstSubDir2(dstSubPath);
            if (!dstSubDir2.exists()) {
                dstSubDir2.mkpath(dstSubPath);
            }
            ProTreeItem* item = new ProTreeItem(ProTreeItem::ITEM_DIR, root, parent, fileInfo.fileName(), dstSubPath);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, dstSubPath);
            // item->setExpanded(true); // 目录默认展开
            CreateProTree(fileInfo.absoluteFilePath(), dstSubPath, root, item, prev); // 目录节点不设置 prev_ 字段，只有图片节点有 prev_
        } else {
            if (stop_) {
                return;
            }
            QString suffix = fileInfo.suffix();
            if (suffix != "bmp" && suffix != "png" && suffix != "jpg" && suffix != "jpeg") {
                continue;
            }
            auto item = new ProTreeItem(ProTreeItem::ITEM_PIC, root, parent, fileInfo.fileName(), fileInfo.filePath());
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.path());
            QDir dstDir(dst);
            QString dstFilePath = dstDir.absoluteFilePath(fileInfo.fileName());
            if (!QFile::copy(fileInfo.absoluteFilePath(), dstFilePath)) {
                continue;
            }

            // prev 不为空的时候设置 prev 的 next_为新添加的节点
            if (prev) {
                ProTreeItem* prevItem = dynamic_cast<ProTreeItem*>(prev);
                prevItem->SetNext(item);
            }
            // 设置新添加的节点 item 的 prev_，并且将 prev 指向新节点
            item->SetPrev(prev);
            prev = item;

            emit SigProThreadProgess(++fileCount_);
        }
    }
    // 这里不能设置 parentItem_->setExanded，因为非 gui 主线程操作界面元素会出现问题（ui 元素是主线程绑定的，可能会出现竟态条件，出现线程安全问题，程序异常）
    // 所以这里发送信号到 ProTreeWidget 中去处理
    // parent->setExpanded(true);
    emit SigExpandItem(parent);
}
