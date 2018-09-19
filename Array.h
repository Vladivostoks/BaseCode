/******************************************************/
//-*-coding-utf-8-*-
//File:Array.h                                        
//Date:2018-09-11
//Author:Ayden.Shu                                    
//brief: function about Array operation               
/******************************************************/

#ifndef __ARRAY_H__
#define __ARRAY_H__

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
