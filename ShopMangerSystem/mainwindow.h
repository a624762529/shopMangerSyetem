#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"loadview.h"
#include"chargeview.h"
#include<memory>
#include"logon.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setSendSer(shared_ptr<SockClient> sendSer);
private:
    Ui::MainWindow *ui;
    LoadView               m_load;
    ChargeView             m_chargeViw;
    shared_ptr<SockClient> client;
    Logon                  m_log;
};

#endif // MAINWINDOW_H
