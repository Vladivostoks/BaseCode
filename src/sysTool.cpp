/******************************************************/
//-*-coding-utf-8-*-
//File:sysTool.cpp
//Date:2018-09-11
//Author:Ayden.Shu
//brief:system tool function
/******************************************************/

//System Time tool
#include "sysTool.h"

Time::Time():sTime("")
{
    memset(&iTime,0,sizeof(struct timeval));
}

Time::~Time()
{
}

bool Time::freshTime()
{
    char temp[64]={0};
    struct tm info;

    memset(&iTime,0,sizeof(struct timeval));
    gettimeofday(&iTime,NULL);

    memcpy(&info,gmtime(&iTime.tv_sec),sizeof(struct tm));

    sprintf(temp,"%04d-%02d-%02dT%02d:%02d:%02d.%03d",info.tm_year+1900,
                                                info.tm_mon,
                                                info.tm_mday,
                                                info.tm_hour,
                                                info.tm_min,
                                                info.tm_sec,
                                                iTime.tv_usec/1000);
    //exchange to string
    sTime = temp;
    return true;
}
int Time::GetITime()
{
    freshTime();
    return iTime.tv_sec*1000+iTime.tv_usec/1000;
}
/*返回为毫秒*/
unsigned long Time::GetITime(std::string StrTime)
{
    struct tm info;
    timeval temp;
    memset(&info,0,sizeof(struct tm));
    //微秒存储毫秒值
    sscanf(StrTime.c_str(),"%04d-%02d-%02dT%02d:%02d:%02d.%03d",&info.tm_year,
                                                            &info.tm_mon,
                                                            &info.tm_mday,
                                                            &info.tm_hour,
                                                            &info.tm_min,
                                                            &info.tm_sec,
                                                            &temp.tv_usec);
    temp.tv_sec = mktime(&info);
    
    return (temp.tv_sec*1000+temp.tv_usec/1000);
}

std::string Time::GetSTime()
{
    freshTime();
    return sTime;
}

std::string Time::GetSTime(int iTimeSec)
{
    std::string stemp;
    char ctemp[64]={0};
    struct tm info;
    timeval tempStamp;
    memset(&tempStamp,0,sizeof(timeval));

    memcpy(&info,gmtime(&tempStamp.tv_sec),sizeof(struct tm));

    sprintf(ctemp,"%04d-%02d-%02dT%02d:%02d:%02d.%03d",info.tm_year+1900,
                                                info.tm_mon,
                                                info.tm_mday,
                                                info.tm_hour,
                                                info.tm_min,
                                                info.tm_sec,
                                                iTime.tv_usec/1000);
    stemp = ctemp;
    return stemp;
}



























