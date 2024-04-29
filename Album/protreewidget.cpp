#include "protreewidget.h"
#include "globalconfig.h"
#include "protreeitem.h"
#include "prothread.h"
#include "prodbquery.h"
#include "proloadthread.h"
#include "CommonDefine.h"
#include <QGuiApplication>
#include <QHeaderView>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QMenu>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>
#include <QThread>

ProTreeWidget::ProTreeWidget(QWidget *parent) : QTreeWidget(parent),
    rightClickItem_(nullptr), selectedItem_(nullptr), activeItem_(nullptr)
{
    header()->hide();
    connect(this, &QTreeWidget::itemPressed, this, &ProTreeWidget::SlotItemPressed);
    importAction_ = new QAction(QIcon(":/icon/import.png"), tr("导入相片"), this);
    closeAction_ = new QAction(QIcon(":/icon/close.png"), tr("关闭项目"), this);
    slideShowAction_ = new QAction(QIcon(":/icon/slideshow.png"), tr("播放幻灯片"), this);
    activeAction_ = new QAction(QIcon(":/icon/core.png"), tr("设置为活动"), this);
    connect(importAction_, &QAction::triggered, this, &ProTreeWidget::SlotImport);
    connect(this, &QTreeWidget::itemDoubleClicked, this, &ProTreeWidget::SlotItemDoubleClicked);

    dbQuery_ = new ProDbQuery();
    loadThread_ = new ProLoadThread(this);

    // 创建数据库线程
    dbQuery_->Init(loadThread_, &ProLoadThread::SlotQueryResult);
    // 创建加载线程
    // std::shared_ptr<ProLoadThread> loadThread_ = std::make_shared<ProLoadThread>(this);
    loadThread_->Init();
}

ProTreeWidget::~ProTreeWidget()
{
    qDebug() << "~ProTreeWidget delete loadThread_";
    delete loadThread_;
    loadThread_ = nullptr;
    delete dbQuery_;
    dbQuery_ = nullptr;
}

void ProTreeWidget::AddProTree(const QString &dstProjectName, const QString &srcPath)
{
    qDebug() << "ProTreeWidget::AddProTree called!";
    QDir srcDir(srcPath);
    if (!srcDir.exists()) {
        qDebug() << QStringLiteral("path %1 not exist!").arg(srcPath);
        return;
    }

    QString proPath = GlobalConfig::GetInstance().GetConfig(PROJECT_DIR);
    QDir proDir(proPath);
    QString dstProjectPath = proDir.absoluteFilePath(dstProjectName);
    QDir dstProPath(dstProjectPath);
    if (dstProPath.exists()) {
        qDebug() << "[" << dstProPath << "] already exist!";
        return;
    }

    auto it = projects_.find(dstProjectPath);
    if (it != projects_.end()) {
        qDebug() << QStringLiteral("Project %1 exist!").arg(dstProjectPath);
        return;
    }

    // 构造项目文件夹
    if (!dstProPath.mkpath(".")) {
        qDebug() << "mkpath " << dstProjectPath << " failed!";
        return;
    }
    projects_.insert(dstProjectPath);

    // 构造 item 的时候就会将 item 添加到 this 去
    auto item = new ProTreeItem(ProTreeItem::ITEM_PRO, this, dstProjectName, dstProjectPath);
    item->setData(0, Qt::DisplayRole, dstProjectName);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, dstProjectPath);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *item, int column)
{
    qDebug() << "item pressed";
    ProTreeItem *pressedItem = dynamic_cast<ProTreeItem*>(item);
    if (!pressedItem) {
        return;
    }

    rightClickItem_ = item;
    if (QGuiApplication::mouseButtons() == Qt::RightButton) { // 如果是鼠标右键
        if (item->type() == ProTreeItem::ITEM_PRO) {
            QMenu menu(this);
            menu.addAction(importAction_);
            menu.addAction(closeAction_);
            menu.addAction(activeAction_);
            menu.addAction(slideShowAction_);
            menu.exec(QCursor::pos());
        }
    }
}

void ProTreeWidget::SlotImport(bool checked)
{
    ProTreeItem *item = dynamic_cast<ProTreeItem*>(rightClickItem_);
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("选择导入的文件"));
    fileDialog.setDirectory(item->GetPath());
    fileDialog.setViewMode(QFileDialog::Detail);
    QStringList files;
    if (fileDialog.exec()) {
        files = fileDialog.selectedFiles();
    }

    if (files.empty()) {
        return;
    }

    QString importFile = files[0];

    QProgressDialog progressDialog;
    // size_t fileCount = 0;
    std::shared_ptr<ProThread> proThread =
        std::make_shared<ProThread>(std::ref(importFile), std::ref(item->GetPath()),
                                    item, this, item, nullptr, &progressDialog);

    connect(proThread.get(), &ProThread::SigProThreadProgess, this, [&progressDialog] (size_t fileCount) {
        progressDialog.setValue(fileCount % MAX_FILE_COUNT);
    });
    connect(proThread.get(), &ProThread::SigProThreadFinish, this, [&progressDialog] {
        progressDialog.setValue(MAX_FILE_COUNT);
    });
    connect(&progressDialog, &QProgressDialog::canceled, proThread.get(), [proThread] {
        proThread->Stop();
    });
    connect(proThread.get(), &ProThread::SigExpandItem, this, [](QTreeWidgetItem *item) {
        if (item) {
            item->setExpanded(true);
        }
    });
    qDebug() << "progressDialog thread: " << progressDialog.thread()->currentThreadId();
    qDebug() << "proThread thread: " << proThread->currentThreadId();

    progressDialog.setWindowTitle(tr("正在导入..."));
    progressDialog.setFixedWidth(PROGRESS_WIDTH);
    progressDialog.setRange(0, PROGRESS_MAX);

    proThread->start();
    int result = progressDialog.exec();

    if (result == QProgressDialog::Accepted) {
        dbQuery_->InsertProDirectory(item->GetName(), item->GetPath());
        qDebug() << QStringLiteral("insert name(%1) path(%2) to db success!").arg(item->GetName(), item->GetPath());
        return;
    }
}

void ProTreeWidget::SlotItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (QGuiApplication::mouseButtons() == Qt::LeftButton) {
        ProTreeItem *proItem = dynamic_cast<ProTreeItem*>(item);
        if (!proItem) {
            qDebug() << "SlotItemDoubleClicked item dynamic_cast failed!";
            return;
        }
        if (item->type() == ProTreeItem::ITEM_PIC) {
            emit SigUpdateSelected(proItem->GetPath());
            selectedItem_ = item;
        }
    }
}

void ProTreeWidget::SlotPreviousPic()
{
    if (!selectedItem_) {
        return;
    }
    ProTreeItem *currentItem = dynamic_cast<ProTreeItem*>(selectedItem_);
    if (!currentItem) {
        qDebug() << "dynamic_cast selectedItem failed!";
        return;
    }
    if (currentItem->GetPrev()) {
        emit SigUpdateSelected(currentItem->GetPath());
        selectedItem_ = currentItem->GetPrev();
        setCurrentItem(selectedItem_);
    }
}

void ProTreeWidget::SlotNextPic()
{
    if (!selectedItem_) {
        return;
    }
    ProTreeItem *currentItem = dynamic_cast<ProTreeItem*>(selectedItem_);
    if (!currentItem) {
        qDebug() << "dynamic_cast selectedItem failed!";
        return;
    }

    if (currentItem->GetNext()) {
        emit SigUpdateSelected(currentItem->GetPath());
        selectedItem_ = currentItem->GetNext();
        setCurrentItem(selectedItem_);
    }
}
