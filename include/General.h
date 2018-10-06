/******************************************************/
//-*-coding-utf-8-*-
//File:General.h
//Date:2018-10-01
//Author:Ayden.Shu
//brief:BaseCode General Head file
/******************************************************/
#ifndef __GENERAL__H__
#define __GENERAL__H__

#ifndef NULL
#define NULL ((void *)0)
#endif

#include "sysTool.h"

extern Time DBGTime;

#define LOG_ERR(x) std::cout<<"["<<DBGTime.GetSTime()<<"][ERROR]<"<<__FILE__<<"><"<<__LINE__<<">"<<x<<std::endl;
#define LOG_WARN(x) std::cout<<"["<<DBGTime.GetSTime()<<"][WARN]<"<<__FILE__<<"><"<<__LINE__<<">"<<x<<std::endl;
#define LOG_INFO(x) std::cout<<"["<<DBGTime.GetSTime()<<"][INFO]<"<<__FILE__<<"><"<<__LINE__<<">"<<x<<std::endl;


#endif

