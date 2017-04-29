#ifndef SENDWINDOW_H
#define SENDWINDOW_H

#include <QMainWindow>

namespace Ui {
class SendWindow;
}

class SendWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SendWindow(QWidget *parent = 0);
    ~SendWindow();
    void show_comboBox();
    void get_currentAccount(const QString&);

private slots:
    void on_sendbutton_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_savebutton_clicked();

    void Replymail(QString& unique_id, int flag);

    void Editmail(QString&);

    void on_attachbutton_clicked();

private:
    Ui::SendWindow *ui;
    QString username;
    QString password;
    QString smtp_server;
    int smtp_port;
    QString pop_server;
    int pop_port;

    int is_reply_or_forward;//1 stands for reply command; 2 stands for forward command
    QString reply_forward_message;
};

#endif // SENDWINDOW_H
