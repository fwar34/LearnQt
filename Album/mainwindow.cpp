#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "wizard.h"
#include "protree.h"
#include "picshow.h"
#include "protreewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":app_icon"));
    // 文件菜单
    QMenu* menuFile = menuBar()->addMenu(tr("文件(&F)"));
    QAction* actionCreatePro = new QAction(QIcon(":/icon/createpro.png"), tr("创建项目"), this);
    actionCreatePro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    menuFile->addAction(actionCreatePro);

    QAction* actionOpenPro = new QAction(QIcon(":/icon/openpro.png"), tr("打开项目"), this);
    actionOpenPro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    menuFile->addAction(actionOpenPro);

    // 设置菜单
    QMenu* menuSetting = menuBar()->addMenu(tr("设置(&S)"));
    QAction* actionMusic = new QAction(QIcon(tr(":/icon/music.png")), tr("背景音乐"), this);
    actionMusic->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    menuSetting->addAction(actionMusic);

    //
    connect(actionCreatePro, &QAction::triggered, this, &MainWindow::SlotCreatePro);

    proTree_ = new ProTree();
    ui->proLayout->addWidget(proTree_);
    picShow_ = new PicShow;
    ui->picLayout->addWidget(picShow_);
    // 更新显示图片，在双击图片树中的 item 或者点击向前向后箭头最终会调用到此槽函数来更新显示图片
    connect(dynamic_cast<ProTree*>(proTree_)->GetProTreeWidget(), &ProTreeWidget::SigUpdateSelected,
            dynamic_cast<PicShow*>(picShow_), &PicShow::SlotUpdateSelected);
    // 向前箭头
    connect(dynamic_cast<PicShow*>(picShow_), &PicShow::SigPreviousPic,
            dynamic_cast<ProTree*>(proTree_)->GetProTreeWidget(), &ProTreeWidget::SlotPreviousPic);
    // 向后箭头
    connect(dynamic_cast<PicShow*>(picShow_), &PicShow::SigNextPic,
            dynamic_cast<ProTree*>(proTree_)->GetProTreeWidget(), &ProTreeWidget::SlotNextPic);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    PicShow *picShow = dynamic_cast<PicShow*>(picShow_);
    picShow->Reload();
    QMainWindow::resizeEvent(event);
}

void MainWindow::SlotCreatePro(bool checked)
{
    qDebug() << "SlotCreatePro called!";
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    auto* page = wizard.page(0);
    page->setTitle(tr("设置项目配置"));
    // todo 连接信号
    connect(&wizard, &Wizard::SigProSettings, dynamic_cast<ProTree*>(proTree_), &ProTree::AddProTree);

    wizard.show();
    wizard.exec();

    // 断开 wizard 所有信号的接收槽函数
    disconnect(&wizard);
}
