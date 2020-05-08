#ifndef SERVICE_H
#define SERVICE_H
#pragma once
#include"goods.h"
#include<QString>
#include<iostream>

using namespace std;

void gettime_now(char *cur_time);

struct SendBack
{
    int m_len;  //信息长度
    int m_num;  //数据的数目
    bool m_tag; //sql语句是否执行成功
    char m_ch;  //数据内容
};

struct Tg
{
    int price=-1;
    int qua=-1;
    char type[20]{0};
};

struct Store
{
public:
    Store() {}
    Store(string name)
    {
        setStore(name);
    }
    void setStore(string name)
    {
        memcpy(arry_name,name.c_str(),name.size());
    }
public:
    char arry_name[20]{0};
};

struct RetCls
{
public:
    RetCls() {}
    void setType(string typev,int sizev)
    {
        memcpy(type,typev.c_str(),typev.size());
        size=sizev;
    }
public:
    char type[20];
    int  size;
};
class SendPackImpl
{
public:
    enum{
        replenish,   find,     charge_store,soldcensus,
      //补货           查询       管理仓库      销售统计
        deletegoods,changegoods,clsgoods,addgoods,Load, Login,salItem,earlyWarn,
      //删除商品      改变商品     分类统计  增加商品   登录   注册  售卖商品
        showStoreAll,solderLoad,solderLogin
      //展示全部的商品
    };
    SendPackImpl()
    {
        m_send_type=-1;
        m_info_len=-1;
    }
    SendPackImpl(int type)
    {
        m_send_type=type;
        m_info_len=-1;
    }
public:
    char m_username[20]{0};
    char m_status[20]{0};
    int m_send_type;//发送类型
    int m_info_len; //信息的长度
};



struct UserLoadInfo
{
    char user_name[20];
    char password[20];
};



//销售商品
class SalItem:public SendPackImpl
{
public:
    SalItem (const Goods &from )
        :SendPackImpl(SendPackImpl::salItem)
    {
        memset(&m_sal,0,sizeof(m_sal));
        memcmp(&m_sal,&from,sizeof(Goods));
        m_info_len=sizeof(SalItem);
    }
    pair<string,string> explain(int price)
    {
        QString changeValue=QString(
                    "UPDATE %1 set item_qua=item_qua-%2"
                    " where item_name='%3'"
                    ).arg(QString(m_sal.m_store),
                          QString::number(m_sal.m_qua),
                          QString( m_sal.m_name));
        char now_time[20]{0};
        gettime_now(now_time);

        QString insertSalTable=QString("insert into SalTable(store,item_name,"
                                       "time,price,qua)"
                                       "values('%1','%2','%3','%4','%5')")
                                .arg(m_sal.m_store,m_sal.m_name,now_time,
                                     QString::number(price),
                                     QString::number(m_sal.m_qua)
                                     );
        return pair<string,string>(changeValue.toStdString(),
                            insertSalTable.toStdString());

    }
    QString getVal()
    {
        QString sql_get=QString("select item_qua,item_price,item_type from %1 where item_name='%2'").
                arg(m_sal.m_store,m_sal.m_name);
        return sql_get;
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string addinfo=(char *)arry+
                string("用户名:")+m_username+
                "身份"+m_status+
                string("salItem: 仓库名")+m_sal.m_store+
                string("销售的物品名:")   +m_sal.m_name+
                string("数目")           +to_string(m_sal.m_qua)+
                string("价格")           +to_string(m_sal.m_price);
        addinfo.push_back('\n');
        return addinfo;
    }

public:
    Goods m_sal;
};


class ShowStoreAll:public SendPackImpl
{
public:
    ShowStoreAll ()
        :SendPackImpl(SendPackImpl::showStoreAll)
    {
        m_info_len=sizeof(ShowStoreAll);
    }

    void setStore(string store)
    {
        memcpy(m_store,store.c_str(),store.size());
    }

    string explain()
    {
        QString sql=QString("select * from %1").arg(m_store);
        return sql.toStdString();
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string addinfo=(char *)arry+ string("用户名:")+m_username+
                "身份"+m_status+string("show table:")+m_store;
        addinfo.push_back('\n');
        return addinfo;
    }
public:
    char m_store[20]{0};
};


//补货  从A仓库 拿到B仓库
class Replenish:public SendPackImpl
{
public:
    Replenish (const Goods &from,const Goods &to)
        :SendPackImpl(SendPackImpl::replenish)
    {
        m_from=from;
        m_to=to;
        m_info_len=sizeof(Replenish);
    }
    struct RetSql
    {
        string find_sq;   //查询sql表中是否有该列
        string reduce_sql;//sql1是从要减去的表中减去对应的值
        string insert_sql;//sql2是将对应的值加到目的表上
        string add_sql;   //sql3是将对应的值插入到对应的表中
    };
    RetSql explain()
    {
        string find_sql=QString("select * from store_table where store='%1'").
                arg(QString(m_to.m_store)).toStdString();
        string remove_sql= QString("UPDATE %1 SET item_qua =item_qua-%2 "
                 "WHERE item_name = '%3'").arg(
                     QString(m_from.m_store),
                     QString::number(m_from.m_qua),
                     QString(m_from.m_name)).toStdString();

        string insert_sql=QString("insert into %1(item_name, item_price,item_qua,item_type)"
                                   " values('%2', %3,%4,'%5')").
                arg((char*)m_to.m_store,
                    (char*)m_to.m_name,
                    QString::number(m_to.m_price),
                    QString::number(m_to.m_qua),
                    (char*)m_to.m_type)

                .toStdString();

        string add_sql=QString("UPDATE %1 SET item_qua =item_qua+%2 "
                                "WHERE item_name = '%3'").arg(
                                    QString(m_to.m_store),
                                    QString::number(m_from.m_qua),
                                    QString(m_from.m_name)).toStdString();



        RetSql ret_back;
        ret_back.add_sql=add_sql;
        ret_back.insert_sql=insert_sql;
        ret_back.reduce_sql=remove_sql;
        ret_back.find_sq=find_sql;
        return ret_back;
    }
    string getLogInfo()
    {
        char arry[20];
        memset(arry,0,sizeof(arry));
        gettime_now(arry);

        string addinfo=
                string(arry)+
                string("用户名:")+m_username+
                "身份"+m_status+
                string("Replenish--->")+
                "from:"+m_from.m_store+
                "  to:"+m_to.m_store  +
                " name:"+m_from.m_name+
                " qua :"+to_string( m_from.m_qua);
        addinfo.push_back('\n');
        return addinfo;
    }
public:
    Goods m_from;
    Goods m_to;
};

//查询  查询对应仓库 对应的物品
class Find:public SendPackImpl
{
public:
    Find (const Goods &to)
        :SendPackImpl(SendPackImpl::find)
    {
        m_to=to;
        m_info_len=sizeof(Find);
    }
    string explain()
    {
        QString sql=QString("select *from %1 where item_name='%2'")
                .arg(m_to.m_store , m_to.m_name);
        return sql.toStdString();
    }
    string getLogInfo()
    {
        char arry[20]={0};
        gettime_now(arry);
        string add=string(arry)+
                string("用户名:")+m_username+
                "身份"+m_status+"find:--->"+
                "name:"+m_to.m_name+
                "store"+m_to.m_store;
        add.push_back('\n');
        return add;
    }
public:
    Goods m_to;
};

//管理仓库
class ChargeStore:public SendPackImpl
{
public:
    enum Type
    {
        addStore,deleteStore,findAll,find,findOneStore
      //增加      删除         选择
    };

    ChargeStore (int type)
        :SendPackImpl(SendPackImpl::charge_store)
    {
    }
    ChargeStore():SendPackImpl
                  (SendPackImpl::charge_store)
    {
    }

public:
     int   m_type;
};

class DeleteStore:public ChargeStore
{
public:
    DeleteStore()
    {
        m_type=Type::deleteStore;
        m_info_len=sizeof(DeleteStore);
    }
    void setStore(string store_)
    {
        memcpy(store_name,store_.c_str(),store_.size());
    }
    pair<string,string> explain()
    {
        pair<string,string> delete_store_ret;
        //从仓库表中删除对应的行号
        QString deletesql= QString("DELETE FROM store_table WHERE  store= '%1'")
                            .arg(reinterpret_cast<char*>(store_name));
        //删除对应的表
        QString drop_table=QString("drop table %1;").arg(reinterpret_cast<char*>(store_name));
        delete_store_ret.first=deletesql.toStdString();
        delete_store_ret.second=drop_table.toStdString();
        return delete_store_ret;
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry) +
                string("用户名:")+m_username+
                "身份"+m_status+"delete store:----->"+

                        store_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    char store_name[20];
};

class FindAll:public ChargeStore
{
public:
    FindAll()
    {
        m_type=Type::findAll;
        m_info_len=sizeof(DeleteStore);
    }
    string explain()
    {
        string sql_selectAlltab="select *from store_table";
        return sql_selectAlltab;
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+ string("用户名:")+m_username+
                "身份"+m_status+"findAll store:----->";
        add_info.push_back('\n');
        return add_info;
    }
};

class AddStore:public ChargeStore
{
public:
    AddStore()
    {
        m_type=Type::addStore;
        m_info_len=sizeof(AddStore);
    }
    pair<string,string> explain()
    {
        pair<string,string> mypair;
        QString create_table_sql=QString("create  table  %1(item_name  varchar(20) primary key,"
                                   " item_price int CHECK (item_price>= 0),"
                                   " item_qua   int CHECK (item_qua>= 0),"
                                   " item_type  varchar(20) )").arg(store_name);

        QString insertValue     =QString("insert into store_table(store)"
                                   " values('%1')").arg(store_name);

        mypair.first=create_table_sql.toStdString();
        mypair.second=insertValue.toStdString();
        return mypair;
    }

    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"create store:----->"
                        +store_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    char store_name[20];
};


//销售统计
class SoldCensus:public SendPackImpl
{
public:
    SoldCensus()
        :SendPackImpl(SendPackImpl::soldcensus)
    {
        m_info_len=sizeof(SoldCensus);
    }
    void setData(string data)
    {
        memcpy(arry,data.c_str(),
               data.size());
        return;
    }
    string explain()
    {
        QString sql=QString("select * from SalTable"
                            " where time='%1';").arg(arry);
        return sql.toStdString();
    }

    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"sold   census:----->"
                        +"time:"+arry;
        add_info.push_back('\n');
        return add_info;
    }
public:
    char arry[30]{0};
};

//删除商品
class DeleteGoods:public SendPackImpl
{
public:
    DeleteGoods(const Goods &dele)
        :SendPackImpl(SendPackImpl::deletegoods)
    {
        m_delete=dele;
        m_info_len=sizeof(DeleteGoods);
    }

    QString getVal()
    {
        QString sqlget=QString("select item_price,item_qua from %1 where item_name='%2'")
                       .arg(m_delete.m_store,m_delete.m_name);
        return sqlget;
    }

    pair<string,string> explain(pair<int,int> val)
    {
        QString deletesql= QString("DELETE FROM %1 WHERE  item_name= '%2'")
                            .arg(m_delete.m_store,m_delete.m_name);
        char now_time[20]{0};
        gettime_now(now_time);
        QString insertValue=QString("insert into DelTable(store,item_name,"
                                    "time,price,qua)"
                                    " values('%1','%2','%3','%4','%5')")
                .arg(m_delete.m_store,
                     m_delete.m_name,
                     now_time,
                     QString::number(val.first),
                     QString::number(val.second)
                     );

        return pair<string,string>(
                                    deletesql.toStdString(),
                                    insertValue.toStdString()
                                  );
    }

    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"delete item:----->"
                        +"store:"+m_delete.m_type
                        +"name: "+m_delete.m_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    Goods m_delete;
};

//改变商品
class ChangeGoods:public SendPackImpl
{
public:
    ChangeGoods(const Goods &to,const Goods &from)
        :SendPackImpl(SendPackImpl::changegoods)
    {
        m_to=to;
        m_from=from;
        m_info_len=sizeof(ChangeGoods);
    }

    string explain()
    {
       string sql= QString("UPDATE %1 SET item_name = '%2' ,"
                "item_price =%3,"
                "item_qua=%4,"
                "item_type='%5'"
                "WHERE item_name = '%6'").arg(
                    QString(m_from.m_store),
                    QString(m_to.m_name) ,
                    QString::number(m_to.m_price),
                    QString::number(m_to.m_qua),
                    QString(m_to.m_type),
                    QString(m_from.m_name)).toStdString();
       //cout<<sql<<endl;
       return sql;
    }

    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"ChangeGoods:----->"
                        +"name:"+m_from.m_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    Goods m_from;
    Goods m_to;
};

//分类统计
class ClsGoods:public SendPackImpl
{
public:
    ClsGoods():
        SendPackImpl(SendPackImpl::clsgoods)
    {
        m_info_len=sizeof(ChangeGoods);
    }
    void setStore(string store_)
    {
        memset(store_name,0,20);
        memcpy(store_name,store_.c_str(),store_.size());
    }
    string explain()
    {

        QString sql=QString("select item_type ,count(*)"
                                        "from %1 "
                            "group by %2.item_type").arg(store_name,
                                                         store_name);
        return sql.toStdString();

    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"ClsGoods:----->"
                        +"store:"+store_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    char store_name[20]={0};
};

class AddGoods:public SendPackImpl
{
    //插入新的商品
public:
    AddGoods(const Goods &add)
        :SendPackImpl(SendPackImpl::addgoods)
    {
        m_add=add;
        m_info_len=sizeof(AddGoods);
    }
    string explain()
    {

        QString sql=QString("insert into %1(item_name, item_price,item_qua,item_type)"
                            " values('%2', %3,%4,'%5')").
                arg((char*)m_add.m_store,
                    (char*)m_add.m_name,
                    QString::number(m_add.m_price),
                    QString::number(m_add.m_qua),
                    (char*)m_add.m_type);
        return sql.toStdString();
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"AddGoods:----->"
                        +"store:"+m_add.m_store
                        +"name:"+m_add.m_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    Goods m_add;
};


class Load:public SendPackImpl
{
public:
    //登录
    Load():
        SendPackImpl(SendPackImpl::Load)
    {
         m_info_len=sizeof(Load);
    }
    void setInfo(string count,string password)
    {
        memcpy(info.password,count.c_str(),count.size());
        memcpy(info.user_name,password.c_str(),password.size());
    }
    string explain()
    {
        //user_name, cipher
        QString sql=QString("select * from user where user_name='%1'and "
                            "cipher='%2'").
                arg(info.user_name,info.password);
        return sql.toStdString();
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+"load:----->"
                        +"user:"+info.user_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    UserLoadInfo info;
};


class SolderLoad:public SendPackImpl
{
public:
    //登录
    SolderLoad():
        SendPackImpl(SendPackImpl::solderLoad)
    {
         m_info_len=sizeof(SolderLoad);
    }
    void setInfo(string count,string password)
    {
        memcpy(info.password,count.c_str(),count.size());
        memcpy(info.user_name,password.c_str(),password.size());
    }
    string explain()
    {
        //user_name, cipher
        QString sql=QString("select * from Solder where user_name='%1'and "
                            "cipher='%2'").
                arg(info.user_name,info.password);
        return sql.toStdString();
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"load:----->"
                        +"user:"+info.user_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    UserLoadInfo info;
};

class SolderLogin:public SendPackImpl
{
public:
    //注册
    SolderLogin():
        SendPackImpl(SendPackImpl::solderLogin)
    {
        memset(&info,0,sizeof(info));
        m_info_len=sizeof(Login);
    }

    SolderLogin(string count,string password):
        SendPackImpl(SendPackImpl::Login)
    {
        memset(&info,0,sizeof(info));
        m_info_len=sizeof(Login);
        setInfo(count,password);
    }

    void setInfo(string count,string password)
    {
        memcpy(info.password,count.c_str(),count.size());
        memcpy(info.user_name,password.c_str(),password.size());
    }
public:
    string explain()
    {
        QString sql=QString("insert into Solder(user_name, cipher)"
                    " values('%1', '%2')")
                .arg(info.user_name,info.password);
        return sql.toStdString();
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"Login:----->"
                        +"user:"+info.user_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    UserLoadInfo info;
};


class Login:public SendPackImpl
{
public:
    //注册
    Login():
        SendPackImpl(SendPackImpl::Login)
    {
        memset(&info,0,sizeof(info));
        m_info_len=sizeof(Login);
    }

    Login(string count,string password):
        SendPackImpl(SendPackImpl::Login)
    {
        memset(&info,0,sizeof(info));
        m_info_len=sizeof(Login);
        setInfo(count,password);
    }

    void setInfo(string count,string password)
    {
        memcpy(info.password,count.c_str(),count.size());
        memcpy(info.user_name,password.c_str(),password.size());
    }
public:
    string explain()
    {
        QString sql=QString("insert into user(user_name, cipher)"
                    " values('%1', '%2')")
                .arg(info.user_name,info.password);
        return sql.toStdString();
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"Login:----->"
                        +"user:"+info.user_name;
        add_info.push_back('\n');
        return add_info;
    }
public:
    UserLoadInfo info;
};


class EarlyWarning:public SendPackImpl
{
public:
    EarlyWarning():
        SendPackImpl(SendPackImpl::earlyWarn)
    {

        m_info_len=sizeof(EarlyWarning);
    }

    EarlyWarning(string store,int val):
        SendPackImpl(SendPackImpl::Login)
    {
        m_info_len=sizeof(EarlyWarning);
        setStore(store,val);
    }

    void setStore(string stor,int val)
    {
        memcpy(m_store,stor.c_str(),stor.size());
        m_val=val;
        return;
    }
    string explain()
    {
         QString sqlv=QString("select * from %1  where item_qua<%2")
                       .arg(QString(m_store),QString::number(m_val));
        return sqlv.toStdString();
    }
    string getLogInfo()
    {
        char arry[20]{0};
        gettime_now(arry);
        string add_info=string(arry)+string("用户名:")+m_username+
                "身份"+m_status+"early warning:----->"
                        +"user:"+m_store
                        +"warn_num:"+to_string(m_val);
        add_info.push_back('\n');
        return add_info;
    }

public:
   char m_store[20];
   int  m_val;
};

#endif // SERVICE_H
