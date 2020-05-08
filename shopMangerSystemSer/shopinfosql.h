#ifndef SHOPINFOSQL_H
#define SHOPINFOSQL_H
#include<QSqlDatabase>
#include<QDebug>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include "head.h"
#include"service.h"
#include<QSqlQueryModel>
#include<string>

using namespace std;
class ShopActSql
{
public:
    ShopActSql(string db_name);
    ~ShopActSql();
    ShopActSql(const ShopActSql&)=delete;
    void operator ==(const ShopActSql&)=delete;
    bool doSql(string sql_lag);   //增 删 改 建表
    bool selectDB(string sql_lag);//查接口

    SendBack* selectDB(string sql_lag,int arg);
    vector<string> readType();
    vector<string> readStore();
    void    sendBackType();
    void    sendBackStore();
    QString Transform(QString info);
    Tg      getVal(QString sql);
public:
    SendBack* searchItem  (string sql_lag,int arg);
    SendBack* getAllStore (string sql_lag,int arg);
    SendBack* clsGoods    (string sql_lag,int arg);
    SendBack* soldCensus  (string sql_lag,int arg);
    SendBack* earlyWarning(string sql_lag,int arg);
public:
    int       getLen      (string sql);
public:
    std::string  m_dbname;//sql名字
    QSqlDatabase m_db_act; //数据库操作
};

#endif // SHOPINFOSQL_H
