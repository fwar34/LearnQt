#include "piclabel.h"
#include <QEvent>

bool PicLabel::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        emit SigLabelEnter();
        break;
    case QEvent::Leave:
        emit SigLabelLeave();
        break;
    default:
        break;
    }
    return QLabel::event(event);
}
