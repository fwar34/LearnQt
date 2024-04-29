#include "wizard.h"
#include "ui_wizard.h"

Wizard::Wizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}

void Wizard::done(int r)
{
    if (r == QDialog::Rejected) {
        return QWizard::done(r);
    }

    QString name, path;
    ui->wizardPage1->GetNameAndPath(name, path);
    emit SigProSettings(name, path);
    QWizard::done(r);
}
