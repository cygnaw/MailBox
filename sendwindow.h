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

private slots:
    void on_sendbutton_clicked();

private:
    Ui::SendWindow *ui;
};

#endif // SENDWINDOW_H
