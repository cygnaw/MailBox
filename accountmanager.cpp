    #include "accountmanager.h"
    #include "lib/pop3.h"
    #include "lib/smtp.h"
#include <QDebug>

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

    void AccountManager::change(const QString &username) {
        QSqlQuery query;
        query.exec(QString("SELECT password, pop_server, pop_port, smtp_server, smtp_port FROM account where username='%1';").arg(username));
        query.next();
        this->username = username;
        password = query.value(QLatin1String("password")).toString();
        pop_server = query.value(QLatin1String("pop_server")).toString();
        pop_port = query.value(QLatin1String("pop_port")).toInt();
        smtp_server = query.value(QLatin1String("smtp_server")).toString();
        smtp_port = query.value(QLatin1String("smtp_port")).toInt();
    }

    bool AccountManager::modify(const QString &username, const QString &password,
                                const QString &pop_server, int pop_port,
                                const QString &smtp_server, int smtp_port) {
        if (validatePop(username, password, pop_server, pop_port) &&
                validateSmtp(username, password, smtp_server, smtp_port)) {
            QSqlQuery query;
            query.exec(QString("UPDATE account SET password = '%1', pop_server = '%2', pop_port = %3,"
                       "smtp_server = '%4', smtp_port = '%5' WHERE username = %6").arg(
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

    void AccountManager::remove(const QString &username) {
        QSqlQuery query;

        query.exec(QString("DELETE FROM account WHERE username='%1';").arg(username));
        if (this->username == username) {
            query.exec("SELECT username, password, pop_server, pop_port, smtp_server, smtp_port FROM account;");
            query.next();
            this->username = query.value(QLatin1String("username")).toString();
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
        bool valid;
        QString resp;
        valid = pop.user(user, resp) && pop.pass(pass, resp);
        pop.quit(resp);
        return valid;
    }

    bool AccountManager::validateSmtp(const QString &user, const QString &pass,
                                      const QString &server, int port) {
        Smtp  smtp;
        bool valid;
        valid = smtp.connect(server,port);
        if(valid)
        {
            valid = smtp.login(user,pass);
            return valid;
        }
        return valid;
    }

    bool AccountManager::add(const QString &username, const QString &password,
                             const QString &pop_server, int pop_port,
                             const QString &smtp_server, int smtp_port) {
        if (validatePop(username, password, pop_server, pop_port) &&
                validateSmtp(username, password, smtp_server, smtp_port)) {
            QSqlQuery query;
            query.exec(QString("INSERT INTO account (username, password, pop_server, pop_port,"
                               "smtp_server, smtp_port) VALUES ('%1', '%2', '%3', %4, '%5', %6);"
                               ).arg(username, password, pop_server).arg(pop_port).arg(smtp_server).arg(smtp_port));
            query.exec(QString("INSERT INTO dir (user, folder) VALUES ('%1', 'Default');").arg(username));
            return true;
        }
        return false;
    }

    bool AccountManager::isEmpty() {
        QSqlQuery query;
        query.exec(QLatin1String("SELECT count(*) FROM account;"));
        query.next();
        return query.value(0).toInt() == 0;
    }
