#ifndef POP3_H
#define POP3_H

#include <QTcpSocket>

class Pop3
{
    const static int POP3_PORT = 110;
    const static int MAXLINE = 2048;

    QTcpSocket *tcpSocket;
    QString welcome;
    int debugging;

    void putline(const QString &line);
    void putcmd(const QString &line);
    void getline(QString &line, int &octets);
    void getresp(QString &resp);
    void getlongresp(QString &resp, QStringList &list, int &octets);
    bool shortcmd(const QString &line, QString &resp);
    bool longcmd(const QString &line, QString &resp, QStringList &list, int &octets);

public:
    Pop3(QString const &host, int port=POP3_PORT);
    ~Pop3();

    QString getwelcome();
    void set_debuglevel(int level);
    bool user(const QString &username, QString &resp);
    bool pass(const QString &password, QString &resp);
    bool stat(int &num, int &size);
    bool list(QString &resp, QStringList &list, int &octets);
    bool list(int which, QString &resp);
    bool retr(int which, QString &resp, QStringList &list, int &octets);
    bool dele(int which, QString &resp);
    bool noop(QString &resp);
    bool rset(QString &resp);
    bool quit(QString &resp);
    void close();
    // optinal commands:
    bool apop(const QString &user, const QString &password, QString &resp);
    bool top(int which, int howmuch, QString &resp, QStringList &list, int &octets);
    bool uidl(QString &resp, QStringList &list, int &octets);
    bool uidl(int which, QString &resp);
};

#endif // POP3_H
