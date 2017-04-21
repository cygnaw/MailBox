#include "sendwindow.h"
#include "ui_sendwindow.h"
#include "lib/smtp.h"
#include <QMessageBox>

SendWindow::SendWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SendWindow)
{
    ui->setupUi(this);
}

SendWindow::~SendWindow()
{
    delete ui;
}

void SendWindow::on_sendbutton_clicked()
{
    Smtp smtp;
    if(!smtp.connect("smtp.163.com",25))
    {
        QMessageBox warning;
        warning.setText(smtp.responseText);
        warning.exec();
        return;
    }
    QString name = "taylover";
    if(!smtp.ehlo(name))
    {
        QMessageBox warning;
        warning.setText(smtp.responseText);
        warning.exec();
        return;
    }
    QString user = "taylover_pei@163.com";
    QString passwd = "angdajia1996jwd";
    if(!smtp.login(user,passwd))
    {
        QMessageBox warning;
        warning.setText("test : "+ smtp.responseText);
        warning.exec();
        return;
    }
    QString To = ui->recipient->text().trimmed();
    if(To.isEmpty())
    {
        QMessageBox warning;
        warning.setText("请设置收件人！");
        warning.exec();
        return;
    }
    QString Cc = ui->CC->text().trimmed();
    QString Bcc = ui->BCC->text().trimmed();
    QString subject = ui->subject->text();
    QString content = ui->content->toPlainText();
    if(!smtp.sendmail(user,To,Cc,Bcc,subject,content))
    {
        QMessageBox warning;
        warning.setText("123: "+smtp.responseText);
        warning.exec();
        return;
    }
    else
    {
        QMessageBox warning;
        warning.setText("邮件发送成功！");
        warning.exec();
        return;
    }
    smtp.quit();
}
