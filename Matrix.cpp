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
    protected:
        /*嵌套类，向量*/
        class Vector{ 
            private:
                M* indexaddr;
            public:
                Vector(){};
                ~Vector(){};
                M operator[](unsigned int index);
        };
    private:
        /*@line&row*/
        unsigned int Line;
        unsigned int Row;
        /*T flag*/
        bool flagT;
        /*data address*/
        M *address;
        Vector* IndexVector;
        /*apply space size*/
        unsigned long spaceSize;
    public:
        Array(unsigned int line,unsigned int row,M defaultValue);
        ~Array();
        //copy function
        Array(const Array<M>& A);
        template <class K> Array(const Array<K>& A);
        //set value function
        Array<M>& operator=(const Array<M>& A);
        template <class K> Array<M>& operator=(const Array<K>& A);

        //matrix function
        Array<M>& T();
        static Array<M> dot(const Array<M>& left,const Array<M>& right);
        Vector& operator[](unsigned int index);

};

template <class M>
Array<M>::Array(unsigned int line,unsigned int row,M defaultValue):Line(line),
                                                                   Row(line),
                                                                   flagT(false),
                                                                   address(NULL),
                                                                   IndexVector(NULL),
                                                                   spaceSize(0)
{
    //非异常抛出new
    address = new(std::nothrow) M[Line*Row];
    //申请行列里最大的索引指针
    IndexVector = new(std::nothrow) Vector<M>[Line>Row?Line:Row];

    if(NULL == address || NULL == IndexVector)
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
        spaceSize = sizeof(M)*Line*Row;
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

//same type deep copy
template <class M>
Array<M>::Array(const Array<M>& A)
{
    //deep copy
    //step 1 compare address size
}


//different type deep copy
template <class M>
template <class K> Array<M>::Array(const Array<K>& A)
{

    
}

template <class M>
Array<M>& Array<M>::operator=(const Array<M>& A)
{
}


template <class M>
Array<M>& Array<M>::T()
{
    flagT = !flagT;

}

template <class M>
Array<M> Array<M>::dot(const Array<M>& left,const Array<M>& right)
{
}

template <class M>
typename Array<M>::Vector& Array<M>::operator[](unsigned int index)
{
    Vector* tempIndexVector = IndexVector+index;
    return *tempIndexVector;
}
