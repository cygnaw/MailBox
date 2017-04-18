#ifndef INITDB_H
#define INITDB_H

#include <QtSql>

QSqlError initDb() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    if (tables.contains("account", Qt::CaseInsensitive) &&
            tables.contains("mail_info", Qt::CaseInsensitive) &&
            tables.contains("mail_body", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;
    // open foreign key constraint
    if (!q.exec(QLatin1String("PRAGMA foreign_keys = ON;")))
        return q.lastError();
    // create table accout
    if (!q.exec(QLatin1String("create table if not exists account("
                              "username     text primary key    not null unique,"
                              "password     text                not null,"
                              "pop_server   text                not null,"
                              "pop_port     integer             not null,"
                              "smtp_server  text                not null,"
                              "smtp_port    integer             not null,"
                              "check (username like '%@%.%')"
                              ");")))
        return q.lastError();
    // create table mail_info
    if (!q.exec(QLatin1String("create table if not exists mail_info ("
                              "m_id    integer primary key autoincrement not null,"
                              "owner   text not null, "
                              "folder  text not null default 'Inbox',"
                              "sender  text,"
                              "recipient text,"
                              "subject text,"
                              "foreign key(owner) references accout(username),"
                              "check (folder in ('Inbox', 'Send', 'Draft', 'Spam'))"
                              ");")))
        return q.lastError();
    // create table mail_body
    if (!q.exec(QLatin1String("create table if not exists mail_body ("
                              "m_id integer primary key not null,"
                              "body text,"
                              "foreign key(m_id) references mail_info(m_id)"
                              ");")))
        return q.lastError();

    return QSqlError();
}

#endif // INITDB_H
