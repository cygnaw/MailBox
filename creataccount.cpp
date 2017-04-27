#include "creataccount.h"
#include "ui_creataccount.h"
#include "accountmanager.h"
#include "mainwindow.h"
#include <QMessageBox>

CreatAccount::CreatAccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatAccount)
{
    ui->setupUi(this);
    setWindowTitle(QLatin1String("Account Settings"));
}

CreatAccount::~CreatAccount()
{
    delete ui;
}

void CreatAccount::on_createButton_clicked()
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
        accept();
        this->close();
    }
    else
    {
        QMessageBox message;
        message.setText(QLatin1String("Create Failed!"));
        message.exec();
    }
}

void CreatAccount::on_cancelButton_clicked()
{
    this->close();
}
