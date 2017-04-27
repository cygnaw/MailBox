#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //is used to search information when there is a reply or forward command
    QString unique_id;

signals:
    void show_tree_info();

    void reply_mail(QString&, int);

    void forward_mail(QString&, int);

    void edit_mail(QString&);

    void creat_newfolder(const QString&);

private slots:

    void on_actionAccount_triggered();

    void on_actionCompose_triggered();

    void on_treeView_clicked(const QModelIndex &index);

    void show_treeview_info();

    void show_rightmenu(const QPoint&);

    void show_Tree_rightmenu(const QPoint&);

    void on_treeView_collapsed();

    void Reply();

    void Forward();

    void Full_delete();

    void Delete();

    void Edit();

    void move_to_folder(QAction*);

    void on_actionReply_triggered();

    void on_tableView_clicked(const QModelIndex &index);

    void on_actionForward_triggered();

    void on_actionDelete_triggered();

    void Delete_account();

    void Delete_folder();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QStandardItemModel* Tree_model = NULL;
    QStandardItemModel* Table_model = NULL;
    QModelIndex update_index;

};

#endif // MAINWINDOW_H
