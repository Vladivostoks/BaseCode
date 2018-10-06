/******************************************************/
//-*-coding-utf-8-*-
//File:sysTool.h
//Date:2018-09-11
//Author:Ayden.Shu
//brief:system tool function head file
/******************************************************/
#ifndef __SYSTOOL_H__
#define __SYSTOOL_H__

#include <string>
#include <time.h>
#include <sys/time.h>

class Time{
    private:
        struct timeval iTime;
        std::string sTime;
        bool freshTime();
    public:
        Time();
        ~Time();
        int GetITime(); 
        unsigned long GetITime(std::string StrTime); 
        std::string GetSTime();
        std::string GetSTime(int iTimeSec);
};



#endif
