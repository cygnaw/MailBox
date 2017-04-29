#include "newfolder.h"
#include "ui_newfolder.h"
#include <QtSql>

NewFolder::NewFolder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFolder)
{
    ui->setupUi(this);
}

NewFolder::~NewFolder()
{
    delete ui;
}

void NewFolder::on_createButton_clicked()
{
    QString folder_name = ui->foldername->text();
    QSqlQuery q;
    q.exec(QString("INSERT INTO dir (user, folder) VALUES ('%1', '%2');").arg(saved_account).arg(folder_name));
    this->accept();
}

void NewFolder::on_cancelButton_clicked()
{
    this->close();
}

void NewFolder::save_account(const QString & account)
{
    saved_account = account;
}
