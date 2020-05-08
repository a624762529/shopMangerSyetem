#include "shopinfosql.h"

ShopActSql::ShopActSql(string db_name)
{
    m_db_act = QSqlDatabase::addDatabase("QSQLITE");
    m_db_act.setDatabaseName(db_name.c_str() );
    //打开数据库
    if( !m_db_act.open() ) //数据库打开失败
    {
        cout<<"open error"<<endl;
    }
}

bool ShopActSql::doSql(string sql_lag)
{
    QSqlQuery query;
    bool ret= query.exec(sql_lag.c_str());
    return ret;

}

ShopActSql::~ShopActSql()
{
    m_dbname.clear();
    m_db_act.close();
}

bool ShopActSql::selectDB(string sql_lag)
{
    bool ret_tag=false;
    //逐行取出
    QSqlQuery query1;
    query1.exec(sql_lag.c_str());
    while(query1.next()) //一行一行遍历
    {
        //取出当前行的内容
        ret_tag=true;
    }
    return ret_tag;
}

int ShopActSql::getLen(string sql_lag)
{
    //获取到数据之后
    QSqlQuery query;
    bool ret= query.exec(sql_lag.c_str());
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(query);
    int nRecordCount = queryModel->rowCount();
    return nRecordCount;
}

//获取仓库中全部的商品
SendBack* ShopActSql::selectDB(string sql_lag,int arg)
{
    SendBack* ret_type=nullptr;
    //获取到对应的列数
    int lines=getLen(sql_lag);
    int allSize=sizeof(SendBack)+lines*sizeof(Goods);

    ret_type=reinterpret_cast<SendBack*>(malloc(allSize));
    memset(ret_type,0,allSize);

    ret_type->m_len=allSize;
    ret_type->m_num=lines;
    Goods *goods=reinterpret_cast<Goods *>(&ret_type->m_ch);
    QSqlQuery query;
    if(query.exec(sql_lag.c_str()))
    {
        while(query.next())
        {
            string name=query.value(0).toString().toStdString();
            int    price    =query.value(1).toInt();
            int    qua      =query.value(2).toInt();
            string item_type=query.value(3).toString().toStdString();
            goods->setInfo(name,price,qua,"",item_type);
            goods++;
            ret_type->m_tag=true;
        }
    }
    return ret_type;
}

vector<string> ShopActSql::readType()
{
    QString sql="select *from goodstype_table";
    QSqlQuery query;
    bool ret= query.exec(sql.toStdString().c_str());
    if(ret)
    {
        vector<string> ret_back;
        while(query.next()) //一行一行遍历
        {
            //取出当前行的内容
             string i=(query.value(0).toString().toStdString());
             ret_back.push_back(i);
        }
        return ret_back;
    }
}

vector<string> ShopActSql::readStore()
{
    QString sql="select *from store_table";
    QSqlQuery query;
    bool ret= query.exec(sql.toStdString().c_str());
    if(ret)
    {
        vector<string> ret_back;
        while(query.next()) //一行一行遍历
        {
            //取出当前行的内容
             ret_back.push_back(query.value(0).toString().toStdString());
        }
        return ret_back;
    }
}

void ShopActSql::sendBackType()
{}

void ShopActSql::sendBackStore()
{}

Tg ShopActSql::getVal(QString sql)
{
    Tg tg;
    QSqlQuery query;
    if(query.exec(sql))
    {
        while(query.next())
        {
            tg.qua=query.value(0).
                    toString().toInt();
            tg.price =query.value(1).
                    toString().toInt();
            string type=query.value(2).
                    toString().toStdString();
            memcpy( tg.type,type.c_str(),type.size());
            return tg;
        }
    }
    return tg;
}

//查找对应的物品
SendBack* ShopActSql::searchItem(string sql_lag,int arg)
{ 
    SendBack* ret_type=nullptr;
    int allSize=sizeof(SendBack)+1*sizeof(Goods);
    ret_type=reinterpret_cast<SendBack*>(malloc(allSize));
    memset(ret_type,0,allSize);
    ret_type->m_len=allSize;
    ret_type->m_num=1;
    Goods *goods=reinterpret_cast<Goods *>(&ret_type->m_ch);

    QSqlQuery query;
    if(query.exec(sql_lag.c_str()))
    {
        while(query.next())
        {
            string name=query.value(0).toString().toStdString();
            int    price    =query.value(1).toInt();
            int    qua      =query.value(2).toInt();
            string item_type=query.value(3).toString().toStdString();
            goods->setInfo(name,price,qua,"",item_type);
            goods++;
            ret_type->m_tag=true;
        }
    }
    return ret_type;
}

//获取全部的仓库
SendBack* ShopActSql::getAllStore(string sql_lag,int arg)
{
    int all_line=getLen(sql_lag);
    int allSize=sizeof(SendBack)+all_line*sizeof(Store);
    SendBack* ret_type=reinterpret_cast<SendBack*>(malloc(allSize));
    memset(ret_type,0,allSize);

    ret_type->m_len=allSize;     //数据长度
    ret_type->m_num=all_line;    //数据行数

    Store *stor=reinterpret_cast<Store *>(&ret_type->m_ch);
    QSqlQuery query;
    if(query.exec(sql_lag.c_str()))
    {
        while(query.next())
        {
            string getstore= query.value("store").toString().toStdString();
            stor->setStore(getstore);
            stor++;
            ret_type->m_tag=true;
        }
    }
    return ret_type;
}

//分类统计
SendBack* ShopActSql::clsGoods(string sql_lag,int arg)
{
    int info_qua=getLen(sql_lag);
    int size= info_qua*sizeof(RetCls)+sizeof(SendBack);
    SendBack *back=reinterpret_cast< SendBack *> (malloc(size));
    memset(back,0,size);
    back->m_len=size;
    back->m_tag=false;
    back->m_num=info_qua;

    RetCls* cls= reinterpret_cast<RetCls*>(&back->m_ch);
    QSqlQuery query;
    if(query.exec(sql_lag.c_str()))
    {
        while(query.next()) //一行一行遍历
        {
            QString type=query.value(0).toString();
            int     qua=query.value(1).toInt();
            cls->setType(type.toStdString(),qua);
            cls++;
            back->m_tag=true;
        }
    }
    return back;
}

//销售统计
SendBack* ShopActSql::soldCensus(string sql_lag,int arg)
{
    //初始化数据
    int cll_len=getLen(sql_lag);
    int size=cll_len*sizeof(Goods)+sizeof(SendBack);
    SendBack *back= reinterpret_cast<SendBack *>(malloc(size));
    memset(back,0,size);
    back->m_len=size;
    back->m_num=cll_len;
    back->m_tag=false;
    Goods *goods=reinterpret_cast<Goods *>(&back->m_ch);

    QSqlQuery query;
    if(query.exec(sql_lag.c_str()))
    {
        while(query.next()) //一行一行遍历
        {
            string store    =query.value("store").toString().toStdString();
            string item_name=query.value("item_name").toString().toStdString();
            string time     =query.value("time").toString().toStdString();
            int    price    =query.value("price").toInt();
            int    qua      =query.value("qua").toInt();
            goods->setInfo(item_name,price,qua,store,"");
            goods++;
            back->m_tag=true;
        }
    }
    return back;
}


SendBack* ShopActSql::earlyWarning(string sql_lag,int arg)
{
    int cll_len=getLen(sql_lag);
    int size=cll_len*sizeof(Goods)+sizeof(SendBack);
    SendBack *back= reinterpret_cast<SendBack *>(malloc(size));

    memset(back,0,size);

    back->m_len=size;
    back->m_num=cll_len;
    back->m_tag=false;
    Goods *goods=reinterpret_cast<Goods *>(&back->m_ch);

    QSqlQuery query;
    if(query.exec(sql_lag.c_str()))
    {
        while(query.next()) //一行一行遍历
        {
            string name=query.value(0).toString().toStdString();
            int    price    =query.value(1).toInt();
            int    qua      =query.value(2).toInt();
            string item_type=query.value(3).toString().toStdString();
            goods->setInfo(name,price,qua,"",item_type);
            goods++;
            back->m_tag=true;
        }
    }
    return back;
}


QString ShopActSql::Transform(QString info)
{
    int get_size=info.size();

    string n_sp((20-2*get_size),' ');
    //cout<<n_sp<<info.toStdString()<<endl;
    return (n_sp.c_str()+info);
}


