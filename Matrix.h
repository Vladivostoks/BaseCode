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
        Array(int num,int unitSize);
        ~Array();
        Array(Array const& A);
        Array& operator=(Array const & A);

         




};


#endif







