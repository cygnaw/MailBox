#ifndef CREATACCOUNT_H
#define CREATACCOUNT_H

#include <QDialog>

namespace Ui {
class CreatAccount;
}

class CreatAccount : public QDialog
{
    Q_OBJECT

public:
    explicit CreatAccount(QWidget *parent = 0);
    ~CreatAccount();

signals:
    void show_tree_info();

private slots:
    void on_createButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CreatAccount *ui;
};

#endif // CREATACCOUNT_H
