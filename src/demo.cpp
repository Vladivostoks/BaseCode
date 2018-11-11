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
    Array<double> D=A[0];
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
    A = 0.1*A; 
    A.show();
    A = 4.3123*(A*1.0);
    A.show();
    std::cout<<"===================================="<<std::endl;
    Array<double>H = A.exp();
    A = 0.1*H; 
    H.show();
    std::cout<<"====================END==================="<<std::endl;
    
    std::cout<<"========Test 6 overload model function  Test======="<<std::endl;
    //is using  Array<M> Array<M>::operator*(Array<K>&& Value)
    Array<double> I = (1-H)*(H-1); 
    I.show();
    std::cout<<"====================END==================="<<std::endl;
    std::cout<<"========Test 7 assigment function  Test======="<<std::endl;
    Array<double> J(3,2,1.5);
    //Array<double> K(4,6,12);
    //Array<double> L(4,6,12);
    for(int i =0;i<12;i++)
    {
        Array<double> T(i+2,i+4,2.5);
        J = T;
        std::cout<<"===================================="<<std::endl;
    }
    std::cout<<"====================END==================="<<std::endl;
    return true;
}
//训练次数
#define TRAIN   500000
bool FCLNetTestFunc()
{
    /*做一个简单异或运算网络的demo*/
    FCLNet<double> testNet;
    
    double resultA[]={0};
    double inputA[]={0,0};

    double resultB[]={1};
    double inputB[]={0,1};

    double resultC[]={1};
    double inputC[]={1,0};

    double resultD[]={0};
    double inputD[]={1,1};

    Array<double>Input[]={Array<double>(1,2,inputA,2),
                          Array<double>(1,2,inputB,2),
                          Array<double>(1,2,inputC,2),
                          Array<double>(1,2,inputD,2)};

    Array<double>Result[]={Array<double>(1,1,resultA,1),
                           Array<double>(1,1,resultB,1),
                           Array<double>(1,1,resultC,1),
                           Array<double>(1,1,resultD,1)};
    std::cout<<"=================INPUT==================="<<std::endl;
    testNet.Addlayer(new NolinearUnit<double,2,3,4>(new SigmoidActivator<double>()));
    testNet.Addlayer(new NolinearUnit<double,3,1,4>(new SigmoidActivator<double>()));
    
    std::cout<<"=================TRAIN==================="<<std::endl;
    SquareLoss<double> lossFun;
    //Stochastic gradient 随机梯度 
    for(int i=0;i<TRAIN;i++)
    {
        LOG_INFO("["<<i<<"]["<<i*1.0/TRAIN*100<<"%]Loss Value "<<
            testNet.train(Input,Result,lossFun,4));
        
    }
    std::cout<<"==================RESULT=================="<<std::endl;
    for(int i=0;i<4;i++)
    {
        testNet.run(Input[i]).show();
    }
    std::cout<<"================END==================="<<std::endl;
    return true;
}















