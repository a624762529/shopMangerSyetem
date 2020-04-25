#ifndef CHARGEVIEW_H
#define CHARGEVIEW_H

#include <QWidget>
#include<QString>
#include<string>
#include<iostream>
#include<memory>
#include<QStandardItemModel>
#include"sockclient.h"
#include"good.h"
#include"service.h"
#include<QMessageBox>
#include <QFileDialog>
#include"file.h"
#include<sstream>
#include<iomanip>
using namespace std;
namespace Ui
{
class ChargeView;
}

class ChargeView : public QWidget
{
    Q_OBJECT
public:
    explicit ChargeView(QWidget *parent = 0);
    ~ChargeView();
public:
    void setSendSer(shared_ptr<SockClient> cli);
private slots:
    void on_salButton_clicked();            //商品的销售
    void on_pushButton_deleteting_clicked();//删除商品
    void on_pushButton_change_clicked();    //改变商品
    void on_pushButton_addInfo_clicked();   //增加商品
    void on_find_button_clicked();          //寻找商品
    void on_pushButton_2_clicked();         //调货
    void on_button_showstr_clicked();       //查询全部仓库
    void on_pushButton_clicked();           //删除仓库
    void on_pushButton_addStre_clicked();   //增加仓库
    void on_butt_SalAll_clicked();          //分类统计
    void on_buttonClassSalar_clicked();     //销售统计
    void on_buttonEarlyWarn_clicked();      //销售预警
    void on_buttonClassSalar_2_clicked();   //查询该仓库的全部商品
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    void setTableViewHead();
private:
    void  stableReadHead(SendBack *back); //读取首部
    char* stableReadInfo(int back_len);//读取信息
private:


public:
    void setModelGoods     (QStandardItemModel &mode,Goods *good,int line);
    void setModelStore     (QStandardItemModel &mode,Store *store,int line);
    void setModelCls       (QStandardItemModel &mode,RetCls *cls,int line);
    void setModelSoldCent  (QStandardItemModel &mode,Goods *good,int line);
    void setModelLine      (QStandardItemModel &mode,int line);
    void setModelSoldCe    (QStandardItemModel &mode,Goods *good,int line);
private:
    pair<char*,int> readExp(int per_size);
private:
    QStandardItemModel model_ShowAll;
    QStandardItemModel model_Find;
    QStandardItemModel model_ShowAllStore;
    QStandardItemModel model_Cls;
    QStandardItemModel model_SailCensus;
    QStandardItemModel model_EarlyWarn;

private:
    Ui::ChargeView *ui;
    shared_ptr<SockClient> client;
};

#endif // CHARGEVIEW_H
