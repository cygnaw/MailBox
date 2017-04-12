#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>

class DbManager
{
public:
    static DbManager& getInstance() {
        static DbManager database;
        return database;
    }
private:
    QSqlDatabase m_db;
    DbManager();
    DbManager(DbManager const&);
    DbManager& operator=(DbManager const&);
    ~DbManager();
};

#endif // DBMANAGER_H
