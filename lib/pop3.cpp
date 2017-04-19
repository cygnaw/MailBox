#include "pop3.h"

Pop3::Pop3(QString host, int port, int timeout):
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
void Pop3::putline(QString line) {
    if (debugging > 1)
        qDebug() << "*put*" << line;
    tcpSocket->write(line + CRLF);
}

// Private: send one command to the server (through putline())
void Pop3::putcmd(QString line) {
    if (debugging)
        qDebug() << "*cmd*" << line;
    putline(line);
}

// Private: return one line from the server, stripping CRLF.
// This is where algl the CPU time of this module is consumed.
pair<QString, int> Pop3::getline() {
    if (!tcpSocket->canReadLine())
        tcpSocket->waitForReadyRead();
    QString line = tcpSocket->readLine(MAXLINE + 1);

    if (debugging > 1)
        qDebug() << "*get*" << line;
    int octets = line.size();

    // server can send any combination of CR & LF
    // however, 'readline()' returns lines ending in LF
    // so only possibilities are ...LF, ...CRLF, CR...LF
    if (line.endsWith(CRLF)) {
        return pair<QString, int>(line.mid(0, octets-2), octets);
    }
    if (line.startsWith(CR))
        return pair<QString, int>(line.mid(1, octets-2), octets);
    return pair<QString, int>(line.mid(0, octets-1), octets);
}

// Internal: get a response from the server.
QString Pop3::getresp() {
    pair<QString, int> resp = getline();
    if (debugging > 1)
        qDebug() << "*resp*" << resp.first;
    return resp.first;
}

// Private: get a response plus following text from the server.
tuple<QString, QStringList, int> Pop3::getlongresp() {
    QString resp = getresp();
    QStringList list;
    int octets = 0;
    pair<QString, int> tmp = getline();
    while (tmp.first != ".") {
        if (tmp.first.startsWith("..")) {
            --tmp.second;
            tmp.first.remove(0, 1);
        }
        octets += tmp.second;
        list << tmp.first;
        tmp = getline();
    }
    return tuple<QString, list, int>(resp, list, octets);
}

// Private: send a command and get the response
QString Pop3::shortcmd(line) {
    putcmd(line);
    return getresp();
}

// Private: send a command and get the response plus following text
tuple<QString, QStringList, int> Pop3::longcmd(line) {
    putcmd(line);
    return getlongresp();
}

// These are public functions

QString Pop3::getwelcome() {
    return welcome;
}

QString Pop3::QUIT() {
    tcpSocket->write("QUIT\r\n");
    tcpSocket->waitForReadyRead();
    return tcpSocket->readLine();
}

pair<int, int> Pop3::STAT() {
    tcpSocket->write("STAT\r\n");
    tcpSocket->waitForReadyRead();
    QString message = tcpSocket->readLine();
    QStringList list = message.split(' ', QString::SkipEmptyParts);
    return pair(list[1].toInt(), list[2].toInt());
}

QString Pop3::LIST(int which) {
    tcpSocket->write(QString("LIST %1" CRLF).arg(which));
    tcpSocket->waitForReadyRead();
    return tcpSocket->readLine();
}

pair<QString, QStringList> Pop3::LIST() {
    tcpSocket->write("LIST" CRLF);
    tcpSocket->waitForReadyRead();
    QString stat = tcpSocket->readLine();
    tcpSocket->waitForReadyRead();
    return pair(stat, readMultipleLine());
}

QString Pop3::USER(QString username) {
    tcpSocket->write(QString("USER %1\r\n").arg(username));
    tcpSocket->waitForReadyRead();
    return tcpSocket->readLine();
}

QString Pop3::PASS(QString password) {
    tcpSocket->write(QString("PASS %1\r\n").arg(password));
    tcpSocket->waitForReadyRead();
    return tcpSocket->readLine();
}

QStringList Pop3::readMultipleLine() {
    QStringList result;
    QString line;
    forever {
        line = tcpSocket->readLine()
    }
}
