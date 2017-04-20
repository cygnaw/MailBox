#include "accountmanager.h"
#include "lib/pop3.h"

AccountManager::AccountManager()
{
    QSqlQuery query;
    query.exec("SELECT username, password, pop_server, pop_port, smtp_server, smtp_port FROM account;");
    query.next();
    username = query.value(QLatin1String("username")).toString();
    password = query.value(QLatin1String("password")).toString();
    pop_server = query.value(QLatin1String("pop_server")).toString();
    pop_port = query.value(QLatin1String("pop_port")).toInt();
    smtp_server = query.value(QLatin1String("smtp_server")).toString();
    smtp_port = query.value(QLatin1String("smtp_port")).toInt();
}

void AccountManager::change(const QSqlRecord &record) {
    username = record.value("username").toString();
    password = record.value("password").toString();
    pop_server = record.value("popserver").toString();
    pop_port = record.value("pop_port").toInt();
    smtp_server = record.value("smtp_server").toString();
    smtp_port = record.value("smtp_port").toInt();
}

bool AccountManager::modify(const QString &username, const QString &password,
                            const QString &pop_server, int pop_port,
                            const QString &smtp_server, int smtp_port) {
    if (validatePop(username, password, pop_server, pop_port) &&
            validateSmtp(username, password, smtp_server, smtp_port)) {
        QSqlQuery query;
        query.exec(QString("UPDATE account SET password = '%1', pop_server = '%2', pop_port = %3,"
                   "smtp_server = '%4', smtp_port = %5 WHERE username = '%6'").arg(
                       password, pop_server).arg(pop_port).arg(smtp_server).arg(smtp_port).arg(username));
        if (username == this->username) {
            this->password = password;
            this->pop_server = pop_server;
            this->pop_port = pop_port;
            this->smtp_server = smtp_server;
            this->smtp_port = smtp_port;
        }
        return true;
    }

    return false;
}

void AccountManager::remove(QString username, bool withMail) {
    QSqlQuery query;
    if (withMail) {
        // TODO
    }
    query.exec(QString("DELETE FROM account WHERE username='%1';").arg(username));
    if (this->username == username) {
        query.exec("SELECT username, password, pop_server, pop_port, smtp_server, smtp_port FROM account;");
        query.next();
        username = query.value(QLatin1String("username")).toString();
        password = query.value(QLatin1String("password")).toString();
        pop_server = query.value(QLatin1String("pop_server")).toString();
        pop_port = query.value(QLatin1String("pop_port")).toInt();
        smtp_server = query.value(QLatin1String("smtp_server")).toString();
        smtp_port = query.value(QLatin1String("smtp_port")).toInt();
    }
}

bool AccountManager::validatePop(const QString &user, const QString &pass,
                                 const QString &server, int port) {
    Pop3 pop(server, port);
    QString resp;
    return pop.user(user, resp) && pop.pass(pass, resp);
}

bool AccountManager::validateSmtp(const QString &user, const QString &pass,
                                  const QString &server, int port) {
    return true;
}

bool AccountManager::add(const QString &username, const QString &password,
                         const QString &pop_server, int pop_port,
                         const QString &smtp_server, int smtp_port) {
    if (validatePop(username, password, pop_server, pop_port) &&
            validateSmtp(username, password, smtp_server, smtp_port)) {
        QSqlQuery query;
        query.exec(QString("INSERT INTO account (username, password, pop3_server, pop3_port,"
                           "smtp_server, smtp_port) VALUES ('%1', '%2', '%3', %4, '%5', %6);"
                           ).arg(username, password, pop_server).arg(pop_port).arg(smtp_server).arg(smtp_port));
        return true;
    }
    return false;
}
