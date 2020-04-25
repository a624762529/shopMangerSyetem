#include "file.h"

File::File(string path)
{
    magic=true;
    fd=open(path.c_str(),O_RDWR|O_CREAT,0777);
    if(fd<0)
    {
         magic=false;
         perror("open file error");
         throw bad_alloc();
    }
}


int File::Read(char *info,int len)
{
    if(magic)
    {
        return read(fd,info,len);
    }
    return -1;
}

int File::Write(char *info,int len)
{
    if(magic)
    {
        return write(fd,info,len);
    }
    return -1;
}

File::~File()
{
    close(fd);
    magic=false;
}

void gettime_now(char *cur_time)
{
    char Year[6] = {0};
    char Month[4] = {0};
    char Day[4] = {0};
    time_t current_time;
    struct tm* now_time;
    time(&current_time);
    now_time = localtime(&current_time);
    strftime(Year, sizeof(Year), "%Y-", now_time);
    strftime(Month, sizeof(Month), "%m-", now_time);
    strftime(Day, sizeof(Day), "%d ", now_time);
    strncat(cur_time, Year, 5);
    strncat(cur_time, Month, 3);
    strncat(cur_time, Day, 3);
}
