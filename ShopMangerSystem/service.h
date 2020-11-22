#ifndef SERVICE_H
#define SERVICE_H
#include"good.h"

#include<iostream>
using namespace std;
extern string user_name;
extern string status;
struct SendBack
{
    int m_len=0;  //信息长度
    int m_num=0;  //数据的数目
    bool m_tag=0; //sql语句是否执行成功
    char m_ch=0;  //数据内容
};

struct SendBackz
{
    int m_len;  //信息长度
    int m_num;  //数据的数目
    bool m_tag; //sql语句是否执行成功
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
      //删除商品      改变商品     分类统计  增加商品   登录   注册  销售商品 预警
        showStoreAll,solderLoad,solderLogin
      //展示全部的商品  销售员登录   销售员注册   预警
    };
    SendPackImpl()
    {
        m_send_type=-1;
        m_info_len=-1;
        memcpy(m_username,user_name.c_str(),user_name.size());
        memcpy(m_status,status.c_str(),status.size());
    }
    SendPackImpl(int type)
    {
        m_send_type=type;
        m_info_len=-1;
        memcpy(m_username,user_name.c_str(),user_name.size());
        memcpy(m_status,status.c_str(),status.size());
    }
public:
    char m_username[20]{0};
    char m_status[20]{0};
    int m_send_type;//发送类型
    int m_info_len; //信息的长度
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
public:
    char m_store[20]{0};
};

class SalItem:public SendPackImpl
{
public:
    SalItem (const Goods &from )
        :SendPackImpl(SendPackImpl::salItem)
    {
        memset(&m_sal,0,sizeof(m_sal));
        memcpy(&m_sal,&from,sizeof(Goods));
        m_info_len=sizeof(SalItem);
    }
private:
    Goods m_sal;
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
private:
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
private:
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

        m_info_len=sizeof(ChargeStore);
        m_type=type;
    }
    ChargeStore():SendPackImpl
                  (SendPackImpl::charge_store)
    {
        m_info_len=sizeof(ChargeStore);
    }
public:
     //Goods m_to;
     int   m_type;
};

class AddStore:public ChargeStore
{
public:
    AddStore()
    {
        m_type=Type::addStore;

        m_info_len=sizeof(AddStore);
    }
    void setStore(string store_)
    {
        memset(store_name,0,sizeof(store_name));
        memcpy(store_name,store_.c_str(),store_.size());
    }
public:
    char store_name[20]={0};
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
        memset(store_name,0,20);
        memcpy(store_name,store_.c_str(),store_.size());
    }
public:
    char store_name[20]={0};
};

class FindAll:public ChargeStore
{
public:
    FindAll()
    {
        m_type=Type::findAll;
        m_info_len=sizeof(FindAll);
    }
};

class FindOneStore:public ChargeStore
{
public:
    FindOneStore(){}
    void setStore(string store_)
    {
        memset(store_name,0,20);
        memcpy(store_name,store_.c_str(),store_.size());
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
private:
    Goods m_delete;
};

//改变商品
class ChangeGoods:public SendPackImpl
{
public:
    ChangeGoods(const Goods &from,const Goods &to)
        :SendPackImpl(SendPackImpl::changegoods)
    {
        m_to=to;
        m_from=from;
        m_info_len=sizeof(ChangeGoods);
    }
    ChangeGoods(){}
private:
    Goods m_from;
    Goods m_to;
};

//对仓库中的商品分类统计
class ClsGoods:public SendPackImpl
{
public:
    ClsGoods():
        SendPackImpl(SendPackImpl::clsgoods)
    {
        m_info_len=sizeof(ClsGoods);
    }
    void setStore(string store_)
    {
        memset(store_name,0,20);
        memcpy(store_name,store_.c_str(),store_.size());
    }
public:
    char store_name[20]={0};
};

class AddGoods:public SendPackImpl
{
public:
    AddGoods(const Goods &add)
        :SendPackImpl(SendPackImpl::addgoods)
    {
        m_add=add;
        m_info_len=sizeof(AddGoods);
    }
private:
    Goods m_add;
};


class Load:public SendPackImpl
{
public:
    Load():
        SendPackImpl(SendPackImpl::Load)
    {
         m_info_len=sizeof(Load);
         memset(&info,0,sizeof(info));
    }
    void setInfo(string count,string password)
    {
        memcpy(info.user_name,count.c_str(),count.size());
        memcpy(info.password,password.c_str(),password.size());
    }
private:
    UserLoadInfo info;
};

class Login:public SendPackImpl
{
public:
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
        memcpy(info.user_name,count.c_str(),count.size());
        memcpy(info.password,password.c_str(),password.size());
    }
private:
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
private:
    char m_store[20]{0};
    int  m_val;
};



class SolderLoad:public SendPackImpl
{
public:
    //登录
    SolderLoad():
        SendPackImpl(SendPackImpl::solderLoad)
    {
         m_info_len=sizeof(SolderLoad);
         memset(&info,0,sizeof(info));
    }
    void setInfo(string count,string password)
    {
        memset(&info,0,sizeof(info));
        memcpy(info.password,count.c_str(),count.size());
        memcpy(info.user_name,password.c_str(),password.size());
        m_info_len=sizeof(SolderLoad);
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
        m_info_len=sizeof(SolderLogin);
    }

    SolderLogin(string count,string password):
        SendPackImpl(SendPackImpl::solderLogin)
    {
        memset(&info,0,sizeof(info));
        m_info_len=sizeof(SolderLogin);
        setInfo(count,password);
    }

    void setInfo(string count,string password)
    {
        memset(&info,0,sizeof(info));
        memcpy(info.user_name,count.c_str(),count.size());
        memcpy(info.password,password.c_str(),password.size());
    }
public:
    UserLoadInfo info;
};



#endif // SERVICE_H
