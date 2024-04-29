#include "picshow.h"
#include "ui_picshow.h"
#include <QFile>

PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow)
{
    ui->setupUi(this);
    ui->previousBtn->SetIcons(":/icon/previous.png", ":/icon/previous_press.png", ":/icon/previous_hover.png");
    ui->nextBtn->SetIcons(":/icon/next.png", ":/icon/next_press.png", ":/icon/next_hover.png");
    connect(ui->previousBtn, &PicButton::clicked, this, &PicShow::SigPreviousPic);
    connect(ui->nextBtn, &PicButton::clicked, this, &PicShow::SigNextPic);
    connect(ui->picLabel, &PicLabel::SigLabelEnter, this, &PicShow::SlotBtnShow);
    connect(ui->picLabel, &PicLabel::SigLabelLeave, this, &PicShow::SlotBtnHide);
}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::Reload()
{
    if (!currentPath_.isEmpty()) {
        const int width = ui->gridLayout->geometry().width();
        const int height = ui->gridLayout->geometry().height();
        curPixMap_ = curPixMap_.scaled(width, height, Qt::KeepAspectRatio);
        ui->picLabel->setPixmap(curPixMap_);
    }
}

// void PicShow::resizeEvent(QResizeEvent *event)
// {
//     const int width = ui->gridLayout->geometry().width();
//     const int height = ui->gridLayout->geometry().height();
//     curPixMap_ = curPixMap_.scaled(width, height, Qt::KeepAspectRatio);
//     ui->picLabel->setPixmap(curPixMap_);
//     QDialog::resizeEvent(event);
// }

void PicShow::SlotUpdateSelected(const QString &path)
{
    QFile file(path);
    if (file.exists()) {
        currentPath_ = path;
        const int width = ui->gridLayout->geometry().width();
        const int height = ui->gridLayout->geometry().height();
        QPixmap pixMap(path);
        curPixMap_ = pixMap.scaled(width, height, Qt::KeepAspectRatio);
        ui->picLabel->setPixmap(curPixMap_);
    }
}

void PicShow::SlotBtnHide()
{
    ui->previousBtn->OpacityAnimationHide();
    ui->nextBtn->OpacityAnimationHide();
}

void PicShow::SlotBtnShow()
{
    ui->previousBtn->OpacityAnimationShow();
    ui->nextBtn->OpacityAnimationShow();
}
