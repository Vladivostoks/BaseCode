/******************************************************/
//-*-coding-utf-8-*-
//File:demo.cpp
//Date:2018-09-11
//Author:Ayden.Shu
//brief:Unit Test
/******************************************************/

#include <cstdio>

#include "Matrix.h"
#include "Ann.h"
#include "General.h"


enum Type{
    ArrayTest=0,
    FCLNetTest,
    MAXType
};


bool FCLNetTestFunc();
bool ArrayTestFunc();

int main(int argc,char* argv[])
{
    int type = 0;
    if(argc < 2)
    {
        do{
            printf("Illigal Test Type,please choose a unit of test:\n");
            printf("                                0 Array Test\n");
            printf("                                1 FCLNet Test\n");
            std::cin>>type;
        }while(type>MAXType || type < 0);
    }
    else
    {
        type = atoi(argv[1]);
    }
    switch(type)
    {
        case ArrayTest:{
            ArrayTestFunc();
            break;
        }

        case FCLNetTest:{
            FCLNetTestFunc();
            break;
        }

        default:{
            LOG_INFO("Please choose a test type");
        }
    }


    LOG_INFO("Unit["<<type<<"] Test Over!");
}


bool ArrayTestFunc()
{
    Array<double> A(3,3,3.24);

    double unit[3] = {1.34,2.45,3.155};
    Array<double> B(1,3,unit,3);
   
    //Test 1 diag Matrix;
    std::cout<<"========Test 1 diag Matrix================"<<std::endl;
    Array<double> G = A;
    G.show();
    G.diag(4);
    std::cout<<"===================================="<<std::endl;
    G.show();

    std::cout<<"========Test 2 T=========================="<<std::endl;
    B.show();
    //Array<double> D=B[0].V();
    Array<double> D=B[0];
    std::cout<<"========Vector 1=================="<<std::endl;
    D.show();
    B.T(); 
    std::cout<<"===================================="<<std::endl;
    B.show();
    Array<double> F=B[0].V();
    //Array<double> F=B[0];
    std::cout<<"========Vector 1=================="<<std::endl;
    F.show();

    std::cout<<"========Test 3 Dot Multiplication========="<<std::endl;
    Array<double> C=Array<double>::dot(A,B);
    C.show(); 

    /*
    std::cout<<"========Test 4 Type Change================"<<std::endl;
    Array<int> E(3,3,0);
    A[1][3]=2.452;
    A[2][2]=3.441;
    A.show();
    std::cout<<"===================================="<<std::endl;
    E = A;
    E.show();
    */

    std::cout<<"========Test 5 Value Multiplicaiton======="<<std::endl;
    A.show();
    std::cout<<"===================================="<<std::endl;
    A = A*50;
    A.show();
    std::cout<<"===================================="<<std::endl;
    A = 1.0/10*A; 
    A.show();
    std::cout<<"====================END==================="<<std::endl;
    
    
    return true;
}

bool FCLNetTestFunc()
{
    return true;
}















