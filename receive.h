#ifndef RECEIVE_H
#define RECEIVE_H
#include "lib/pop3.h"
#include "email.h"

class Parser {
public:
    void parseHeader(QStringList::iterator &it,
                     const QStringList::iterator &end,
                     Email &mail);
    void parseBody(QStringList::iterator &it,
                   const QStringList::iterator &end,
                   QString &body);
    QString parseFrom(const QString &str);
    QString parseTo(const QString &str);
    QString parseCc(const QString &str);
    QString parseSubject(const QString &str);
    QString parseDate(const QString &str);
private:
};

class Receive
{
public:
    void receiveHeaders();
    void receiveBody(const QString &uid);
private:
    Parser parser;
    void saveHeader(const Email &mail);
    void saveBody(const QString &uid, const QString &body);
};


#endif // RECEIVE_H
