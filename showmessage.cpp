#include "showmessage.h"
#include "receive.h"
#include <QtSql>
#include <QStandardItem>
#include <QMap>
#include <QMessageBox>
showmessage::showmessage()
{
}

showmessage::~showmessage()
{
}

bool showmessage::showTreeview(QStandardItemModel* Tree_model)
{
    QMap<QString, QIcon> m_Iconmap;
    //insert the icons
    m_Iconmap[QStringLiteral("Inbox")] = QIcon(QStringLiteral("e:/MailBox/icons/Inbo.ico"));
    m_Iconmap[QStringLiteral("Draft")] = QIcon(QStringLiteral("e:/MailBox/icons/draft.ico"));
    m_Iconmap[QStringLiteral("Sent")] = QIcon(QStringLiteral("e:/MailBox/icons/sent.ico"));
    m_Iconmap[QStringLiteral("Trash")] = QIcon(QStringLiteral("e:/MailBox/icons/trash.ico"));
    m_Iconmap[QStringLiteral("Folder")] = QIcon(QStringLiteral("e:/MailBox/icons/folder.ico"));
    //q1 is used to select accounts while q2 is used to select folders
    QSqlQuery q1,q2;
    QStandardItem* item;
    QStandardItem* inbox;
    QStandardItem* draft;
    QStandardItem* sent;
    QStandardItem* trash;
    QStandardItem* folder;
    q1.exec("SELECT username FROM account;");
    while(q1.next())
    {
        QString user = q1.value(0).toString();
        item = new QStandardItem(user);
        Tree_model->appendRow(item);
        inbox = new QStandardItem(m_Iconmap[QStringLiteral("Inbox")],QStringLiteral("Inbox"));
        item->appendRow(inbox);
        q2.exec(QString("SELECT folder FROM dir WHERE user = '%1'").arg(user));
        while(q2.next())
        {
            QString folder_name = q2.value(0).toString();
            folder = new QStandardItem(m_Iconmap[QStringLiteral("Folder")],QStringLiteral("%1").arg(folder_name));
            inbox->appendRow(folder);
        }
        draft = new QStandardItem(m_Iconmap[QStringLiteral("Draft")],QStringLiteral("Draft"));
        item->appendRow(draft);
        sent = new QStandardItem(m_Iconmap[QStringLiteral("Sent")],QStringLiteral("Sent"));
        item->appendRow(sent);
        trash = new QStandardItem(m_Iconmap[QStringLiteral("Trash")],QStringLiteral("Trash"));
        item->appendRow(trash);
    }
    return true;
}

bool showmessage::showTableview(QStandardItemModel* Table_model,const QModelIndex &index)
{
    if(!index.parent().isValid())
        return false;//meaning that the index points the root directory
    QModelIndex rootIndex = index.parent();
    //get tht rootIndex
    while((rootIndex.parent()).isValid())
    {
        rootIndex = rootIndex.parent();
    }
    QString indexName = index.data().toString();
    QString indexParentName = index.parent().data().toString();
    QString rootIndexName = rootIndex.data().toString();
    //show headers of tableview
    Table_model->setColumnCount(6);
    if(indexName == "Inbox" || indexName == "Trash" || indexParentName == "Inbox")
        Table_model->setHeaderData(0,Qt::Horizontal,"From");
    else
        Table_model->setHeaderData(0,Qt::Horizontal,"To");
    Table_model->setHeaderData(1,Qt::Horizontal,"Subject");
    Table_model->setHeaderData(2,Qt::Horizontal,"Data");
    Table_model->setHeaderData(3,Qt::Horizontal,"Uid");
    Table_model->setHeaderData(4,Qt::Horizontal,"Type");
    Table_model->setHeaderData(5,Qt::Horizontal,"User");
    QSqlQuery q;
    QStandardItem* item0;
    QStandardItem* item1;
    QStandardItem* item2;
    QStandardItem* item3;
    QStandardItem* item4;
    QStandardItem* item5;
    if(indexParentName == "Inbox")
    {//show the information of selected folder
        q.exec(QString("SELECT deleted, sender, subject, date, uid FROM receive_mail WHERE user = '%1' and folder = '%2'").arg(rootIndexName).arg(indexName));
        int i = 0;
        while(q.next())
        {
            if(q.value(QLatin1String("deleted")).toInt() == 0)
            {
                item0 = new QStandardItem(q.value(QLatin1String("sender")).toString());
                Table_model->setItem(i,0,item0);
                Table_model->item(i,0)->setTextAlignment(Qt::AlignCenter);
                item1 = new QStandardItem(q.value(QLatin1String("subject")).toString());
                Table_model->setItem(i,1,item1);
                Table_model->item(i,1)->setTextAlignment(Qt::AlignCenter);
                item2 = new QStandardItem(q.value(QLatin1String("date")).toString());
                Table_model->setItem(i,2,item2);
                Table_model->item(i,2)->setTextAlignment(Qt::AlignCenter);
                item3 = new QStandardItem(q.value(QLatin1String("uid")).toString());
                Table_model->setItem(i,3,item3);
                Table_model->item(i,3)->setTextAlignment(Qt::AlignCenter);
                item4 = new QStandardItem("folder");
                Table_model->setItem(i,4,item4);
                Table_model->item(i,4)->setTextAlignment(Qt::AlignCenter);
                item5 = new QStandardItem(rootIndexName);
                Table_model->setItem(i,5,item5);
                Table_model->item(i,5)->setTextAlignment(Qt::AlignCenter);
                i++;
            }
        }
    }
    else if(indexName == "Inbox")
    {//show the information of inbox
        QCoreApplication::processEvents();
        Receive receive_mail;
        receive_mail.receiveHeaders();
        q.exec(QString("SELECT deleted, sender, subject, date, uid FROM receive_mail WHERE user = '%1'").arg(rootIndexName));
        int i = 0;
        while(q.next())
        {
            if(q.value(QLatin1String("deleted")).toInt() == 0)
            {
                item0 = new QStandardItem(q.value(QLatin1String("sender")).toString());
                Table_model->setItem(i,0,item0);
                Table_model->item(i,0)->setTextAlignment(Qt::AlignCenter);
                item1 = new QStandardItem(q.value(QLatin1String("subject")).toString());
                Table_model->setItem(i,1,item1);
                Table_model->item(i,1)->setTextAlignment(Qt::AlignCenter);
                item2 = new QStandardItem(q.value(QLatin1String("date")).toString());
                Table_model->setItem(i,2,item2);
                Table_model->item(i,2)->setTextAlignment(Qt::AlignCenter);
                item3 = new QStandardItem(q.value(QLatin1String("uid")).toString());
                Table_model->setItem(i,3,item3);
                Table_model->item(i,3)->setTextAlignment(Qt::AlignCenter);
                item4 = new QStandardItem("inbox");
                Table_model->setItem(i,4,item4);
                Table_model->item(i,4)->setTextAlignment(Qt::AlignCenter);
                item5 = new QStandardItem(rootIndexName);
                Table_model->setItem(i,5,item5);
                Table_model->item(i,5)->setTextAlignment(Qt::AlignCenter);
                i++;
            }
        }
    }
    else if(indexName == "Trash")
    {//show the information of trash
        q.exec(QString("SELECT deleted, sender, subject, date, uid FROM receive_mail WHERE user = '%1'").arg(rootIndexName));
        int i = 0;
        while(q.next())
        {
            if(q.value(QLatin1String("deleted")).toInt() == 1)
            {
                item0 = new QStandardItem(q.value(QLatin1String("sender")).toString());
                Table_model->setItem(i,0,item0);
                Table_model->item(i,0)->setTextAlignment(Qt::AlignCenter);
                item1 = new QStandardItem(q.value(QLatin1String("subject")).toString());
                Table_model->setItem(i,1,item1);
                Table_model->item(i,1)->setTextAlignment(Qt::AlignCenter);
                item2 = new QStandardItem(q.value(QLatin1String("date")).toString());
                Table_model->setItem(i,2,item2);
                Table_model->item(i,2)->setTextAlignment(Qt::AlignCenter);
                item3 = new QStandardItem(q.value(QLatin1String("uid")).toString());
                Table_model->setItem(i,3,item3);
                Table_model->item(i,3)->setTextAlignment(Qt::AlignCenter);
                item4 = new QStandardItem("trash");
                Table_model->setItem(i,4,item4);
                Table_model->item(i,4)->setTextAlignment(Qt::AlignCenter);
                item5 = new QStandardItem(rootIndexName);
                Table_model->setItem(i,5,item5);
                Table_model->item(i,5)->setTextAlignment(Qt::AlignCenter);
                i++;
            }
        }
    }
    else if(indexName == "Sent")
    {//show the information of sent
        q.exec(QString("SELECT sended, receiver, subject, date, uid FROM send_mail WHERE user = '%1'").arg(rootIndexName));
        int i = 0;
        while(q.next())
        {
            if(q.value(QLatin1String("sended")).toInt() == 1)
            {
                item0 = new QStandardItem(q.value(QLatin1String("receiver")).toString());
                Table_model->setItem(i,0,item0);
                Table_model->item(i,0)->setTextAlignment(Qt::AlignCenter);
                item1 = new QStandardItem(q.value(QLatin1String("subject")).toString());
                Table_model->setItem(i,1,item1);
                Table_model->item(i,1)->setTextAlignment(Qt::AlignCenter);
                item2 = new QStandardItem(q.value(QLatin1String("date")).toString());
                Table_model->setItem(i,2,item2);
                Table_model->item(i,2)->setTextAlignment(Qt::AlignCenter);
                item3 = new QStandardItem(q.value(QLatin1String("uid")).toString());
                Table_model->setItem(i,3,item3);
                Table_model->item(i,3)->setTextAlignment(Qt::AlignCenter);
                item4 = new QStandardItem("sent");
                Table_model->setItem(i,4,item4);
                Table_model->item(i,4)->setTextAlignment(Qt::AlignCenter);
                item5 = new QStandardItem(rootIndexName);
                Table_model->setItem(i,5,item5);
                Table_model->item(i,5)->setTextAlignment(Qt::AlignCenter);
                i++;
            }
        }
    }
    else
    {//show the information of draft
        q.exec(QString("SELECT sended, receiver, subject, date, uid FROM send_mail WHERE user = '%1'").arg(rootIndexName));
        int i = 0;
        while(q.next())
        {
            if(q.value(QLatin1String("sended")).toInt() == 0)
            {
                item0 = new QStandardItem(q.value(QLatin1String("receiver")).toString());
                Table_model->setItem(i,0,item0);
                Table_model->item(i,0)->setTextAlignment(Qt::AlignCenter);
                item1 = new QStandardItem(q.value(QLatin1String("subject")).toString());
                Table_model->setItem(i,1,item1);
                Table_model->item(i,1)->setTextAlignment(Qt::AlignCenter);
                item2 = new QStandardItem(q.value(QLatin1String("date")).toString());
                Table_model->setItem(i,2,item2);
                Table_model->item(i,2)->setTextAlignment(Qt::AlignCenter);
                item3 = new QStandardItem(q.value(QLatin1String("uid")).toString());
                Table_model->setItem(i,3,item3);
                Table_model->item(i,3)->setTextAlignment(Qt::AlignCenter);
                item4 = new QStandardItem("draft");
                Table_model->setItem(i,4,item4);
                Table_model->item(i,4)->setTextAlignment(Qt::AlignCenter);
                item5 = new QStandardItem(rootIndexName);
                Table_model->setItem(i,5,item5);
                Table_model->item(i,5)->setTextAlignment(Qt::AlignCenter);
                i++;
            }
        }
    }
    return true;
}

void showmessage::show_processbar(int min, int max, QProgressDialog* processbar)
{
    for(int i=min;i<=max;i++)
    {
        QCoreApplication::processEvents();
        processbar->setValue(i);
    }

}
