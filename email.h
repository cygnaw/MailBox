#ifndef EMAIL_H
#define EMAIL_H

#include <QString>
#include <QStringList>
#include <QDateTime>

struct Email
{
    QString uid;
    QString subject;
    QString from;
    QString to;
    QString cc;
    QString date;
    QString body;
    Email() {
        date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }
};


#endif // EMAIL_H
