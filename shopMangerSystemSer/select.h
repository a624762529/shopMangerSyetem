#ifndef EPOLLLOOP_H
#define EPOLLLOOP_H
#pragma once
#include"head.h"
#include<set>
#include<map>
#include<string>
#include"service.h"
#include<iostream>
#include"explainsql.h"
#include"heartalarm.h"

using namespace std;
#define PERALARM 20
struct SockArry
{
public:
    SockArry(){}
    SockArry(SendBack *back,int send_len)
    {
        m_back=back;
        m_send_len=send_len;
    }
    void setInfo(SendBack *back,int send_len)
    {
        m_back=back;
        m_send_len=send_len;
    }
public:
    SendBack *m_back=nullptr;
    int       m_send_len=0;
};

class Select
{
public:
    Select(int port);
    ~Select();
    void loop();
private:
    int createSock(int prot);
    int acceptCallBack();
    void readCallBack(int i);
    void writeCallBack(int i);
    void expCallBack(int i);
private:
    void doWrite(SendBack* tag,int cfd);
    void changeFdWrite(int cfd);
    void changeFdRead (int cfd);
    void doAlarm();
private:
    void spy();
    void startSpy();
private:
    void removeFd(int cfd);
private:
    atomic<int>         num;
    HeartAlarm  m_alarm;
    ExplainSql_ m_expl;
    int lfd;              //监听套接字
    sockaddr_in addr;     //地址
    fd_set m_read;        //读集
    fd_set m_write;       //写集
    fd_set m_exp;         //异常集合
    map<int,SockArry> mp; //存放读出写入信息的集合
    map<int,HeartNode> mp_heart;
};
#endif // EPOLLLOOP_H
