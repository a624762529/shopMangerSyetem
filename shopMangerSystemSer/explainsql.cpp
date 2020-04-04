#include "explainsql.h"

ExplainSql_::ExplainSql_():m_db("./info.db"),log("./log")
{

}

SendBack* ExplainSql_::chargeStore(char *msg)
{
    ChargeStore *cgs=reinterpret_cast<ChargeStore*>(msg);

    switch (cgs->m_type)
    {
    case ChargeStore::findAll:
    {
        //查询全部的信息
        FindAll *fd=reinterpret_cast<FindAll *>(msg);
        log.addinfo(fd->getLogInfo());
        return m_db.getAllStore(fd->explain(),1);
        break;
    }
    case ChargeStore::deleteStore:
    {
        DeleteStore *dl=reinterpret_cast<DeleteStore *>(msg);
        log.addinfo(dl->getLogInfo());
        SendBack*back= getSendBack();
        back->m_tag=false;
        auto i=dl->explain();
        if(m_db.doSql(i.first))
        {
            if(m_db.doSql(i.second))
            {
                back->m_tag=true;
                return back;
            }
        }
        return back;
        break;
    }
    case ChargeStore::addStore:
    {
        AddStore *add_s=reinterpret_cast<AddStore *>(msg);
        auto i=add_s->explain();
        log.addinfo(add_s->getLogInfo());
        SendBack*back= getSendBack();
        back->m_tag=false;

        if(m_db.doSql(i.first))
        {
            if(m_db.doSql(i.second))
            {
                back->m_tag=true;
                return back;
            }
        }
        return back;
        break;
    }
    default:
        break;
    }
    return NULL;
}

SendBack* ExplainSql_::explain(char *msg,int len)
{
    char ch='\n';
    SendPackImpl *type=reinterpret_cast<SendPackImpl *>(msg);
    switch (type->m_send_type)
    {
    case SendPackImpl::showStoreAll:
    {
        ShowStoreAll *sho=reinterpret_cast<ShowStoreAll *>(msg);

        log.addinfo(sho->getLogInfo());
        return m_db.selectDB(sho->explain(),5);
        break;
    }
    case SendPackImpl::salItem:
    {
        //商品的销售
        SendBack* ret1=getSendBack();
        SalItem *item=reinterpret_cast<SalItem*>(msg);
        log.addinfo(item->getLogInfo());
        auto ret=item->explain();
        log.addinfo(item->getLogInfo());
        if(m_db.doSql(ret.first))
        {
            if(m_db.doSql(ret.second))
            {
                ret1->m_tag=true;
            }
        }
        return ret1;
        break;
    }
    case SendPackImpl::addgoods:
    {
        //添加商品
        AddGoods *add=reinterpret_cast<AddGoods*>(msg);
        bool ret=m_db.doSql(add->explain());
        SendBack* ret_back=getSendBack();
        ret_back->m_tag=ret;
        log.addinfo(add->getLogInfo());
        return ret_back;
        break;
    }

    case SendPackImpl::changegoods:
    {
        //改变商品
        ChangeGoods *chg=reinterpret_cast<ChangeGoods*>(msg);
        bool ret=m_db.doSql(chg->explain());
        SendBack* ret_back=getSendBack();
        ret_back->m_tag=ret;
        log.addinfo(chg->getLogInfo());
        return ret_back;
        break;
    }

    case SendPackImpl::charge_store:
    {
        return chargeStore(msg);
        break;
    }

    case SendPackImpl::replenish:
    {
        //调货商品
        Replenish *rep=reinterpret_cast<Replenish*>(msg);
        auto ret=rep->explain();
        SendBack* ret_back=getSendBack();
        log.addinfo(rep->getLogInfo());
        //如果商品表中有该表
        if(m_db.selectDB( ret.find_sq))
        {
            if(m_db.doSql( ret.reduce_sql))
            {
                if(m_db.doSql( ret.insert_sql))
                {
                    ret_back->m_tag=true;
                    return ret_back;
                }
                if(m_db.doSql( ret.add_sql))
                {
                    ret_back->m_tag=true;
                    return ret_back;
                }
            }
        }
        return ret_back;
        break;
    }

    case SendPackImpl::clsgoods:
    {
        //分类统计商品
        ClsGoods *cls=reinterpret_cast<ClsGoods *>(msg);
        log.addinfo(cls->getLogInfo());
        return m_db.clsGoods(cls->explain(),2);
        break;
    }

    case SendPackImpl::deletegoods:
    {
        //删除商品
        DeleteGoods *del=reinterpret_cast<DeleteGoods*>(msg);
        log.addinfo(del->getLogInfo());
        bool ret=m_db.doSql(del->explain());
        SendBack* ret_back=getSendBack();
        ret_back->m_tag=ret;
        return ret_back;
        break;
    }

    case SendPackImpl::find:
    {
        //查找商品
        Find *fd=reinterpret_cast<Find*>(msg);
        log.addinfo(fd->getLogInfo());
        return m_db.searchItem(fd->explain(),4);
        break;
    }
    case SendPackImpl::Load:
    {
        Load *load=reinterpret_cast<Load*>(msg);
        SendBack* ret_back=getSendBack();
        log.addinfo(load->getLogInfo());
        bool ret=m_db.selectDB(load->explain());
        ret_back->m_tag=ret;
        return ret_back;
        break;
    }
    case SendPackImpl::Login:
    {
        Login *logv=reinterpret_cast<Login*>(msg);
        SendBack* ret_back=getSendBack();
        log.addinfo(logv->getLogInfo());
        bool ret=m_db.doSql(logv->explain());
        ret_back->m_tag=ret;
        return ret_back;
        break;
    }
    case SendPackImpl::soldcensus:
    {
        SoldCensus *sol=reinterpret_cast<SoldCensus*>(msg);
        log.addinfo(sol->getLogInfo());
        return m_db.soldCensus(sol->explain(),5);
        break;
    }
    case SendPackImpl::earlyWarn:
    {
        EarlyWarning *earlywarn=reinterpret_cast<EarlyWarning *>(msg);
        log.addinfo(earlywarn->getLogInfo());
        return m_db.earlyWarning(earlywarn->explain(),4);
    }
    }
    return  NULL;
}

SendBack* ExplainSql_::getSendBack()
{
    SendBack* back=reinterpret_cast<SendBack*>(malloc(sizeof(SendBack)));
    back->m_len=sizeof(SendBack);
    back->m_num=0;
    back->m_tag=false;

    return back;
}


ExplainSql_:: ~ExplainSql_()
{
    log.stop();
}
