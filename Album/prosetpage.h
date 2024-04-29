#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = nullptr);
    ~ProSetPage();
    void GetNameAndPath(QString& name, QString& path);

protected:
    bool isComplete() const override;

private slots:
    void on_pushButton_clicked();

private:
    void Tips(const QString& tips) const;
    Ui::ProSetPage *ui;
};

#endif // PROSETPAGE_H
