#ifndef GOOD_H
#define GOOD_H
#pragma once
#include<iostream>
#include<cstring>
using namespace std;

class Goods
{
public:
    Goods();
    Goods(string name,int price,int qua,string store,string type);
    Goods(const Goods &good);
    void operator =(const Goods &good);
    void setInfo(string name,int price,int qua,string store,string type);
public:
    char m_name[20];    //商品的名字
    int  m_price;       //商品的价格
    int  m_qua;         //商品的数目
    char m_store[20];   //所处的仓库
    char m_type[20];    //商品的类型
};

#endif // GOOD_H
