#ifndef POP3_H
#define POP3_H

#include <QTcpSocket>

class Pop3
{
public:
    Pop3(QString host, int port, int timeout=30);
    ~Pop3();

    void QUIT();
    STAT();
    LIST();
    RETR();
    DELE();
    bool NOOP();
    RSET();
    TOP();
    UIDL();
    USER();
    PASS();
    APOP();
private:
    QTcpSocket *tcpSocket;

};

#endif // POP3_H
