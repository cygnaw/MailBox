#include "accountsetting.h"
#include "ui_accountsetting.h"
#include "accountmanager.h"
#include "mainwindow.h"
#include <QMessageBox>

accountsetting::accountsetting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::accountsetting)
{
    ui->setupUi(this);
    setWindowTitle(QLatin1String("Account Settings"));
}

accountsetting::~accountsetting()
{
    delete ui;
}

void accountsetting::on_createButton_clicked()
{
    QString user,passwd,incoming,outgoing;
    int inport,outport;
    user = ui->Account->text();
    passwd = ui->Password->text();
    incoming = ui->Incoming->text();
    outgoing = ui->Outgoing->text();
    inport = ui->Inport->text().toInt();
    outport = ui->Outport->text().toInt();
    if(user.isEmpty() || passwd.isEmpty() || incoming.isEmpty() || outgoing.isEmpty())
    {
        QMessageBox warning;
        warning.setText("Please complete the information!");
        warning.exec();
        return;
    }
    AccountManager &accountManager = AccountManager::getInstance();
    QString user1 = accountManager.getUser();
    if(user == user1)
    {
        QMessageBox message;
        message.setText(QLatin1String("The user already exits!"));
        message.exec();
        return;
    }
    if(accountManager.add(user,passwd,incoming,inport,outgoing,outport))
    {
        emit show_tree_info();
        QMessageBox message;
        message.setText(QLatin1String("Create Successfully!"));
        message.exec();
        this->close();
    }
    else
    {
        QMessageBox message;
        message.setText(QLatin1String("Create Failed!"));
        message.exec();
    }
}

void accountsetting::on_cancelButton_clicked()
{
    this->close();
}
