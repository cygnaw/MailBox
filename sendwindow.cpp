#include "sendwindow.h"
#include "ui_sendwindow.h"
#include "lib/smtp.h"
#include "accountmanager.h"
#include <QMessageBox>
#include <QtSql>
#include <QDateTime>
#include <QProgressDialog>
#include "showmessage.h"

SendWindow::SendWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SendWindow)
{
    ui->setupUi(this);
    show_comboBox();
    QString User = "";
    SendWindow::get_currentAccount(User);
}

SendWindow::~SendWindow()
{
    delete ui;
}

void SendWindow::on_sendbutton_clicked()
{
    showmessage process_bar;
    QProgressDialog* process = new QProgressDialog(this);
    process->setWindowTitle("Please wait...");
    process->setRange(0,100);
    process->setFixedWidth(300);
    process->setFixedHeight(100);
    process->show();
    Smtp smtp;
    process_bar.show_processbar(0,10,process);
    if(!smtp.connect(smtp_server,smtp_port))
    {
        QMessageBox warning;
        warning.setText(smtp.responseText);
        warning.exec();
        return;
    }
    process_bar.show_processbar(10,20,process);
    QString name = "taylover";
    if(!smtp.ehlo(name))
    {
        QMessageBox warning;
        warning.setText(smtp.responseText);
        warning.exec();
        return;
    }
    process_bar.show_processbar(20,30,process);
    if(!smtp.login(username,password))
    {
        QMessageBox warning;
        warning.setText("test : "+ smtp.responseText);
        warning.exec();
        return;
    }
    process_bar.show_processbar(30,40,process);
    QString To = ui->recipient->text().trimmed();
    if(To.isEmpty())
    {
        QMessageBox warning;
        warning.setText("Please set the recipient!");
        warning.exec();
        return;
    }
    QString Cc = ui->CC->text().trimmed();
    QString Bcc = ui->BCC->text().trimmed();
    QString subject = ui->subject->text();
    QString content = ui->content->toPlainText();
    if(is_reply_or_forward == 1 || is_reply_or_forward == 2)
    {
        content = content + reply_forward_message;
    }
    process_bar.show_processbar(40,50,process);
    if(!smtp.sendmail(username,To,Cc,Bcc,subject,content))
    {
        QMessageBox warning;
        warning.setText(smtp.responseText);
        warning.exec();
        return;
    }
    else
    {
        QDateTime* time = new QDateTime;
        QString localtime = time->currentDateTime().toString("MM-dd hh:mm");
        QSqlQuery q;
        q.exec(QString("INSERT INTO send_mail (user, sended, sender, receiver, cc, bcc, date, subject, body)"
                        "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9');"
                        ).arg(username).arg(1).arg(username, To, Cc, Bcc, localtime, subject, content));
        process_bar.show_processbar(50,100,process);
        QMessageBox warning;
        warning.setText("Message sent successfully!");
        warning.exec();
        smtp.quit();
    }
}

void SendWindow::show_comboBox()
{
    QSqlQuery q;
    q.exec("SELECT username FROM account");
    while(q.next())
        ui->comboBox->addItem(q.value(0).toString());
}

void SendWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->from_mail->setText(arg1);
    SendWindow::get_currentAccount(arg1);
//    AccountManager &accountManager = AccountManager::getInstance();
//    accountManager.change(arg1);
//    username = accountManager.getUser();
//    password = accountManager.getPass();
//    smtp_server = accountManager.getSmtpServer();
//    smtp_port = accountManager.getSmtpPort();
//    pop_server = accountManager.getPopServer();
//    pop_port = accountManager.getPopPort();
}

void SendWindow::on_savebutton_clicked()
{
    QString To = ui->recipient->text().trimmed();
    QString Cc = ui->CC->text().trimmed();
    QString Bcc = ui->BCC->text().trimmed();
    QString subject = ui->subject->text();
    QString content = ui->content->toPlainText();
    QDateTime* time = new QDateTime;
    QString localtime = time->currentDateTime().toString("MM-dd hh:mm");
    QSqlQuery q;
    q.exec(QString("INSERT INTO send_mail (user, sended, sender, receiver, cc, bcc, date, subject, body)"
                   "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9');"
                   ).arg(username).arg(0).arg(username, To, Cc, Bcc, localtime, subject, content));
    QMessageBox warning;
    warning.setText("Save successfully!");
    warning.exec();
}

void SendWindow::Replymail(QString& unique_id, int flag)
{
    QSqlQuery q;
    q.exec(QString("SELECT user, sender, date, receiver, subject, body FROM receive_mail WHERE uid = '%1'").arg(unique_id));
    q.next();
    if(flag == 1)//reply command
    {
        ui->recipient->setText(q.value("sender").toString());
        ui->subject->setText("Re: " + q.value("subject").toString());
    }
    else if(flag == 2)
    {
        ui->subject->setText("Fw: " + q.value("subject").toString());
    }
    ui->from_mail->setText(q.value("receiver").toString());
    ui->comboBox->setCurrentText(q.value("receiver").toString());
    SendWindow::get_currentAccount(q.value("user").toString());
    is_reply_or_forward = flag;
    reply_forward_message = QString("\n\n\n\n---------------------------------------") +
                    QString("\nFrom:  ") + q.value("sender").toString() +
                    QString("\nDate:  ") + q.value("date").toString() +
                    QString("\nTo:  ") + q.value("receiver").toString() +
                    QString("\nSubject:  ") + q.value("subject").toString() +
                    QString("\n\n\n") + q.value("body").toString() +
                    QString("\n\n---------------------------------------");
}

void SendWindow::Editmail(QString& unique_id)
{
    QSqlQuery q;
    q.exec(QString("SELECT user, sender, receiver, cc, bcc, subject, body FROM send_mail WHERE uid = '%1'").arg(unique_id));
    q.next();
    qDebug() <<q.lastError();
    SendWindow::get_currentAccount(q.value("user").toString());
    ui->from_mail->setText(q.value("sender").toString());
    ui->comboBox->setCurrentText(q.value("sender").toString());
    ui->recipient->setText(q.value("receiver").toString());
    ui->CC->setText(q.value("cc").toString());
    ui->BCC->setText(q.value("bcc").toString());
    ui->subject->setText(q.value("subject").toString());
    ui->content->setText(q.value("body").toString());
}

void SendWindow::get_currentAccount(const QString& User)
{
    AccountManager &accountManager = AccountManager::getInstance();
    if(User != "")
        accountManager.change(User);
    username = accountManager.getUser();
    password = accountManager.getPass();
    smtp_server = accountManager.getSmtpServer();
    smtp_port = accountManager.getSmtpPort();
    pop_server = accountManager.getPopServer();
    pop_port = accountManager.getPopPort();
}
