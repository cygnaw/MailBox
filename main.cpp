#include "mainwindow.h"
#include "dbmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    qDebug() << QSqlDriver::hasFeature(QSqlDriver::Transactions);
    return 0;
}
