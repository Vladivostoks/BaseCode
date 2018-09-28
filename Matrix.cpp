/******************************************************/
//-*-coding-utf-8-*-
//File:Matrix.cpp
//Date:2018-09-11
//Author:Ayden.Shu
//brief: base function about Matrix
/******************************************************/
#include <new>
#include <cmath>
#include <vector>
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>

#include "sysTool.h"

static Time DBGTime;
#define LOG_ERR(x) std::cout<<"["<<DBGTime.GetSTime()<<"][ERROR]<"<<__FILE__<<"><"<<__LINE__<<">"<<x<<std::endl;
#define LOG_WARN(x) std::cout<<"["<<DBGTime.GetSTime()<<"][WARN]<"<<__FILE__<<"><"<<__LINE__<<">"<<x<<std::endl;
#define LOG_INFO(x) std::cout<<"["<<DBGTime.GetSTime()<<"][INFO]<"<<__FILE__<<"><"<<__LINE__<<">"<<x<<std::endl;


template <class M>
class Array{
    private:
        /*@line&row*/
        unsigned int Line;
        unsigned int Row;
        /*转置标记位*/
        /*data address*/
        M *address;
    public:
        Array(unsigned int line,unsigned int row,M defaultValue);
        ~Array();
        Array(const Array<M>& A);
        //矩阵操作方法
        Array<M>& operator=(const Array<M>& A);
        Array<M>& T(Array& ArraySelf);

        //矩阵点乘
        static Array<M> dot(const Array<M>& left,const Array<M>& right);
};

template <class M>
Array<M>::Array(unsigned int line,unsigned int row,M defaultValue):Line(line),
                                                                   Row(line)
{
    //非异常抛出new
    address = new(std::nothrow) M[Line*Row];
    if(NULL == address)
    {
        LOG_ERR("Array alloc sapce failed!!!!");
    }
    else
    {
        int i=0;
        if(defaultValue)
        {
            for(i=0;i<Line*Row;i++)
            {
                address = defaultValue;
            }
        }
        else
        {
            memset(reinterpret_cast<void *>(address),0,sizeof(M)*Line*Row);
        }
    }
}

template <class M>
Array<M>::~Array()
{
    if(NULL != address)
    {
        delete[] address;
    }
}


template <class M>
Array<M>::Array(const Array<M>& A)
{
}

template <class M>
Array<M>& Array<M>::operator=(const Array<M>& A)
{
}


template <class M>
Array<M>& Array<M>::T(Array& ArraySelf)
{
}

template <class M>
Array<M> Array<M>::dot(const Array<M>& left,const Array<M>& right)
{
}

