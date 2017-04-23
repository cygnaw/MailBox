#include "receive.h"
#include "accountmanager.h"
#include "lib/pop3.h"
#include <QSet>
#include <QRegularExpression>

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
    foreach (const QString &line, list) {
        QStringList l = line.split(' ', QString::SkipEmptyParts);
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
    AccountManager AM = AccountManager::getInstance();
    Pop3 pop(AM.getPopServer(), AM.getPopPort());
    pop.user(AM.getUser());
    pop.pass(AM.getPass());

    QString resp;
    QStringList list;
    int octets;
    pop.uidl(resp, list, octets);
    foreach (resp, list) {
        QStringList l = resp.split(' ', QString::SkipEmptyParts);
        if (l[1] == uid)
    }

    pop.quit(resp);
}

void Receive::parseHeader(const QStringList &list, Email &mail) const {
    QRegularExpression re("<(.+)>", QRegularExpression::InvertedGreedinessOption);
    QRegularExpressionMatch match;
    QRegularExpressionMatchIterator i;
    foreach (const QString &line, list) {
        if (line.startsWith("from:", Qt::CaseInsensitive)) {
            match = re.match(line);
            mail.from = match.captured(1);
        } else if (line.startsWith("to:", Qt::CaseInsensitive)) {
            i = re.globalMatch(line);
            while (i.hasNext()) {
                match = i.next();
                mail.to << match.captured(1);
            }
        } else if (line.startsWith("cc:", Qt::CaseInsensitive)){
            i = re.globalMatch(line);
            while (i.hasNext()) {
                match = i.next();
                mail.cc << match.captured(1);
            }
        } else if (line.startsWith("subject:", Qt::CaseInsensitive)) {
            mail.subject = line.mid(9);
        } else if (line.startsWith("date:", Qt::CaseInsensitive)) {
            mail.date = line.mid(6);
        } else if (line.isEmpty()) {
            break;
        }
    }
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
