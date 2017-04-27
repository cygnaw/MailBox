#ifndef ACCOUNTSETTING_H
#define ACCOUNTSETTING_H

#include <QMainWindow>

namespace Ui {
class accountsetting;
}

class accountsetting : public QMainWindow
{
    Q_OBJECT

public:
    explicit accountsetting(QWidget *parent = 0);
    ~accountsetting();

signals:
    void show_tree_info();

private slots:
    void on_createButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::accountsetting *ui;
};

#endif // ACCOUNTSETTING_H
