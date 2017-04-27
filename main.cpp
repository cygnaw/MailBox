#include "mainwindow.h"
#include "initdb.h"
#include <QApplication>
#include <QTableView>
#include <QtSql>
#include <QMessageBox>
#include <QFile>
#include "lib/pop3.h"
#include "initdb.h"
#include "accountmanager.h"
#include "creataccount.h"
#include "lib/decodestrategy.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    QFile file("e:/MailBox/qss0.qss");
    if( file.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
        file.close();
    }
    else
    {
        QMessageBox::warning(NULL, "warning", "Open failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

    initDb();
    /////////////////////////////////////////////////////
    AccountManager::add("kaijietang@sina.cn","guessmypwd","pop.sina.cn",110,"smtp.sina.cn",25);
    AccountManager::add("peiyunjia@sina.com","jia123","pop.sina.cn",110,"smtp.sina.cn",25);
    QSqlQuery q;
    q.exec(QString("INSERT INTO send_mail (user, sended, sender, receiver, date, subject, body)"
                   "VALUES ('peiyunjia@sina.com', 1, 'peiyunjia@sina.com', 'taylover_pei@163.com', '04-26', 'ceshi1', 'jiayunpei');"));
    q.exec(QString("INSERT INTO send_mail (user, sended, sender, receiver, date, subject, body)"
                   "VALUES ('peiyunjia@sina.com', 1, 'peiyunjia@sina.com', 'jiayunpei@126.com', '04-26', 'ceshi2', 'peiyunjia');"));
    q.exec(QString("INSERT INTO send_mail (user, sended, sender, receiver, date, subject, body)"
                   "VALUES ('peiyunjia@sina.com', 0, 'peiyunjia@sina.com', 'jiayunpei@126.com', '04-26', 'caogao1', 'peiyunjia');"));
    q.exec(QString("INSERT INTO dir (user, folder)"
                   "VALUES ('peiyunjia@sina.com', 'folder1');"));
    q.exec(QString("INSERT INTO dir (user, folder)"
                   "VALUES ('peiyunjia@sina.com', 'wenjianjia');"));
    q.exec(QString("INSERT INTO receive_mail (uid, user, folder, deleted, downloaded, sender, receiver, subject, date, body)"
                   "VALUES ('xxyy11', 'peiyunjia@sina.com', 'folder1', 0, 1, 'kaijietang@sina.cn', 'peiyunjia@sina.com',"
                   "'jiayunpei1', '04-27', 'This is a test.');"));
    q.exec(QString("INSERT INTO receive_mail (uid, user, folder, deleted, downloaded, sender, receiver, subject, date, body)"
                   "VALUES ('xxyy22', 'peiyunjia@sina.com', 'folder1', 0, 1, 'jiayunpei@126.com', 'peiyunjia@sina.com',"
                   "'peiyunjia', '04-22', 'Hello world.');"));
 /////////////////////////////////////////////////////////////////////////

    MainWindow w;
    AccountManager &accountManager = AccountManager::getInstance();
    if(accountManager.isEmpty())
    {
        CreatAccount account;
        if(account.exec() == QDialog::Accepted)
        {
            emit w.show_tree_info();
            w.show();
            return a.exec();
        }
    }
    else
    {
        emit w.show_tree_info();
        w.show();
        return a.exec();
    }


//    accountsetting account;
//    account.show();
//    QSqlQuery q;
//    q.exec("SELECT username FROM account");
//    if(q.next())
//    {
//        w.show();
//    }
//    MainWindow w;
//    emit w.show_tree_info();
//    w.show();
//    Pop3 pop("pop3.sina.cn");
//    QString resp;
//    QStringList list;
//    int octets;
//    int num, size;

//    pop.user("kaijietang@sina.cn", resp);
//    pop.pass("guessmypwd", resp);

//    pop.retr(1, resp, list, octets);
//    foreach (resp, list) {
//        qDebug() << resp;
//    }

//    pop.quit(resp);
*/
    QStringList l;
    l << "ttS/ucj8vda3u8HavtPKpbWuv+zA1ri9vfxzYWS9+Lfnv9rAy7zisKLLucH";
    l << "Lt6jCyb/yvNwNCrCituTKrrfWvOi/4MCtyPi9+Lfnv9rBy7XayP28vreows";
    l << "m/8rzcsKG3xbzZwK2/y8u/veK3xcK3v6jLwLv6wcsNCrbgydm8trCht9bA5";
    l << "b+ouLG/xry2sKHL6rXEwM+4vr/VvOTA77+oveHL48Dvv6i1xL7NysfBy7/V";
    l << "vOTKtbXYwcu94mxhZMu8v7wNCsvqtcTAz7i+v6jArcu5v8K077fnwaa/ybT";
    l << "vyPi30nNhZLeowLy/y8mvwK212b/L";
    DecodeStrategy* decoder = new DecodeBase64();
    QString result;
    decoder->decode(l, result);
    qDebug() << result;
    return a.exec();
}
