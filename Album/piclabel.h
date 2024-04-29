#ifndef PICLABEL_H
#define PICLABEL_H

#include <QLabel>

class PicLabel : public QLabel
{
    Q_OBJECT
public:
    PicLabel(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags()) : QLabel(parent, f)
    {
    }

protected:
    bool event(QEvent *event) override;

signals:
    void SigLabelEnter();
    void SigLabelLeave();
};

#endif // PICLABEL_H
