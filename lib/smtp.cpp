#include "smtp.h"
#include "qdebug.h"
#include <QMessageBox>
#include <QByteArray>
Smtp::Smtp()
{
    socket = new QTcpSocket;
}

Smtp::~Smtp()
{
    delete socket;
}

void Smtp::waitresponse()
{
    if(!socket->waitForReadyRead())
    {
        QMessageBox warning;
        warning.setText("ResponseTimeoutError");
        warning.exec();
    }
    while(socket->canReadLine() && !socket->atEnd())
    {
        responseText = socket->readLine();
    }
    if(!responseText.isEmpty())
    {
        responseCode = responseText.left(3).toInt();
    }
    return;
}

void Smtp::docmd(const QString &message)
{
    socket->write(message.toUtf8() + "\r\n");
}

bool Smtp::connect(const QString &host, int port)
{
    socket->connectToHost(host,port);
    if(!socket->waitForConnected(5000))
    {
        responseText = "ConnectionTimeoutError";
        return false;
    }
    waitresponse();
    if(responseCode != 220)
        return false;
    return true;
}

bool Smtp::ehlo(const QString &name)
{
    docmd("EHLO " + name);
    waitresponse();
    if(responseCode != 250)
        return false;
    return true;
}

bool Smtp::login(const QString &user, const QString &passwd)
{
    QString name = "taylover";
    docmd("EHLO " + name);
    waitresponse();
    if(responseCode != 250)
        return false;
    docmd("AUTH LOGIN");
    waitresponse();
    if(responseCode != 334)
        return false;
    docmd(QByteArray().append(user).toBase64());
    waitresponse();
    if(responseCode != 334)
        return false;
    docmd(QByteArray().append(passwd).toBase64());
    waitresponse();
    if(responseCode != 235)
        return false;
    return true;
}

bool Smtp::sendmail(const QString from_address, QString to, QString cc, QString bcc, const QString subject, const QString content, QFile *file,int number)
{
    docmd("MAIL FROM: <" + from_address + ">");
    waitresponse();
    if(responseCode != 250)
        return false;
    int i;
    QStringList To = to.split(";");
    for(i = 0;i < To.size();i++)
    {
        docmd("RCPT TO: <" + To.at(i) + ">");
        waitresponse();
        if(responseCode != 250)
            return false;
    }
    if(!cc.isEmpty())
    {
        QStringList Cc = cc.split(";");
        for(i = 0;i<Cc.size();i++)
        {
            docmd("RCPT TO: <" + Cc.at(i) + ">");
            waitresponse();
            if(responseCode != 250)
                return false;
        }
    }
    if(!bcc.isEmpty())
    {
        QStringList Bcc = bcc.split(";");
        for(i = 0;i< Bcc.size();i++)
        {
            docmd("RCPT TO: <" + Bcc.at(i) + ">");
            waitresponse();
            if(responseCode != 250)
                return false;
        }
    }
    docmd("DATA");
    waitresponse();
    if(responseCode != 354)
        return false;
    QString message;
    //message = "From: <" + sender + ">\r\n";
    //message = message + "To: <" + address + ">\r\n";
    //message = message + "Subject: " + subject + "\r\n\r\n";

    /* ----sender / from---- */
    message = "From: <" + from_address + ">\r\n";

    /* ---- recipients / To ----*/
    message = message + "To:";
    for(i = 0;i < To.size();i++)
    {
        if(i != 0)
            message += ",";
        message += " <" + To.at(i) + ">";
    }
    message += "\r\n";

    /* ---- recipients / Cc ----*/
    if(!cc.isEmpty())
    {
        QStringList Cc = cc.split(";");
        message = message + "Cc:";
        for(i = 0;i < Cc.size();i++)
        {
            if(i != 0)
                message += ",";
            message += " <" + Cc.at(i) + ">";
        }
        message += "\r\n";
    }

    if(number)//have attachment
    {
          message = message + "Subject: " + subject + "\r\n";
          message = message + "MIME-Version: 1.0" + "\r\n";
          message = message + "Content-Type: multipart/mixed; boundary=nextmessage_jia_123" + "\r\n";
          message = message + "This is a multi-part message in MIME format." + "\r\n\r\n";
          message = message + "--nextmessage_jia_123" + "\r\n";
          message = message + "Content-Type: text/html;" + " charset=utf-8\r\n\r\n";
          message = message + content;
          message = message + "\r\n";
          message = message + "--nextmessage_jia_123" + "\r\n";
          QString name = QFileInfo(*file).fileName();
          message = message + "Content-Type: application/octet-stream; " + "name=\"" + name + "\"\r\n";
          message = message + "Content-Disposition: attachment" + "\r\n\r\n";
          file->open(QIODevice::ReadOnly);
          message = message + file->readAll();
          file->close();
          message = message + "\r\n\r\n";
          message = message + "--nextmessage_jia_123--" + "\r\n";
    }
    else//no attachment
    {
        message = message + "Subject: " + subject + "\r\n";
        message = message + "MIME-Version: 1.0" + "\r\n";
        message = message + "Content-Type: text/html;" + " charset=utf-8\r\n\r\n";
        message = message + content;
    }
     docmd(message);
     docmd("\r\n.\r\n");
     waitresponse();
     if(responseCode != 250)
         return false;
     return true;
}

bool Smtp::quit()
{
    docmd("QUIT");
    return true;
}

