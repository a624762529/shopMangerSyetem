#pragma once
#include "select.h"
#include <iostream>
#include <QString>
#include "shopinfosql.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <iomanip>
using namespace std;
int main()
{
    Select sel(8888);
    sel.loop();
    return 1;

}


//测试与数据库的通讯
int main234qwwe56()
{
    //增加用户表
    ShopActSql mylit("./info.db");

    string createTable="create table user(user_name varchar(20) primary key,"
                             " cipher varchar(20))";


    if(!mylit.doSql(createTable))
    {
        cout<<"create error"<<endl;
    }


    string insertValue="insert into user(user_name, cipher)"
                       " values('123', '123')";
    if(!mylit.doSql(insertValue))
    {
        cout<<"insert error"<<endl;
    }
    QString sql=QString("select * from user where user_name='%1'and cipher='%2'").
                                                            arg("1239999","123999");
    string showtab="select * from user;";

    if(!mylit.selectDB(showtab))
    {
        cout<<"show error"<<endl;
    }

    return 1;
}

////建立商品表
int m123456ain()
{

    ShopActSql mylit("./info.db");
    //建表通过测试
    {
    string createTable="create  table  goodsItem(item_name  varchar(20) primary key,"
                                           " item_price int CHECK (item_price>0),"
                                           " item_qua   int CHECK (item_qua>0),"
                                           " item_type  varchar(20) )";
    if(!mylit.doSql(createTable))
    {
        cout<<"create error"<<endl;
    }

    //    int pre=clock();
    //    //插入数据
    //    //for(int i=0;i<100;i++)
    //        {
                string insertValue="insert into goodsItem("
                                   "item_name, item_price,item_qua,item_type)"
                                   " values('刀子', 13,200,'货物')";
                if(!mylit.doSql(insertValue))
                {
                    cout<<"insert error"<<endl;
                }

                  insertValue="insert into goodsItem("
                                   "item_name, item_price,item_qua,item_type)"
                                   " values('刀子1', 13,200,'货物')";
                if(!mylit.doSql(insertValue))
                {
                    cout<<"insert error"<<endl;
                }

                  insertValue="insert into goodsItem("
                                   "item_name, item_price,item_qua,item_type)"
                                   " values('刀子2', 13,200,'货物')";
                if(!mylit.doSql(insertValue))
                {
                    cout<<"insert error"<<endl;
                }

    //        }


    //    cout<<"access time"<<clock()-pre<<endl;

    //    QString deletesql= QString("DELETE FROM %1 WHERE  item_name= '%2'")
    //                        .arg("goodsItem","刀子");
    //    cout<<deletesql.toStdString()<<endl;
    //    string changeValue="UPDATE goodsItem set item_price=item_price-3";
    //    if(!mylit.doSql(deletesql.toStdString()))
    //    {
    //        cout<<"change error"<<endl;
    //    }
    }
    string sql="select count(*),item_type from goodsItem group by goodsItem.item_type";
    string showtab="select * from goodsItem;";


    string sqlv="select * from Item2  where item_qua<300";
    string sqlg="select  * from goodsItem  ";
    if(!mylit.selectDB(sqlg))
    {
        cout<<"show error"<<endl;
    }

    //删除表
    string drop_tab="drop table goodsItem;";
    if(!mylit.doSql(drop_tab) )
    {
        cout<<"drop error"<<endl;
    }
    return 1;
}

////建立商品类型表
int main23123()
{
    ShopActSql mylit("./info.db");
    //建表通过测试
    //建立物品表
    string createTable="create  table  goodstype_table(goodstype"
                       " varchar(20) primary key)";

    if(!mylit.doSql(createTable))
    {
        cout<<"create error"<<endl;
    }

    int pre=clock();
    //插入数据
    //for(int i=0;i<100;i++)
    {
        string insertValue="insert into goodstype_table(goodstype)"
                           " values('213')";
        if(!mylit.doSql(insertValue))
        {
            cout<<"insert error"<<endl;
        }
    }


    cout<<"access time"<<clock()-pre<<endl;
    //sleep(1);

    string changeValue="UPDATE goodstype_table set goodstype='3' ";
    if(!mylit.doSql(changeValue))
    {
        cout<<"change error"<<endl;
    }

    string showtab="select * from goodstype_table;";
    if(!mylit.selectDB(showtab))
    {
        cout<<"show error"<<endl;
    }


    //删除表
//    string drop_tab="drop table goodstype_table;";
//    if(!mylit.selectDB(drop_tab))
//    {
//        cout<<"drop error"<<endl;
//    }
    return 1;
}

//建立仓库表
int mai3245n()
{
    ShopActSql mylit("./info.db");
    //建表通过测试
    //建立物品表
        string createTable="create  table  store_table(store"
                           " varchar(20) primary key)";

        if(!mylit.doSql(createTable))
        {
            cout<<"create error"<<endl;
        }

    //    int pre=clock();
    //    //插入数据
    //    //for(int i=0;i<100;i++)
    //    {
    //        string insertValue="insert into store_table(store)"
    //                           " values('goods')";
    //        if(!mylit.doSql(insertValue))
    //        {
    //            cout<<"insert error"<<endl;
    //        }
    //    }


    //    cout<<"access time"<<clock()-pre<<endl;
    //sleep(1);

    //    string changeValue="UPDATE store_table set store='3' ";
    //    if(!mylit.doSql(changeValue))
    //    {
    //        cout<<"change error"<<endl;
    //    }

    string showtab="select * from store_table;";
    if(!mylit.selectDB(showtab))
    {
        cout<<"show error"<<endl;
    }
//    string del_sql= "DELETE  from store_table WHERE  store= 'goods'";
//    if(!mylit.doSql(del_sql))
//    {
//        cout<<"delete error"<<endl;
//    }


//    if(!mylit.selectDB(showtab))
//    {
//        cout<<"show error"<<endl;
//    }
    //删除表
//    string drop_tab="drop table store_table;";
//    if(!mylit.selectDB(drop_tab))
//    {
//        cout<<"drop error"<<endl;
//    }

//    string showDesc="desc tab";
//    if(!mylit.doSql(showDesc))
//    {
//        cout<<"delete error"<<endl;
//    }

    return 1;
}



////建立销售表
int main2134()
{
    ShopActSql mylit("./info.db");
    string createTable="create table SalTable(store     varchar(20) ,"
                                             "item_name varchar(20) ,"
                                             "time      varchar(20) ,"
                                             "price     int,"
                                             "qua       int)";
    if(!mylit.doSql(createTable))
    {
        cout<<"create error"<<endl;
    }

    int pre=clock();
    char time[20]{0};
    gettime_now(time);
    QString insertValue=QString("insert into SalTable(store,item_name,"
                                "time,price,qua)"
                       " values('%1','%2','%3','%4','%5')")
                        .arg("goodsItem","刀子",time,QString::number(39),
                             QString::number(20)
                             );
    if(!mylit.doSql(insertValue.toStdString()))
    {
        cout<<"insert error"<<endl;
    }

{
        string showtab="select * from SalTable ;";
        if(!mylit.selectDB(showtab))
        {
            cout<<"show error"<<endl;
        }

}

    string showtab="select * from SalTable where time='2020-03-27 ';";
    if(!mylit.selectDB(showtab))
    {
        cout<<"show error"<<endl;
    }

    string drop_tab="drop table SalTable;";
    if(!mylit.doSql(drop_tab))
    {
        cout<<"drop error"<<endl;
    }


    return 1;
}




