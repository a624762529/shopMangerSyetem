#ifndef STOREANDTYPE_H
#define STOREANDTYPE_H

#include<vector>
#include<string>
#include<iostream>
#include"shopinfosql.h"
using namespace std;

class StoreAndType
{
public:
    StoreAndType();
public:
    vector<string> m_store;
    vector<string> m_type;
    shared_ptr<ShopActSql> do_read;
};

#endif // STOREANDTYPE_H
