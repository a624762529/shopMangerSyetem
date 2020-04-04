#ifndef LOGON_H
#define LOGON_H

#include <QWidget>
#include<QString>
#include<QMessageBox>
#include<iostream>
#include"sockclient.h"

#include"service.h"
using namespace std;

namespace Ui {
class Logon;
}



class Logon : public QWidget
{
    Q_OBJECT

public:
    explicit Logon(QWidget *parent = 0);
    ~Logon();
    void setSendSer(shared_ptr<SockClient> cli);
private slots:
    void on_pushButton_log_clicked();
    void on_pushButton_clicked();

signals:
    void logonSuccess();
private:
    shared_ptr<SockClient> client;
    Ui::Logon *ui;
};

#endif // LOGON_H
