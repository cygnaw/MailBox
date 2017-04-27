#ifndef EMAIL_H
#define EMAIL_H

#include <QString>
#include <QStringList>

struct Email
{
    QString uid;
    QString subject;
    QString from;
    QString to;
    QString cc;
    QString date;
    QString body;
};


#endif // EMAIL_H
