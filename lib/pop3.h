#ifndef POP3_H
#define POP3_H

#include <QTcpSocket>
#include <tuple>

#define CR "\r"
#define LF "\n"
#define CRLF CR LF

using std::pair;
using std::tuple;

class Pop3
{
    const int POP3_PORT = 110;
    const int MAXLINE = 2048;

    QTcpSocket *tcpSocket;
    QString welcome;
    int debugging;

    void putline(QString line);
    void putcmd(QString line);
    pair<QString, int> getline();
    QString getresp();
    tuple<QString, QStringList, int> getlongresp();
    QString shortcmd(QString line);
    tuple<QString, QStringList, int> longcmd(QString line);

public:
    Pop3(QString host, int port=POP3_PORT, int timeout=300);
    ~Pop3();

    QString getwelcome();
    QString QUIT();
    pair<int, int> STAT();
    QString LIST(int which);
    pair<QString, QStringList> LIST();
    QString RETR();
    QString DELE();
    QString NOOP();
    QString RSET();
    QString TOP();
    QString UIDL();
    QString USER(QString username);
    QString PASS(QString password);
    QString APOP(QString user, QString secret);
};

#endif // POP3_H
