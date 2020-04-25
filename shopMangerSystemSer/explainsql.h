#pragma once
#ifndef EXPLAINSQL_H
#define EXPLAINSQL_H

#include"logpferi.h"
#include"service.h"
#include<set>
#include"shopinfosql.h"

class ExplainSql_
{
public:
    ExplainSql_();
    ~ExplainSql_();
    SendBack* explain(char *msg,int len);
private:
    SendBack* getSendBack();
    SendBack* chargeStore(char *msg);
private:
    ShopActSql m_db;
    logpferI   log;
};

#endif // EXPLAINSQL_H
