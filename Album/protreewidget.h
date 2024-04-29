#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include "prodbquery.h"
#include "CommonDefine.h"
#include <QTreeWidget>
#include <QSet>
#include <QAction>
#include <QSharedPointer>

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    ~ProTreeWidget();
    void AddProTree(const QString& name, const QString& path);
    ProDbQuery* GetProDbQuery()
    {
        return dbQuery_;
    }

public slots:
    void SlotItemPressed(QTreeWidgetItem *item, int column);
    void SlotImport(bool checked = false);
    void SlotItemDoubleClicked(QTreeWidgetItem *item, int column);
    void SlotPreviousPic();
    void SlotNextPic();

private:
    QSet<QString> projects_;
    QAction *importAction_;
    QAction *closeAction_;
    QAction *activeAction_;
    QAction *slideShowAction_;
    QThread *importThread_;
    ProLoadThread *loadThread_;
    ProDbQuery *dbQuery_;

    QTreeWidgetItem *rightClickItem_;
    QTreeWidgetItem *selectedItem_;
    QTreeWidgetItem *activeItem_;

signals:
    void SigUpdateSelected(const QString &path);
};

#endif // PROTREEWIDGET_H
