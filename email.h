#ifndef MAIL_H
#define MAIL_H

#include <QString>
#include <QStringList>

struct Email
{
    QString uid;
    QString subject;
    QString from;
    QStringList to;
    QStringList cc;
    QString date;
    QString body;
};


#endif // MAIL_H
