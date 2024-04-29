#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>
#include <QGraphicsEffect>
#include <QPropertyAnimation>

class PicButton : public QPushButton
{
public:
    PicButton(QWidget *parent = nullptr) : QPushButton(parent)
    {
        AddAnimatorEffect();
    }

    void SetIcons(const QString &normal, const QString &pressed, const QString &hover)
    {
        normalPixmap_.load(normal);
        normalIcon_.addPixmap(normalPixmap_);
        pressedPixmap_.load(pressed);
        pressedIcon_.addPixmap(pressedPixmap_);
        hoverPixmap_.load(hover);
        hoverIcon_.addPixmap(hoverPixmap_);

        setIcon(normalPixmap_);
        resize(normalPixmap_.size());
        setIconSize(normalPixmap_.size());
    }

    void OpacityAnimationHide();
    void OpacityAnimationShow();

protected:
    bool event(QEvent *event) override;

private:
    void AddAnimatorEffect();
    void SetNormalIcon();
    void SetHoverIcon();
    void SetPressedIcon();
    QPixmap normalPixmap_;
    QIcon normalIcon_;
    QPixmap pressedPixmap_;
    QIcon pressedIcon_;
    QPixmap hoverPixmap_;
    QIcon hoverIcon_;
    QGraphicsOpacityEffect *effect_ = nullptr;
    QPropertyAnimation *animation_ = nullptr;
};

#endif // PICBUTTON_H
