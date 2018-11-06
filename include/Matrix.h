/******************************************************/
//-*-coding-utf-8-*-
//File:Matrix.h
//Date:2018-09-11
//Author:Ayden.Shu
//brief: base function about Matrix
/******************************************************/
#ifndef __MATRIX__H_
#define __MATRIX__H_

#include <new>
#include <cmath>
#include <vector>
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>

#include "sysTool.h"
#include "General.h"

template <class M>
class Array{
    protected:
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

                //using for illigal reply
                M Empty;
            public:
                //索引目标起始地址 
                M* dataAddr;
                Vector(unsigned int totalLine,unsigned int totalRow,M* addr):totalline(totalLine),
                                                                             totalrow(totalRow),
                                                                             flagT(false),
                                                                             indexline(0),
                                                                             indexrow(0),
                                                                             Empty(0),
                                                                             dataAddr(addr)
                {
                   //LOG_INFO(totalline<<"--"<<totalrow);
                };
                ~Vector(){};
                bool reset()
                {
                    dataAddr = NULL;
                    flagT = false;
                    totalline = 0;
                    totalrow = 0;
                    indexline = 0;
                    indexrow = 0;
                };
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
                    flagT = Tflag;
                    return true;
                }
                M& operator[](unsigned int RowIndex)
                {
                    if(NULL == dataAddr)
                    {
                        LOG_ERR("Vector's addr is NULL");
                        return Empty; 
                    }
                    //Vector存向量列索引
                    indexrow = RowIndex;
                    if(flagT)
                    {
                        //内存行连续存储,转至后行为列,按照索引先切行列找切对应列
                        M& ret = *((dataAddr+indexrow*totalrow)+indexline);
                        return ret;
                    }
                    //内存行连续存储,不转至，正常索引，按照索引行，切对应列
                    M& ret = *(dataAddr+indexrow+(indexline*totalrow));
                    return ret;
                }
                unsigned int getUnitNum() const
                {
                    if(flagT)
                    {
                        return totalline;
                    }
                    return totalrow;
                }
                //返回当前行的向量
                Array<M> V()
                {
                    if(NULL == dataAddr)
                    {
                        LOG_ERR("Vector's addr is NULL");
                    }
                    if(flagT)
                    {
                        //返回转至后的行，也就是1*line
                        Array<M> tempArray(1,totalline,0);
                        int i =0; 
                        for(i=0;i<totalline;i++)
                        {
                            //LOG_INFO((*this)[i]);
                            tempArray[0][i] = (*this)[i];
                        }
                        return tempArray;
                    }
                    //返回行，也就是1*row
                    Array<M> tempArray(1,totalrow,0);
                    int i =0; 
                    for(i=0;i<totalrow;i++)
                    {
                        //LOG_INFO((*this)[i]);
                        tempArray[0][i] = (*this)[i];
                    }
                    return tempArray;
                }
        };
                
    private:
        /*static line&row can not using straight*/
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
        Array(unsigned int line,unsigned int row,M defaultValue=0);
        ~Array();
        //get info
        unsigned int getLine() const {if(flagT){return Row;} return Line;};
        unsigned int getRow() const {if(flagT){return Line;} return Row;};
        //const M* getAddr const {return address;};
        //unsigned long getSize() const {return spaceSize;};
        //bool getflagT() const {return flagT;};
        //Vector getIndex() const {return Index;};
   
        //copy function
        Array(const Array<M>& A);
        //change function
        Array(Array<M>::Vector& V);
        //change function
        Array(unsigned int line,unsigned int row,M Unit[],unsigned int num);

        template <class K> Array(const Array<K>& A);

        //set value function
        Array<M>& operator=(const Array<M>& A);
        Array<M>& operator=(const Array<M>::Vector& V);

        //暂时不实现不同类型转换，暂时没意义且破坏封装性
        //template <class K> Array<M>& operator=(const Array<K>& A);

        Vector& operator[](unsigned int index);

        //matrix function
        template <class K> Array<M> operator*(K Value);
        template<class K> Array<M> operator*(Array<K>&& Value);
        template<class K> Array<M> operator*(Array<K>& Value);
        //rValue quote reuse
        template<class K,class J> friend Array<J> operator*(const K Value,Array<J>& A);
        //rValue quote reuse
        template<class K,class J> friend Array<J> operator*(const K Value,Array<J>&& A);

        template<class K> Array<M> operator+(K Value);
        template<class K> Array<M> operator+(Array<K>&& Value);
        template<class K> Array<M> operator+(Array<K>& Value);
        template<class K,class J> friend Array<J> operator+(K Value,Array<J>& A);
        template<class K,class J> friend Array<J> operator+(K Value,Array<J>&& A);
            
        template<class K> Array<M> operator-(K Value);
        template<class K> Array<M> operator-(Array<K>&& Value);
        template<class K> Array<M> operator-(Array<K>& Value);
        template<class K,class J> friend Array<J> operator-(K Value,Array<J>& A);
        template<class K,class J> friend Array<J> operator-(K Value,Array<J>&& A);

        template <class K> Array<M> operator/(K Value);
        template<class K> Array<M> operator/(Array<K>&& Value);
        template<class K> Array<M> operator/(Array<K>& Value);
        template<class K,class J> friend Array<J> operator/(K Value,Array<J>& A);
        template<class K,class J> friend Array<J> operator/(K Value,Array<J>&& A);

        Array<M> operator-();
        Array<M> exp();
        M squareSum();
        Array<M>& T();
        Array<M>& random(M min,M max);
        static Array<M> dot(Array<M>& left,Array<M>& right);

        Array<M>& diag(const M Value);
        //矩阵打印
        void show();
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
Array<M>::Array(unsigned int line,unsigned int row,M Unit[],unsigned int num):Line(line),
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
            for(i=0;i<Line*Row;i++)
            {
                if(i<num)
                {
                    *(address+i) = Unit[i];
                }
                else
                {
                    *(address+i) = 0;
                }
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
#if 0
        char addr[32];
        sprintf(addr,"%p",this);
        LOG_INFO("delete Array "<<addr<<" with Line["<<getLine()<<"]Row["<<getRow()<<"]Size["<<spaceSize<<"]");
#endif
        delete[] address;
        address = NULL;
    }
}

//same type deep copy
template <class M>
Array<M>::Array(const Array<M>& A):Line(A.Line),
                                   Row(A.Row),
                                   flagT(A.flagT),
                                   address(NULL),
                                   Index(A.Line,A.Row,NULL),
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

template<class M>
Array<M>::Array(Array<M>::Vector& V):Line(1),
                                    Row(V.getUnitNum()),
                                    flagT(false),
                                    address(NULL),
                                    Index(this->Line,this->Row,NULL),
                                    spaceSize(0)

{   
    //LOG_INFO("Line : "<<Line<<"Row: "<<Row);
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
            for(int i=0;i<V.getUnitNum();i++)
            {
                (*this)[0][i] = V[i];
            }
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
                    (*this)[i][j] = A[i][j];
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
        this->~Array();
        //apply again & copy
        if(0 != A.Line*A.Row)
        {
            address = new(std::nothrow) M[A.Line*A.Row];
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

//using Vector for Array
template<class M>
Array<M>& Array<M>::operator=(const Array<M>::Vector& V)
{
    //if size is diff ,delete space and apply again
    Array<M> A = V.V();
    if(spaceSize != A.spaceSize)
    {
        this->~Array();
        //apply again & copy
        if(0 != A.Line*A.Row)
        {
            address = new(std::nothrow) M[A.Line*A.Row];
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

//暂时不实现不同类型转换，暂时没意义且破坏封装性
/*
template <class M>
template <class K> Array<M>& Array<M>::operator=(const Array<K>& A)
{
    LOG_WARN("Array = between different type");
    //force apply mem again
    this->~Array();

    Line = A.getLine();
    Row = A.getRow();
    //apply again & copy
    if(0 != A.getLine()*A.getRow())
    {
        address = reinterpret_cast<M*>(new(std::nothrow) K[Line*Row]);

        if(NULL == address)
        {
            LOG_ERR("Array alloc sapce failed!!!!");
        }
        else
        {
            Index.dataAddr = address;
            spaceSize = sizeof(K)*A.getLine()*A.getRow();
            memcpy(address,A.address,A.getSize());
        }
    }
    else
    {
        Index.dataAddr = NULL;
        spaceSize = 0;
    }

    flagT = A.getflagT();
    
    //copy index
    Index = A.getIndex();

    return *this;
}
*/

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
    return *this;
}

template <class M>
Array<M>& Array<M>::random(M min,M max)
{
    for(int i=0;i<getLine();i++)
    {
        for(int j=0;j<getRow();j++)
        {
            (*this)[i][j]=((double)rand()/RAND_MAX)*(max-min) + min;
        }
    }
    return *this;
}

template <class M>
Array<M> Array<M>::dot(Array<M>& left,Array<M>& right)
{
    int i = left.getLine();
    int j = right.getRow();
    Array<M> tempArray(i,j,0);

    //矩阵点乘法
    if(left.getRow() != right.getLine())
    {
        LOG_ERR("Array dot illigal,left row["<<left.getRow()<<"] is not same with right line["<<right.getLine()<<"]");
        return tempArray;
    }
    //same result
    for(j=0;j<right.getRow();j++)
    {
        for(i=0;i<left.getLine();i++)
        {
            int k = 0;
            for(k=0;k<right.getLine();k++)
            {
                //新列向量为之前列向量乘转移矩阵的
                tempArray[i][j] += left[i][k] * right[k][j];
            }
        }
    }
    return tempArray;
}

template <class M>
Array<M>& Array<M>::diag(const M Value)
{
    //矩阵对角化
    if(getLine() != getRow())
    {
       LOG_ERR("Array is not a square matrix!");
       return *this;
    }
    else
    {
        int i=0;
        memset(address,0,spaceSize);
        for(i=0;i<getLine();i++)
        {
            (*this)[i][i] = Value;
        }
    }
    return *this;
}


//数乘
template <class M>
template <class K> Array<M> Array<M>::operator*(const K Value)
{
    int i = 0;
    int j = 0;
    if(NULL == address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<M> tempArray(getLine(),getRow(),0);
    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =(*this)[i][j] * Value;
        }
    }
    return tempArray;
}

template<class M> 
template<class K> Array<M> Array<M>::operator*(Array<K>&& Value)
{
    int i = 0;
    int j = 0;

    if(NULL == Value.address||Value.getRow() != getRow() || Value.getLine() != getLine())
    {
        LOG_ERR("data address is NULL or input not with same row&line");
        Array<M> tempArray(0,0);
        return tempArray;
    }

    Array<M> tempArray(getLine(),getRow(),0);

    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =Value[i][j]*(*this)[i][j];
        }
    }
    return tempArray;
}

template<class M> 
template<class K> Array<M> Array<M>::operator*(Array<K>& Value)
{
    int i = 0;
    int j = 0;

    if(NULL == Value.address||Value.getRow() != getRow() || Value.getLine() != getLine())
    {
        LOG_ERR("data address is NULL or input not with same row&line");
        Array<M> tempArray(0,0);
        return tempArray;
    }

    Array<M> tempArray(getLine(),getRow(),0);

    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =Value[i][j]*(*this)[i][j];
        }
    }
    return tempArray;
}


template<class K,class J>
Array<J> operator*(const K Value,Array<J>&& A)
{
    /*
    LOG_INFO("==============rValue==============");
    A.show();
    LOG_INFO("==================================");
    */
    return A*(1.0*Value);    
}

template<class K,class J>
Array<J> operator*(const K Value,Array<J>& A)
{
    /*
    LOG_INFO("==============lValue==============");
    A.show();
    LOG_INFO("==================================");
    */
    return A*(1.0*Value);    
}

template <class M>
template <class K> Array<M> Array<M>::operator+(K Value)
{
    int i = 0;
    int j = 0;
    if(NULL == address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<M> tempArray(getLine(),getRow(),0);
    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =(*this)[i][j] + Value;
        }
    }
    return tempArray;
}

template<class M> 
template<class K> Array<M> Array<M>::operator+(Array<K>&& Value)
{
    int i = 0;
    int j = 0;

    if(NULL == Value.address||Value.getRow() != getRow() || Value.getLine() != getLine())
    {
        LOG_ERR("data address is NULL or input not with same row&line");
        Array<M> tempArray(0,0);
        return tempArray;
    }

    Array<M> tempArray(getLine(),getRow(),0);

    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =Value[i][j]+(*this)[i][j];
        }
    }
    return tempArray;
}

template<class M> 
template<class K> Array<M> Array<M>::operator+(Array<K>& Value)
{
    int i = 0;
    int j = 0;

    if(NULL == Value.address||Value.getRow() != getRow() || Value.getLine() != getLine())
    {
        LOG_ERR("data address is NULL or input not with same row&line");
        Array<M> tempArray(0,0);
        return tempArray;
    }

    Array<M> tempArray(getLine(),getRow(),0);

    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =Value[i][j]+(*this)[i][j];
        }
    }
    return tempArray;
}

template<class K,class J>
Array<J> operator+(const K Value,Array<J>& A)
{
    return A+(1.0*Value);    
}

template<class K,class J>
Array<J> operator+(const K Value,Array<J>&& A)
{
    return A+(1.0*Value);    
}

template <class M>
template <class K> Array<M> Array<M>::operator-(K Value)
{
    int i = 0;
    int j = 0;
    if(NULL == address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<M> tempArray(getLine(),getRow(),0);
    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =(*this)[i][j] - Value;
        }
    }
    return tempArray;
}

template<class K,class J>
Array<J> operator-(const K Value,Array<J>& A)
{
    int i = 0;
    int j = 0;
    if(NULL == A.address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<J> tempArray(A.getLine(),A.getRow(),0);
    for(i=0;i<A.getLine();i++)
    {
        for(j=0;j<A.getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =Value-A[i][j];
        }
    }
    return tempArray;
}

template<class M> 
template<class K> Array<M> Array<M>::operator-(Array<K>&& Value)
{
    int i = 0;
    int j = 0;

    if(NULL == Value.address||Value.getRow() != getRow() || Value.getLine() != getLine())
    {
        LOG_ERR("data address is NULL or input not with same row&line");
        Array<M> tempArray(0,0);
        return tempArray;
    }

    Array<M> tempArray(getLine(),getRow(),0);

    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =Value[i][j]-(*this)[i][j];
        }
    }
    return tempArray;
}

template<class M> 
template<class K> Array<M> Array<M>::operator-(Array<K>& Value)
{
    int i = 0;
    int j = 0;

    if(NULL == Value.address||Value.getRow() != getRow() || Value.getLine() != getLine())
    {
        LOG_ERR("data address is NULL or input not with same row&line");
        Array<M> tempArray(0,0);
        return tempArray;
    }

    Array<M> tempArray(getLine(),getRow(),0);

    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =Value[i][j]-(*this)[i][j];
        }
    }
    return tempArray;
}



template<class K,class J>
Array<J> operator-(const K Value,Array<J>&& A)
{
    int i = 0;
    int j = 0;
    if(NULL == A.address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<J> tempArray(A.getLine(),A.getRow(),0);
    for(i=0;i<A.getLine();i++)
    {
        for(j=0;j<A.getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =Value-A[i][j];
        }
    }
    return tempArray;
}

template <class M>
template <class K> Array<M> Array<M>::operator/(K Value)
{
    int i = 0;
    int j = 0;
    if(NULL == address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<M> tempArray(getLine(),getRow(),0);
    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" * "<< Value);
            tempArray[i][j] =(*this)[i][j] / Value;
        }
    }
    return tempArray;
}

template<class M> 
template<class K> Array<M> Array<M>::operator/(Array<K>&& Value)
{
    int i = 0;
    int j = 0;

    if(NULL == Value.address||Value.getRow() != getRow() || Value.getLine() != getLine())
    {
        LOG_ERR("data address is NULL or input not with same row&line");
        Array<M> tempArray(0,0);
        return tempArray;
    }

    Array<M> tempArray(getLine(),getRow(),0);

    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
             if(0 != (*this)[i][j])
            {
                tempArray[i][j] =Value[i][j]/(*this)[i][j];
            }
            else
            {
                LOG_ERR("right Array 0 found ["<<i<<"]["<<j<<"]");
                tempArray[i][j] = 0;
            }
        }
    }
    return tempArray;
}

template<class M> 
template<class K> Array<M> Array<M>::operator/(Array<K>& Value)
{
    int i = 0;
    int j = 0;

    if(NULL == Value.address||Value.getRow() != getRow() || Value.getLine() != getLine())
    {
        LOG_ERR("data address is NULL or input not with same row&line");
        Array<M> tempArray(0,0);
        return tempArray;
    }

    Array<M> tempArray(getLine(),getRow(),0);

    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            if(0 != (*this)[i][j])
            {
                tempArray[i][j] =Value[i][j]/(*this)[i][j];
            }
            else
            {
                LOG_ERR("right Array 0 found ["<<i<<"]["<<j<<"]");
                tempArray[i][j] = 0;
            }
        }
    }
    return tempArray;
}


template<class K,class J>
Array<J> operator/(const K Value,Array<J>& A)
{
    int i = 0;
    int j = 0;
    if(NULL == A.address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<J> tempArray(A.getLine(),A.getRow(),0);
    for(i=0;i<A.getLine();i++)
    {
        for(j=0;j<A.getRow();j++)
        {
            if(0 != A[i][j])
            {
                tempArray[i][j] =Value/A[i][j];
            }
            else
            {
                LOG_ERR("right Array 0 found ["<<i<<"]["<<j<<"]");
                tempArray[i][j] = 0;
            }
        }
    }
    return tempArray;
}

template<class K,class J>
Array<J> operator/(const K Value,Array<J>&& A)
{
    int i = 0;
    int j = 0;
    if(NULL == A.address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<J> tempArray(A.getLine(),A.getRow(),0);
    for(i=0;i<A.getLine();i++)
    {
        for(j=0;j<A.getRow();j++)
        {
            if(0 != A[i][j])
            {
                tempArray[i][j] =Value/A[i][j];
            }
            else
            {
                LOG_ERR("right Array 0 found ["<<i<<"]["<<j<<"]");
                tempArray[i][j] = 0;
            }
        }
    }
    return tempArray;
}

template <class M>
Array<M> Array<M>::operator-()
{
    int i = 0;
    int j = 0;
    if(NULL == address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<M> tempArray(getLine(),getRow(),0);
    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            tempArray[i][j] =-(*this)[i][j];
        }
    }
    return tempArray;
}

template<class M>
Array<M> Array<M>::exp()
{
    int i =0;
    int j =0;
    if(NULL == address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<M> tempArray(getLine(),getRow(),0);
    for(i=0;i<getLine();i++)
    {
        for(j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" exp "<< Value);
            tempArray[i][j] =std::exp((*this)[i][j]);
        }
    }
    return tempArray;
}

template<class M>
M Array<M>::squareSum()
{
    M sum = 0;
    if(NULL == address)
    {
        LOG_ERR("data address is NULL");
    }
    Array<M> tempArray(getLine(),getRow(),0);
    for(int i=0;i<getLine();i++)
    {
        for(int j=0;j<getRow();j++)
        {
            //LOG_INFO((*this)[i][j]<<" exp "<< Value);
            sum += (*this)[i][j]*(*this)[i][j];
        }
    }
    return sum;
}

template<class M>
void Array<M>::show()
{
    int i=0,j=0;

    for(i=0;i<getLine();i++)
    {
        printf("[");
        for(j=0;j<getRow();j++)
        {
            std::cout<<(*this)[i][j]<<" ";
        }
        std::cout<<"]"<<std::endl;
    }
    return;
}
#endif
