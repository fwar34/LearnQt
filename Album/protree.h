#ifndef PROTREE_H
#define PROTREE_H

#include "protreewidget.h"
#include <QDialog>

namespace Ui {
class ProTree;
}

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();
    ProTreeWidget* GetProTreeWidget();

public slots:
    void AddProTree(const QString& name, const QString &path);

private:
    Ui::ProTree *ui;
};

#endif // PROTREE_H
