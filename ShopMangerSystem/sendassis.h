#ifndef SENDASSIS_H
#define SENDASSIS_H

//发送一次回话
#include"head.h"

#include"sockclient.h"
#include"service.h"
#include<iostream>
#include<memory>

using namespace std;

class SendAssis
{
public:
    SendAssis();
private:
    SendPackImpl *imp;
    shared_ptr<SockClient> client;
};

#endif // SENDASSIS_H
