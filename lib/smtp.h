#ifndef SMTP_H
#define SMTP_H
#include <QObject>
#include <QtNetwork>
#include <QTcpSocket>

class Smtp
{
public:
    QTcpSocket* socket;
    QString responseText;
    int responseCode;

    Smtp();
    ~Smtp();
    void docmd(const QString &message);
    bool connect(const QString &host, int port);
    bool ehlo(const QString &name);
    bool login(const QString &user,const QString &passwd);
    bool sendmail(const QString from_address,QString to,QString cc,QString bcc,const QString subject,const QString content, QFile *file,int number);
    bool quit();
private:
    void waitresponse();
};

#endif // SMTP_H
