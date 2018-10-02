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
#include "General.h"

#define LOG_ERR(x) std::cout<<"["<<Array<M>::DBGTime.GetSTime()<<"][ERROR]<"<<__FILE__<<"><"<<__LINE__<<">"<<x<<std::endl;
#define LOG_WARN(x) std::cout<<"["<<Array<M>::DBGTime.GetSTime()<<"][WARN]<"<<__FILE__<<"><"<<__LINE__<<">"<<x<<std::endl;
#define LOG_INFO(x) std::cout<<"["<<Array<M>::DBGTime.GetSTime()<<"][INFO]<"<<__FILE__<<"><"<<__LINE__<<">"<<x<<std::endl;


template <class M>
class Array{
    protected:
        static Time DBGTime;
        /*嵌套类，向量*/
        class Vector{ 
            private:
                unsigned int totalline;
                unsigned int totalrow;
                //转置标示
                bool flagT;
                
                //索引行
                unsigned int indexline;
                unsigned int indexrow;
            public:
                //索引目标起始地址 
                M* dataAddr;
                Vector(unsigned int totalLine,unsigned int totalRow,M* addr):dataAddr(addr),
                                                                            totalline(totalLine),
                                                                            totalrow(totalRow),
                                                                            flagT(false)
                {};
                ~Vector(){};
                Vector& operator =(const Vector& V)
                {
                   //not copy address 
                   totalline = V.totalline;
                   totalrow = V.totalrow;
                   flagT = V.flagT;
                   indexline = V.indexline;
                   indexrow = V.indexrow;

                   return *this;
                }
                bool setIndexParam(unsigned int line,bool Tflag)
                {
                    indexline = line;
                    return true;
                }
                M* operator[](unsigned int RowIndex)
                {
                    M* ret = NULL;
                    //Vector存向量列索引
                    indexrow = RowIndex;
                    if(flagT)
                    {
                        //内存行连续存储,转至后行为列,按照索引先切行列找切对应列
                        ret = ((dataAddr+indexrow*totalrow)+indexline);
                    }
                    else
                    {
                        //内存行连续存储,不转至，正常索引，按照索引行，切对应列
                        ret = (dataAddr+indexrow+(indexline*totalrow));
                    }
                }
        };
                
    private:
        /*@line&row*/
        unsigned int Line;
        unsigned int Row;
        /*T flag*/
        bool flagT;
        /*data address*/
        M *address;
        /*data index*/
        Vector Index;
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
                                                                   Row(row),
                                                                   flagT(false),
                                                                   address(NULL),
                                                                   Index(Line,Row,NULL),
                                                                   spaceSize(0)
{
    //非异常抛出new
    if(0 != Line*Row)
    {
        address = new(std::nothrow) M[Line*Row];

        if(NULL == address)
        {
            LOG_ERR("Array alloc sapce failed!!!!");
        }
        else
        {
            int i=0;
            Index.dataAddr = address;
            if(defaultValue)
            {
                for(i=0;i<Line*Row;i++)
                {
                    *(address+i) = defaultValue;
                }
            }
            else
            {
                memset(reinterpret_cast<void *>(address),0,sizeof(M)*Line*Row);
            }
            spaceSize = sizeof(M)*Line*Row;
        }
    }
}

template <class M>
Array<M>::~Array()
{
    if(NULL != address)
    {
        delete[] address;
        address = NULL;
    }

    if(NULL != Index)
    {
        delete[] Index;
        Index = NULL;
    }
}

//same type deep copy
template <class M>
Array<M>::Array(const Array<M>& A):Line(A.Line),
                                   Row(A.row),
                                   flagT(A.flagT),
                                   address(NULL),
                                   Index(Line,Row,NULL),
                                   spaceSize(0)
{
    //deep copy
    if(0 != Line*Row)
    {
        address = new(std::nothrow) M[Line*Row];

        if(NULL == address)
        {
            LOG_ERR("Array alloc sapce failed!!!!");
        }
        else
        {
            Index.dataAddr = address;
            spaceSize = sizeof(M)*Line*Row;
            memcpy(address,A.address,A.spaceSize);
        }
    }
}


//different type deep copy
template <class M>
template <class K> Array<M>::Array(const Array<K>& A):Line(A.Line),
                                                      Row(A.row),
                                                      flagT(A.flagT),
                                                      address(NULL),
                                                      Index(Line,Row,NULL),
                                                      spaceSize(0)
{
    LOG_WARN("Array copy between different type");
    //deep copy
    if(0 != Line*Row)
    {
        address = new(std::nothrow) M[Line*Row];

        if(NULL == address)
        {
            LOG_ERR("Array alloc sapce failed!!!!");
        }
        else
        {
            int i=0,j=0;
            Index.dataAddr = address;
            spaceSize = sizeof(M)*Line*Row;
            //copy value
            for(i=0;i<Line;i++)
            {
                for(j=0;j<Row;j++)
                {
                    *this->operator[](i)[j] = *A[i][j];
                }
            }
            
        }
    }
}

template <class M>
Array<M>& Array<M>::operator=(const Array<M>& A)
{
    //if size is diff ,delete space and apply again
    if(spaceSize != A.spaceSize)
    {
        ~Array<M>();
        //apply again & copy
        if(0 != A.Line*A.Row)
        {
            address = new(std::nothrow) M[Line*Row];

            if(NULL == address)
            {
                LOG_ERR("Array alloc sapce failed!!!!");
            }
            else
            {
                Index.dataAddr = address;
                spaceSize = sizeof(M)*A.Line*A.Row;
                memcpy(address,A.address,A.spaceSize);
            }
        }
        else
        {
            Index.dataAddr = NULL;
            spaceSize = 0;
        }
    }
    else
    {
        //just copy 
        memcpy(address,A.address,A.spaceSize);
    }
    
    Line = A.Line;
    Row = A.Row;
    flagT = A.flagT;
    
    //copy index
    Index = A.Index;    

    return *this;
}

template <class M>
template <class K> Array<M>& Array<M>::operator=(const Array<K>& A)
{
    LOG_WARN("Array = between different type");
    //force apply mem again
    ~Array<M>();
    //apply again & copy
    if(0 != A.Line*A.Row)
    {
        address = new(std::nothrow) K[Line*Row];

        if(NULL == address)
        {
            LOG_ERR("Array alloc sapce failed!!!!");
        }
        else
        {
            Index.dataAddr = address;
            spaceSize = sizeof(K)*A.Line*A.Row;
            memcpy(address,A.address,A.spaceSize);
        }
    }
    else
    {
        Index.dataAddr = NULL;
        spaceSize = 0;
    }

 
    Line = A.Line;
    Row = A.Row;
    flagT = A.flagT;
    
    //copy index
    Index = A.Index;    

    return *this;
}


template <class M>
typename Array<M>::Vector& Array<M>::operator[](unsigned int LineIndex)
{
    Index.setIndexParam(LineIndex,flagT);
    return Index;
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

