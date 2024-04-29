#include "picbutton.h"
#include <QEvent>
#include <QAbstractAnimation>

void PicButton::OpacityAnimationShow()
{
    animation_->stop();
    animation_->setStartValue(0.0); // 透明
    animation_->setEndValue(1.0); // 不透明
    animation_->start();
}

void PicButton::OpacityAnimationHide()
{
    animation_->stop();
    animation_->setStartValue(1.0);
    animation_->setEndValue(0.0);
    animation_->start();
}

bool PicButton::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        SetHoverIcon();
        break;
    case QEvent::Leave:
        SetNormalIcon();
        break;
    case QEvent::MouseButtonPress:
        SetPressedIcon();
        break;
    case QEvent::MouseButtonRelease:
        SetHoverIcon();
        break;
    default:
        break;
    }

    return QPushButton::event(event);
}

void PicButton::SetNormalIcon()
{
    // static QPixmap icon(normalIcon_);
    setIcon(normalPixmap_);
}

void PicButton::SetHoverIcon()
{
    // static QPixmap icon(hoverIcon_);
    setIcon(hoverPixmap_);
}

void PicButton::SetPressedIcon()
{
    // static QPixmap icon(pressedIcon_);
    setIcon(pressedPixmap_);
}

void PicButton::AddAnimatorEffect()
{
    // 为 btn 添加一个透明度效果
    effect_ = new QGraphicsOpacityEffect(this);

    // 动画化透明度
    animation_ = new QPropertyAnimation(effect_, "opacity");
    animation_->setDuration(1000);
    setGraphicsEffect(effect_);
}
