#ifndef RECEIVE_H
#define RECEIVE_H
#include "lib/pop3.h"
#include "email.h"

class Receive
{
public:
    Receive();
    void receiveHeaders();
    void receiveBody(const QString &uid);
//    void receiveEmailsHeaders(const QString uid);
//    void receiveEmailsHeaders(int index);
private:
    void parseHeader(const QStringList &list, Email &mail) const;
    void saveHeader(const Email &mail);
};

#endif // RECEIVE_H
