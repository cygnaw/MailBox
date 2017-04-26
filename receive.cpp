#include "receive.h"
#include "accountmanager.h"
#include "lib/pop3.h"
#include "lib/decodestrategy.h"
#include "lib/typestrategy.h"
#include <QSet>
#include <QRegularExpression>
#include <QScopedPointer>

Receive::Receive()
{

}

void Receive::receiveHeaders() {
    AccountManager &AM = AccountManager::getInstance();
    QString user = AM.getUser();
    QString pass = AM.getPass();
    QString server = AM.getPopServer();
    int port = AM.getPopPort();

    QSqlQuery query;
    QSet<QString> set;
    query.exec(QString("SELECT m_id FROM receive_mail WHERE user='%1';").arg(user));
    while (query.next()) {
        set << query.value(QLatin1String("m_id")).toString();
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
            parseHeader(list, mail);
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
    while (!list.isEmpty()) {
        if (list.constFirst().isEmpty()) {
            list.removeAt(0);
            break;
        }
        list.removeAt(0);
    }

    pop.quit(resp);
}

void Receive::parseHeader(const QStringList &list, Email &mail) const {

}

void Receive::saveHeader(const Email &mail) {
    QSqlQuery query;
    QString user = AccountManager::getInstance().getUser();
    QString s = QString("INSERT INTO receive_mail ( "
                        "uid, user, from, to, cc, subject, date) "
                        "values ('%1', '%2', '%3', '%4', '%5', '%6', '%7');").arg(
                            mail.uid, user, mail.from, mail.to.join(','),
                            mail.cc.join(','), mail.subject, mail.date);
    query.exec(s);

}



void Parser::parseHeader(QStringList::iterator &it,
                    const QStringList::iterator &end,
                    Email &mail) const {
    QRegularExpression re("<(.+)>", QRegularExpression::InvertedGreedinessOption);
    QRegularExpressionMatch match;
    QRegularExpressionMatchIterator i;
    for (; it != end; ++it) {
        if (it->startsWith("from:", Qt::CaseInsensitive)) {
            match = re.match(*it);
            mail.from = match.captured(1);
        } else if (it->startsWith("to:", Qt::CaseInsensitive)) {
            i = re.globalMatch(*it);
            while (i.hasNext()) {
                match = i.next();
                mail.to << match.captured(1);
            }
        } else if (it->startsWith("cc:", Qt::CaseInsensitive)){
            i = re.globalMatch(*it);
            while (i.hasNext()) {
                match = i.next();
                mail.cc << match.captured(1);
            }
        } else if (it->startsWith("subject:", Qt::CaseInsensitive)) {
            mail.subject = it->mid(9);
        } else if (it->startsWith("date:", Qt::CaseInsensitive)) {
            mail.date = it->mid(6);
        } else if (it->isEmpty()) {
            break;
        }
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
