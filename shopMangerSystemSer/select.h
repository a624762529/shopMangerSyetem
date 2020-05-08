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
#include <string.h>
#include <stdio.h>
#include <memory.h>
using namespace std;
#define PERALARM 400
struct SockArry
{
public:
    SockArry(){}

    SockArry(SendBack *back,int send_len)
    {
        m_back    =back;
        m_send_len=send_len;
    }

    void setInfo(SendBack *back,int send_len)
    {
        m_back=back;
        m_send_len=send_len;
    }

    struct recvBuf
    {
    public:
        char      m_getfromfd[1024]{0};
        int       m_getlen=0;
        int       m_alllen=0;
        bool      m_issetlen=false;
    public:
        void addInfo(char *buf,int len,int all_len)
        {
            memcpy(&m_getfromfd[m_getlen],buf,len);
            m_getlen+=len;

        }

        void setAllLen()
        {
            if(m_issetlen==false)
            {
                SendPackImpl *type=reinterpret_cast<SendPackImpl *>(m_getfromfd);
                m_issetlen=true;
                m_alllen=type->m_info_len;
            }
        }

        int getReadLen()
        {
            //讲获取的报文长度返回出去
            return m_getlen;
        }

        bool jude_readOver()
        {
            return m_alllen==m_getlen;
        }

        void clear()
        {
            memset(m_getfromfd,0,sizeof(m_getfromfd));
            m_getlen=0;
            m_alllen=0;
            m_issetlen=false;
        }

        char* getInfo()
        {
            pair<char*,int> ret;
            char *info=(char*)malloc(m_alllen);
            memcpy(info,m_getfromfd,m_getlen);
            return info;
        }
    };
    ~SockArry()
    {
        m_back=nullptr;
        m_send_len=0;
        m_have_sendlen=0;
        memset(&m_buf,0,sizeof(m_buf));
    }
public:
    SendBack *m_back=nullptr;
    int       m_send_len=0;
    int       m_have_sendlen=0;
    recvBuf   m_buf;
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
