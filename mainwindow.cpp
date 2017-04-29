#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sendwindow.h"
#include "accountmanager.h"
#include "creataccount.h"
#include "showmessage.h"
#include "newfolder.h"
#include "receive.h"
#include <QMessageBox>
#include <QPalette>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MailBox");
    setWindowIcon(QIcon("head.ico"));
    ui->statusBar->setMaximumHeight(5);
    unique_id = "";
    ui->actionForward->setDisabled(true);
    ui->actionReply->setDisabled(true);
    ui->actionDelete->setDisabled(true);

    QPalette palette;
    palette=ui->textBrowser->palette();
    palette.setBrush(QPalette::Base,QBrush(Qt::NoBrush));
//    ui->textBrowser->setPalette(palette);
//    ui->treeView->setPalette(palette);
    //ui->tableView->setPalette(palette);
    ui->treeView->setFrameStyle(QFrame::NoFrame);
    ui->tableView->setFrameStyle(QFrame::NoFrame);
    ui->textBrowser->setFrameStyle(QFrame::NoFrame);

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);//can not edit
    ui->treeView->setHeaderHidden(true);//hide the header of QTreeview
    ui->treeView->setIconSize(QSize(23,23));//set icon size
    ui->treeView->setMaximumWidth(200);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//can not edit
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//can only select the hole row
    ui->tableView->setMaximumWidth(550);
    //ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(this,SIGNAL(show_tree_info()),this,SLOT(show_treeview_info()));
    connect(ui->tableView, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(show_rightmenu(const QPoint&)));
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(show_Tree_rightmenu(const QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAccount_triggered()
{
    CreatAccount* account = new CreatAccount(this);
    account->show();
    connect(account,SIGNAL(show_tree_info()),this,SLOT(show_treeview_info()));
}

void MainWindow::on_actionCompose_triggered()
{
    SendWindow *send = new SendWindow(this);
    send->show();
}

void MainWindow::show_treeview_info()
{
    Tree_model = new QStandardItemModel(ui->treeView);
    showmessage show_Treeview;
    show_Treeview.showTreeview(Tree_model);
    ui->treeView->setModel(Tree_model);
}

void MainWindow::show_Tree_rightmenu(const QPoint& pos)
{
    QModelIndex current_index = ui->treeView->indexAt(pos);
    update_index = current_index;
    if(!current_index.parent().isValid())
    {
        QMenu* rmenu = new QMenu(this);
        QAction* delete_account = rmenu->addAction("delete account");
        connect(delete_account, SIGNAL(triggered(bool)), this, SLOT(Delete_account()));
        rmenu->exec(QCursor::pos());
    }
    else if(current_index.parent().data().toString() == "Inbox")
    {
        QMenu* rmenu = new QMenu(this);
        QAction* delete_folder = rmenu->addAction("delete folder");
        connect(delete_folder, SIGNAL(triggered(bool)), this, SLOT(Delete_folder()));
        rmenu->exec(QCursor::pos());
    }
}

void MainWindow::Delete_account()
{
    QString current_account = update_index.data().toString();
    AccountManager &accountManager = AccountManager::getInstance();
    accountManager.remove(current_account);
    show_treeview_info();
}

void MainWindow::Delete_folder()
{
    QModelIndex account_index = update_index;
    while((account_index.parent()).isValid())
        account_index = account_index.parent();
    QSqlQuery q;
    q.exec(QString("DELETE FROM dir WHERE user = '%1' AND folder = '%2';")
           .arg(account_index.data().toString()).arg(update_index.data().toString()));
    MainWindow::on_treeView_clicked(update_index);
    show_treeview_info();
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    update_index = index;
    unique_id = "";
    QModelIndex account_index = update_index;
    while((account_index.parent()).isValid())
        account_index = account_index.parent();
    AccountManager &accountManager = AccountManager::getInstance();
    accountManager.change(account_index.data().toString());

    ui->actionForward->setDisabled(true);
    ui->actionReply->setDisabled(true);
    ui->actionDelete->setDisabled(true);
    Table_model = new QStandardItemModel(ui->tableView);
    showmessage show_TableView;
    show_TableView.showTableview(Table_model,index);
    ui->tableView->setModel(Table_model);
    //ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnWidth(0,150);
    ui->tableView->setColumnWidth(1,200);
    ui->tableView->setColumnWidth(2,200);
}

void MainWindow::show_rightmenu(const QPoint& pos)
{
    QModelIndex current_index = ui->tableView->indexAt(pos);
    int tableView_current_row = current_index.row();
    if(tableView_current_row<0)//click the blank space
        return;
    QString type,userName;
    QAbstractItemModel* m = (QAbstractItemModel*)current_index.model();
    unique_id = m->index(tableView_current_row,3).data().toString();
    //QModelIndex type_index = m->index(tableView_current_row,4);//find the uid of selected mail
    type = m->index(tableView_current_row,4).data().toString();
    userName = m->index(tableView_current_row,5).data().toString();
    if(type == "inbox" || type == "folder")
    {
        QMenu* rmenu = new QMenu(this);
        QAction* Reply = rmenu->addAction("reply");
        QAction* Forward = rmenu->addAction("forward");
        QAction* Move = rmenu->addAction("move to");
        QAction* Delete = rmenu->addAction("delete");
        connect(Forward, SIGNAL(triggered(bool)), this, SLOT(Forward()));
        connect(Reply, SIGNAL(triggered(bool)), this, SLOT(Reply()));
        connect(Delete, SIGNAL(triggered(bool)), this, SLOT(Delete()));
        QMenu* submenu = new QMenu(this);
        connect(submenu, SIGNAL(triggered(QAction *)), this, SLOT(move_to_folder(QAction*)));
        QSqlQuery q;
        q.exec(QString("SELECT folder FROM dir WHERE user = '%1'").arg(userName));
        while(q.next())
        {
            QAction* folder_name = new QAction(submenu);
            folder_name->setText(q.value(0).toString());
            submenu->addAction(folder_name);
        }
        submenu->addAction("new folder");
        Move->setMenu(submenu);
        rmenu->exec(QCursor::pos());
    }
    else if(type == "draft")
    {
        QMenu* rmenu = new QMenu(this);
        QAction* edit = rmenu->addAction("edit");
        QAction* full_delete = rmenu->addAction("full delete");
        connect(full_delete, SIGNAL(triggered(bool)), this, SLOT(Full_delete()));
        connect(edit, SIGNAL(triggered(bool)), this, SLOT(Edit()));
        rmenu->exec(QCursor::pos());
    }
    else
    {
        QMenu* rmenu = new QMenu(this);
        QAction* full_delete = rmenu->addAction("full delete");
        connect(full_delete, SIGNAL(triggered(bool)), this, SLOT(Full_delete()));
        rmenu->exec(QCursor::pos());
    }
//    else
//    {
//        QMenu* rmenu = new QMenu(this);
//        QAction* full_delete = rmenu->addAction("full delete");
//        connect(full_delete, SIGNAL(triggered(bool)), this, SLOT(Full_delete()));
//        QAction* Forward = rmenu->addAction("forward");
//        connect(Forward, SIGNAL(triggered(bool)), this, SLOT(Forward()));
//        rmenu->exec(QCursor::pos());
//    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    QAbstractItemModel* m = (QAbstractItemModel*)index.model();
    unique_id = m->index(row,3).data().toString();
    QSqlQuery q;
    q.exec(QString("SELECT downloaded FROM receive_mail WHERE uid = '%1';").arg(unique_id));
    if(q.value("downloaded").toInt() == 0)
    {
        Receive receive_body;
        receive_body.receiveBody(unique_id);
    }
    q.exec(QString("SELECT body FROM receive_mail WHERE uid = '%1';").arg(unique_id));
    ui->textBrowser->setText(q.value(0).toString());

    ui->actionDelete->setEnabled(true);
    QString type = update_index.data().toString();
    if(type== "Draft" || type == "Sent" || type == "Trash")
    {
        ui->actionReply->setDisabled(true);
        ui->actionForward->setDisabled(true);
    }
    else if(unique_id != "")
    {
        ui->actionForward->setEnabled(true);
        ui->actionReply->setEnabled(true);
    }
}

void MainWindow::on_treeView_collapsed()
{
    ui->tableView->setModel(NULL);
    ui->textBrowser->clear();
    unique_id = "";
    ui->actionForward->setDisabled(true);
    ui->actionReply->setDisabled(true);
    ui->actionDelete->setDisabled(true);
}

void MainWindow::Reply()
{
    SendWindow *send = new SendWindow(this);
    connect(this, SIGNAL(reply_mail(QString&,int)), send, SLOT(Replymail(QString&,int)));
    send->show();
    emit reply_mail(unique_id,1);
}

void MainWindow::on_actionReply_triggered()
{
        MainWindow::Reply();
}

void MainWindow::Forward()
{
    SendWindow *send = new SendWindow(this);
    connect(this, SIGNAL(forward_mail(QString&,int)), send, SLOT(Replymail(QString&,int)));
    send->show();
    emit forward_mail(unique_id,2);
}

void MainWindow::on_actionForward_triggered()
{
        MainWindow::Forward();
}

void MainWindow::move_to_folder(QAction* action)
{
    if(action->text() != "new folder")
    {
        QSqlQuery query;
        query.exec(QString("UPDATE receive_mail SET folder = '%1' WHERE uid = '%6'").arg(action->text()).arg(unique_id));
        MainWindow::on_treeView_clicked(update_index);
    }
    else
    {
        NewFolder* newfolder = new NewFolder(this);
        connect(this, SIGNAL(creat_newfolder(const QString&)), newfolder, SLOT(save_account(const QString&)));
        QModelIndex account_index = update_index;
        while((account_index.parent()).isValid())
            account_index = account_index.parent();
        emit creat_newfolder(account_index.data().toString());
        if(newfolder->exec() == QDialog::Accepted)
            show_treeview_info();
    }
}

void MainWindow::Full_delete()
{
    QSqlQuery query;
    query.exec(QString("DELETE FROM receive_mail WHERE uid='%1'").arg(unique_id));
    MainWindow::on_treeView_clicked(update_index);
}

void MainWindow::Delete()
{
    QSqlQuery query;
    query.exec(QString("UPDATE receive_mail SET deleted = '%1' WHERE uid='%2'").arg(1).arg(unique_id));
    MainWindow::on_treeView_clicked(update_index);
}

void MainWindow::on_actionDelete_triggered()
{
    QString type = update_index.data().toString();
    if(type== "Draft" || type == "Sent" || type == "Trash")
        MainWindow::Full_delete();
    else
        MainWindow::Delete();
}

void MainWindow::Edit()
{
    SendWindow *send = new SendWindow(this);
    connect(this, SIGNAL(edit_mail(QString&)), send, SLOT(Editmail(QString&)));
    send->show();
    emit edit_mail(unique_id);
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{

}
