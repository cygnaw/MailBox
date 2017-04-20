#include "pop3.h"

#define CR "\r"
#define LF "\n"
#define CRLF CR LF

Pop3::Pop3(QString const &host, int port):
    tcpSocket(new QTcpSocket),
    debugging(0)
{
    tcpSocket->connectToHost(host, port);
    if (tcpSocket->waitForConnected()) {
        tcpSocket->waitForReadyRead();
        welcome = tcpSocket->readAll();
    }
}

Pop3::~Pop3() {
    delete tcpSocket;
}

// Private
void Pop3::putline(const QString &line) {
    if (debugging > 1)
        qDebug() << "*put*" << line;
    tcpSocket->write(line.toUtf8());
    tcpSocket->write(CRLF);
}

// Private: send one command to the server (through putline())
void Pop3::putcmd(const QString &line) {
    if (debugging)
        qDebug() << "*cmd*" << line;
    putline(line);
}

// Private: return one line from the server, stripping CRLF.
// This is where algl the CPU time of this module is consumed.
void Pop3::getline(QString &line, int &octets) {
    if (!tcpSocket->canReadLine())
        tcpSocket->waitForReadyRead();
    line = tcpSocket->readLine(MAXLINE + 1);

    if (debugging > 1)
        qDebug() << "*get*" << line;
    octets = line.size();

    // server can send any combination of CR & LF
    // however, 'readline()' returns lines ending in LF
    // so only possibilities are ...LF, ...CRLF, CR...LF
    if (line.endsWith(CRLF))
        line.chop(2);
    else if (line.startsWith(CR))
        line = line.mid(1, octets-2);
    else
        line.chop(1);
}

// Internal: get a response from the server.
void Pop3::getresp(QString& resp) {
    int octets;
    getline(resp, octets);
    if (debugging > 1)
        qDebug() << "*resp*" << resp;
}

// Private: get a response plus following text from the server.
void Pop3::getlongresp(QString &resp, QStringList &list, int &octets) {
    getresp(resp);
    list.clear();
    octets = 0;

    QString line;
    int o;
    getline(line, o);
    while (line != ".") {
        if (line.startsWith("..")) {
            --o;
            line.remove(0, 1);
        }
        octets += o;
        list << line;
        getline(line, o);
    }
}

// Private: send a command and get the response
bool Pop3::shortcmd(const QString &line, QString &resp) {
    putcmd(line);
    getresp(resp);
    return resp.startsWith(QLatin1String("+OK"), Qt::CaseInsensitive);
}

// Private: send a command and get the response plus following text
bool Pop3::longcmd(const QString &line, QString &resp, QStringList &list, int &octets) {
    putcmd(line);
    getlongresp(resp, list, octets);
    return resp.startsWith(QLatin1String("+OK"), Qt::CaseInsensitive);
}

// These are public functions

QString Pop3::getwelcome() {
    return welcome;
}

void Pop3::set_debuglevel(int level) {
    debugging = level;
}

bool Pop3::user(const QString &username, QString &resp) {
    return shortcmd(QString("USER %1").arg(username), resp);
}

bool Pop3::pass(const QString &password, QString &resp) {
    return shortcmd(QString("PASS %1").arg(password), resp);
}

bool Pop3::stat(int &num, int &size) {
    QString retval;
    bool ok = shortcmd(QLatin1String("STAT"), retval);
    QStringList rets = retval.split(' ', QString::SkipEmptyParts);
    if (debugging)
        qDebug() << "*stat*" << rets;
    num = rets[1].toInt();
    size = rets[2].toInt();
    return ok;
}

bool Pop3::list(QString &resp, QStringList &list, int &octets) {
    return longcmd(QLatin1String("LIST"), resp, list, octets);
}

bool Pop3::list(int which, QString &resp) {
    return shortcmd(QString("LIST %1").arg(which), resp);
}

bool Pop3::retr(int which, QString &resp, QStringList &list, int &octets) {
    return longcmd(QString("RETR %1").arg(which), resp, list, octets);
}

bool Pop3::dele(int which, QString &resp) {
    return shortcmd(QString("DELE %1").arg(which), resp);
}

bool Pop3::noop(QString &resp) {
    return shortcmd(QLatin1String("NOOP"), resp);
}

bool Pop3::rset(QString &resp) {
    return shortcmd(QLatin1String("RSET"), resp);
}

bool Pop3::quit(QString &resp) {
    bool ok = shortcmd(QLatin1String("QUIT"), resp);
    close();
    return ok;
}

void Pop3::close() {
    tcpSocket->close();
}

// optinal commands
bool Pop3::apop(const QString &user, const QString &password, QString &resp) {
    // not implement
    return false;
}

bool Pop3::top(int which, int howmuch, QString &resp, QStringList &list, int &octets) {
    return longcmd(QString("TOP %1 %2").arg(which).arg(howmuch), resp, list, octets);
}

bool Pop3::uidl(QString &resp, QStringList &list, int &octets) {
    return longcmd(QLatin1String("UIDL"), resp, list, octets);
}

bool Pop3::uidl(int which, QString &resp) {
    return shortcmd(QString("UIDL %1").arg(which), resp);
}
