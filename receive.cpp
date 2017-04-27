#include "receive.h"
#include "accountmanager.h"
#include "lib/pop3.h"
#include "lib/decodestrategy.h"
#include "lib/typestrategy.h"
#include <QSet>
#include <QRegularExpression>
#include <QScopedPointer>
#include <QLocale>


void Receive::receiveHeaders() {
    AccountManager &AM = AccountManager::getInstance();
    QString user = AM.getUser();
    QString pass = AM.getPass();
    QString server = AM.getPopServer();
    int port = AM.getPopPort();

    QSqlQuery query;
    QSet<QString> set;
    query.exec(QString("SELECT uid FROM receive_mail WHERE user='%1';").arg(user));
    while (query.next()) {
        set << query.value(QLatin1String("uid")).toString();
    }

    QString resp;
    QStringList list;
    int octets;

    Pop3 pop(server, port);
    pop.user(user, resp);
    pop.pass(pass, resp);

    pop.uidl(resp, list, octets);
    foreach (const QString &it, list) {
        QStringList l = it.split(' ', QString::SkipEmptyParts);
        if (!set.contains(l[1])) {
            Email mail;
            mail.uid = l[1];
            QString resp;
            QStringList list;
            int octets;
            pop.top(l[0].toInt(), 0, resp, list, octets);
            QStringList::iterator it = list.begin();
            parser.parseHeader(it, list.end(), mail);
            saveHeader(mail);
        }
    }

    pop.quit(resp);

}

void Receive::receiveBody(const QString &uid) {
    QString resp;
    QStringList list;
    int octets;
    AccountManager &AM = AccountManager::getInstance();
    Pop3 pop(AM.getPopServer(), AM.getPopPort());
    pop.user(AM.getUser(), resp);
    pop.pass(AM.getPass(), resp);

    QStringList l;
    pop.uidl(resp, list, octets);
    foreach (resp, list) {
        l = resp.split(' ', QString::SkipEmptyParts);
        if (l[1] == uid)
            break;
    }
    pop.retr(l[0].toInt(), resp, list, octets);
    QStringList::iterator it = list.begin();
    QString body;
    parser.parseBody(it, list.end(), body);
    saveBody(uid, body);
    pop.quit(resp);
}


void Receive::saveHeader(const Email &mail) {
    QSqlQuery query;
    QString user = AccountManager::getInstance().getUser();
    QString s = QString("INSERT INTO receive_mail ( "
                        "uid, user, sender, receiver, cc, subject, date) "
                        "values ('%1', '%2', '%3', '%4', '%5', '%6', '%7');").arg(
                            mail.uid, user, mail.from, mail.to,
                            mail.cc, mail.subject, mail.date);
    query.exec(s);
}

void Receive::saveBody(const QString &uid, const QString &body) {
    QSqlQuery query;
    QString s = QString("UPDATE receive_mail "
                        "SET downloaded = 1, body = '%1'"
                        "WHERE uid = '%2'").arg(body, uid);
    query.exec(s);
    qDebug() << query.lastError();
}

void Parser::parseHeader(QStringList::iterator &it,
                    const QStringList::iterator &end,
                    Email &mail) {
    for (; it != end; ++it) {
        if (it->startsWith("from:", Qt::CaseInsensitive))
            mail.from = parseFrom(*it);
        else if (it->startsWith("to:", Qt::CaseInsensitive))
            mail.to = parseTo(*it);
        else if (it->startsWith("cc:", Qt::CaseInsensitive))
            mail.cc = parseCc(*it);
        else if (it->startsWith("subject:", Qt::CaseInsensitive))
            mail.subject = parseSubject(*it);
        else if (it->startsWith("date:", Qt::CaseInsensitive))
            mail.date = parseDate(*it);
        else if (it->isEmpty())
            break;
    }
}

void Parser::parseBody(QStringList::iterator &it, const QStringList::iterator &end, QString &body) {
    DecodeStrategy* decoder = nullptr;
    TypeStrategy* type = nullptr;
    QString boundary;
    for (; !it->isEmpty(); ++it) {
        if (it->contains("content-type", Qt::CaseInsensitive)) {
            if (it->contains("text/html", Qt::CaseInsensitive))
                type = new TypeTextHtml;
            else if (it->contains("text/plain", Qt::CaseInsensitive))
                type = new TypeTextPlain;
            else if (it->contains("multipart/alternative", Qt::CaseInsensitive))
                type = new TypeMultipartAlternative;
            else if (it->contains("multipart/mixed", Qt::CaseInsensitive))
                type = new TypeMultipartMixed;
        }
        if (it->contains("boundary=", Qt::CaseInsensitive)) {
            QRegularExpression re("boundary=\"(.+)\"", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = re.match(*it);
            boundary = match.captured(1);
        }
        if (it->contains("content-transfer-encoding", Qt::CaseInsensitive)) {
            if (it->contains("7bit", Qt::CaseInsensitive))
                decoder = new Decode7Bit;
            else if (it->contains("8bit", Qt::CaseInsensitive))
                decoder = new Decode8Bit;
            else if (it->contains("binary", Qt::CaseInsensitive))
                decoder = new DecodeBinary;
            else if (it->contains("quoted-printable", Qt::CaseInsensitive))
                decoder = new DecodeQuotedPrintable;
            else if (it->contains("base64", Qt::CaseInsensitive))
                decoder = new DecodeBase64;
        }
    }
    if (type == nullptr)
        type = new TypeTextPlain;
    if (decoder == nullptr)
        decoder = new Decode8Bit;

    type->setDecoder(decoder);
    type->setBoundary(boundary);
    type->handle(it, end, body);
    delete decoder;
    delete type;
}

QString Parser::parseFrom(const QString &str) {
    QRegularExpression re("<(\\w+@\\w+\\.\\w+)>");
    QRegularExpressionMatch match = re.match(str);
    if (match.hasMatch())
        return match.captured(1);
    re = QRegularExpression("\\w+@\\w+\\.\\w+");
    match = re.match(str);
    if (match.hasMatch())
        return match.captured(0);
    return "";
}

QString Parser::parseTo(const QString &str) {
    QRegularExpression re("<(\\w+@\\w+\\.\\w+)>");
    QRegularExpressionMatchIterator i = re.globalMatch(str);
    QRegularExpressionMatch match;
    QString result;
    while (i.hasNext()) {
        result += i.next().captured(1);
        result += ",";
    }
    if (!result.isEmpty())
        return result;

    re = QRegularExpression("\\w+@\\w+\\.\\w+");
    i = re.globalMatch(str);
    while (i.hasNext()) {
        result += i.next().captured(0);
        result += ",";
    }
    return result;
}

QString Parser::parseCc(const QString &str) {
    QRegularExpression re("<(\\w+@\\w+\\.\\w+)>");
    QRegularExpressionMatchIterator i = re.globalMatch(str);
    QRegularExpressionMatch match;
    QString result;
    while (i.hasNext()) {
        result += i.next().captured(1);
        result += ",";
    }
    if (!result.isEmpty())
        return result;

    re = QRegularExpression("\\w+@\\w+\\.\\w+");
    i = re.globalMatch(str);
    while (i.hasNext()) {
        result += i.next().captured(0);
        result += ",";
    }
    return result;
}

QString Parser::parseSubject(const QString &str) {
    QString result;
    QRegularExpression re("=\\?(\\S+)\\?([BQ])\\?(\\S+)\\?=");
    QRegularExpressionMatch match = re.match(str);
    if (match.hasMatch()) {
        DecodeStrategy* decoder;
        if (match.captured(2) == "B")
            decoder = new DecodeBase64;
        else
            decoder = new DecodeQuotedPrintable;
        decoder->decode(match.captured(3), result);
        return result;
    }
    return str.mid(9);
}

QString Parser::parseDate(const QString &str) {
    const QString &s = str.mid(6, 25);
    QString format("ddd, dd MMM yyyy hh:mm:ss");
    QLocale locale(QLocale::English, QLocale::UnitedStates);
    QDateTime date = locale.toDateTime(s, format);
    if (!date.isValid()) date = QDateTime::currentDateTime();
    return date.toString("yyyy-MM-dd hh:mm:ss");
}
