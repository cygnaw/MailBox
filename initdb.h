#ifndef INITDB_H
#define INITDB_H

#include <QtSql>

QSqlError initDb() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mailbox.db");

    if (!db.open())
        return db.lastError();


}

#endif // INITDB_H
