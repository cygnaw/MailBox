#ifndef SHOWMESSAGE_H
#define SHOWMESSAGE_H
#include <QStandardItemModel>
#include <QProgressDialog>

class showmessage
{
public:
    showmessage();
    ~showmessage();
    bool showTreeview(QStandardItemModel* Tree_model);
    bool showTableview(QStandardItemModel* Table_model,const QModelIndex &index);
    bool showTextbrowser();
    void show_processbar(int min, int max, QProgressDialog* processbar);
    //int tableView_current_row = 0;
};

#endif // SHOWMESSAGE_H
