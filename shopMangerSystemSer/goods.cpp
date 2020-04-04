#include "goods.h"


Goods::Goods(string name,int price,int qua,string store, string type)
{
    memcpy(m_name,name.c_str(),name.size());
    memcpy(m_type,type.c_str(),type.size());
    memcpy(m_store,store.c_str(),store.size());
    m_price=price;
    m_qua=qua;
}

Goods::Goods()
{}

Goods::Goods(const Goods &good)
{
    memcpy(this,&good,sizeof(good));
}

void Goods::operator =(const Goods &good)
{
     memcpy((char*)good.m_name,(char*)m_name,sizeof(m_name));
     memcpy((char*)m_type,good.m_type,sizeof(good.m_type));
     memcpy((char*)m_store,good.m_store,sizeof(good.m_store));
     m_qua=good.m_qua;
     m_price=good.m_price;
}

void Goods::setInfo(string name,int price,int qua,string store,string type)
{
    memcpy((char*)this->m_name,name.c_str(),name.size());
    memcpy((char*)m_store,store.c_str(),store.size());
    memcpy((char*)m_type,type.c_str(),type.size());
    m_qua=qua;
    m_price=price;
}
