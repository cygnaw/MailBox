#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QString>

class AccountManager
{
public:
    static AccountManager& getInstance() {
        static AccountManager accountManager;
        return accountManager;
    }
    bool addAccount(QString username, QString password);
    void removeAccout(QString username, bool withMail);
private:
    AccountManager();
    AccountManager(AccountManager const&) {}
    AccountManager& operator=(AccountManager const&);
    ~AccountManager();
};

#endif // ACCOUNTMANAGER_H
