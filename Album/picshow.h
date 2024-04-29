#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>

namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();
    void Reload();

protected:
    // void resizeEvent(QResizeEvent *event) override;

private:
    Ui::PicShow *ui;
    QString currentPath_;
    QPixmap curPixMap_;

public slots:
    void SlotUpdateSelected(const QString &path);
    void SlotBtnHide();
    void SlotBtnShow();

signals:
    void SigPreviousPic();
    void SigNextPic();
};

#endif // PICSHOW_H
