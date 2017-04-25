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
            tables.contains("dir", Qt::CaseInsensitive) &&
            tables.contains("send_mail", Qt::CaseInsensitive) &&
            tables.contains("receive_mail", Qt::CaseInsensitive) &&
            tables.contains("mail_body", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;
    // open foreign key constraint
    if (!q.exec(QLatin1String("PRAGMA foreign_keys = ON;")))
        return q.lastError();
    // create table account
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
                              "uid    text primary key not null,"
                              "user   text not null,"
                              "sended integer not null,"
                              "from text,"
                              "to   text,"
                              "cc   text,"
                              "bcc  text,"
                              "subject text,"
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
                              "from text,"
                              "to   text,"
                              "cc   text,"
                              "subject text,"
                              "date text,"
                              "foreign key(user, folder) references dir(user, folder) ON DELETE CASCADE"
                              ");")))
        return q.lastError();

    // create table mail_body
    if (!q.exec(QLatin1String("create table if not exists mail_body ("
                              "uid text primary key not null,"
                              "body text,"
                              "foreign key(m_id) references mail_info(m_id) ON DELETE CASCADE"
                              ");")))
        return q.lastError();

    return QSqlError();
}

#endif // INITDB_H
