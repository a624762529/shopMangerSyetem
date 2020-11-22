#ifndef LOADVIEW_H
#define LOADVIEW_H
#include <memory>
#include <QMessageBox>
#include <QWidget>
#include"sockclient.h"
#include"service.h"
using namespace std;
namespace Ui {
class LoadView;
}

class LoadView : public QWidget
{
    Q_OBJECT
public:
    explicit LoadView(QWidget *parent = 0);
    ~LoadView();
    void setSendSer(shared_ptr<SockClient>);
signals:
    void load();
    void sig_SolderLoad();
    void logon();
    void loadSuccess();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
public:
    Ui::LoadView *ui;
    shared_ptr<SockClient> cli;
};

#endif // LOADVIEW_H
