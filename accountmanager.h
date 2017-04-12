#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H


class AccountManager
{
public:
    static AccountManager& getInstance() {
        static AccountManager accountManager;
        return accountManager;
    }

private:
    AccountManager();
    AccountManager(AccountManager const&) {}
    AccountManager& operator=(AccountManager const&);
    ~AccountManager();
};

#endif // ACCOUNTMANAGER_H
