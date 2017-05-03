#ifndef INITDB_H
#define INITDB_H

#include <QtSql>

QSqlError initDb() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");

    if (!db.open())
        return db.lastError();

    QSqlQuery q;
    // open foreign key constraint
    if (!q.exec(QLatin1String("PRAGMA foreign_keys = ON;")))
        return q.lastError();

    QStringList tables = db.tables();
    if (tables.contains("account", Qt::CaseInsensitive) &&
            tables.contains("dir", Qt::CaseInsensitive) &&
            tables.contains("send_mail", Qt::CaseInsensitive) &&
            tables.contains("receive_mail", Qt::CaseInsensitive))
        return QSqlError();

    // create table account
    if (!q.exec(QLatin1String("create table if not exists account("
                              "username     text primary key    not null unique,"
                              "password     text                not null,"
                              "pop_server   text                not null,"
                              "pop_port     integer             not null default 110,"
                              "smtp_server  text                not null,"
                              "smtp_port    integer             not null default 25,"
                              "check (username like '%@%.%')"
                              ");")))
        return q.lastError();

    // create table dir
    if (!q.exec(QLatin1String("create table if not exists dir ("
                              "user text not null,"
                              "folder text not null,"
                              "primary key (user, folder),"
                              "foreign key (user) references account(username) ON DELETE CASCADE"
                              ");")))
        return q.lastError();

    // create table send_mail
    if (!q.exec(QLatin1String("create table if not exists send_mail ("
                              "uid    integer primary key autoincrement,"
                              "user   text not null,"
                              "sended integer not null,"
                              "sender text,"
                              "receiver   text,"
                              "cc   text,"
                              "bcc  text,"
                              "date text,"
                              "subject text,"
                              "body text,"
                              "foreign key(user) references account(username) ON DELETE CASCADE"
                              ");")))
        return q.lastError();

    // create table receive_mail
    if (!q.exec(QLatin1String("create table if not exists receive_mail ("
                              "uid   text primary key not null,"
                              "user   text not null,"
                              "folder text not null default 'Default',"
                              "deleted integer not null default 0,"
                              "downloaded integer not null default 0,"
                              "sender text,"
                              "receiver text,"
                              "cc   text,"
                              "subject text,"
                              "date text,"
                              "body text,"
                              "foreign key(user, folder) references dir(user, folder) ON DELETE CASCADE"
                              ");")))
        return q.lastError();

    return QSqlError();
}

#endif // INITDB_H
