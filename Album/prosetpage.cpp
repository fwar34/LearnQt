#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QLineEdit>
#include <QDir>
#include <QFileDialog>

ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proPath", ui->lineEditPath);
    registerField("proName*", ui->lineEditName);
    connect(ui->lineEditName, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    connect(ui->lineEditPath, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    QString curDir = QDir::currentPath();
    ui->lineEditPath->setText(curDir);
    ui->lineEditPath->setCursorPosition(ui->lineEditPath->text().size());
    ui->lineEditPath->setClearButtonEnabled(true);
    ui->lineEditName->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::GetNameAndPath(QString &name, QString &path)
{
    name = ui->lineEditName->text();
    path = ui->lineEditPath->text();
}

void ProSetPage::Tips(const QString& tips) const
{
    ui->tips->setText(tips);
}

bool ProSetPage::isComplete() const
{
    QDir dir(ui->lineEditPath->text());
    if (!dir.exists()) {
        Tips(QStringLiteral("Path[%1] is not exist!").arg(ui->lineEditPath->text()));
        return false;
    }

    QDir destDir = dir.absoluteFilePath(ui->lineEditName->text());
    if (!ui->lineEditName->text().isEmpty() && destDir.exists()) {
        Tips(QStringLiteral("Directory[%1] is exist!!!").arg(destDir.dirName()));
        return false;
    }

    return QWizardPage::isComplete();
}

void ProSetPage::on_pushButton_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setDirectory(QDir::current());
    if (fileDialog.exec()) {
        QStringList fileList = fileDialog.selectedFiles();
        if (fileList.empty()) {
            Tips(QStringLiteral("Please select files!"));
            return;
        }

        ui->lineEditPath->setText(fileList[0]);
    }
}

