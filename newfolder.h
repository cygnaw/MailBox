#ifndef NEWFOLDER_H
#define NEWFOLDER_H

#include <QDialog>

namespace Ui {
class NewFolder;
}

class NewFolder : public QDialog
{
    Q_OBJECT

public:
    explicit NewFolder(QWidget *parent = 0);
    ~NewFolder();

private slots:
    void on_createButton_clicked();

    void on_cancelButton_clicked();

    void save_account(const QString&);

private:
    Ui::NewFolder *ui;
    QString saved_account;
};

#endif // NEWFOLDER_H
