#ifndef GOOD_H
#define GOOD_H
#include<iostream>
#include<cstring>
#include<string>
using namespace std;

struct Goods
{
public:
    Goods(){};
    Goods(string name,int price,int qua,string store,string type)
    {
        memcpy(m_name,name.c_str(),name.size());
        memcpy(m_store,store.c_str(),store.size());
        memcpy(m_type,type.c_str(),type.size());

        m_qua=qua;
        m_price=price;
    }

    void setGoods(string name,int price,int qua,string store,string type)
    {
        memcpy(m_name,name.c_str(),name.size());
        memcpy(m_store,store.c_str(),store.size());
        memcpy(m_type,type.c_str(),type.size());

        m_qua=qua;
        m_price=price;
    }


    ~Goods(){}
public:
    char m_name[20];     //商品的名字
    int  m_price;        //商品的价格
    int  m_qua;          //商品的数目
    char  m_store[20];   //所处的仓库
    char  m_type[20];   //商品的类型
};
struct UserLoadInfo
{
    char user_name[20];
    char password[20];
};


#endif // GOOD_H
