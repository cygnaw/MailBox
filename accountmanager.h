#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QString>
#include <QtSql>

class AccountManager
{
public:
    static AccountManager& getInstance() {
        static AccountManager accountManager;
        return accountManager;
    }
    QString getUser() { return username; }
    QString getPass() { return password; }
    QString getPopServer() { return pop_server; }
    int getPopPort() { return pop_port; }
    QString getSmtpServer() { return smtp_server; }
    int getSmtpPort() { return smtp_port; }

    void change(const QSqlRecord &record);
    bool modify(const QString &username, const QString &password,
                const QString &pop_server, int pop_port,
                const QString &smtp_server, int smtp_port);
    void remove(QString username, bool withMail);

    static bool validatePop(const QString &user, const QString &pass,
                            const QString &server, int port);
    static bool validateSmtp(const QString &user, const QString &pass,
                             const QString &server, int port);
    static bool add(const QString &username, const QString &password,
                    const QString &pop_server, int pop_port,
                    const QString &smtp_server, int smtp_port);
private:
    QString username;
    QString password;
    QString pop_server;
    int pop_port;
    QString smtp_server;
    int smtp_port;

    AccountManager();
    AccountManager(AccountManager const&) {}
    AccountManager& operator=(AccountManager const&);
    ~AccountManager();
};

#endif // ACCOUNTMANAGER_H
