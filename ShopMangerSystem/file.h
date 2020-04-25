#ifndef FILE_H
#define FILE_H
#include"head.h"
#include<iostream>
using namespace std;

class File
{
public:
    File(string path);
    int Read(char *info,int len);
    int Write(char *info,int len);
    ~File();
private:
    bool magic;
    int fd;
};

void gettime_now(char *cur_time);

#endif // FILE_H
