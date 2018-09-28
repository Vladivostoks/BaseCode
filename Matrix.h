/******************************************************/
//-*-coding-utf-8-*-
//File:Matrix.h
//Date:2018-09-11
//Author:Ayden.Shu
//brief:Matrix head file inlucde Matrix operation
/******************************************************/
#ifndef __MATRIX__H__
#define __MATRIX__H__
template <class M>
class Array{
    private:
        /*@line&row*/
        unsigned int line;
        unsigned int row;
        /*data address*/
        char *address;
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
#endif







