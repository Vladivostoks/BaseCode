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

typedef struct
{
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];   
}MD5_CTX;
void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
void MD5Final(MD5_CTX *context,unsigned char digest[16]);
void MD5Transform(unsigned int state[4],unsigned char block[64]);
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);

#endif

