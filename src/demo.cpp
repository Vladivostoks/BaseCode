/******************************************************/
//-*-coding-utf-8-*-
//File:demo.cpp
//Date:2018-09-11
//Author:Ayden.Shu
//brief:Unit Test
/******************************************************/

#include <cstdio>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "Matrix.h"
#include "Ann.h"
#include "General.h"

//训练次数
#define TRAIN   100000
//导出的预设内存大小
#define OUTMEMLEN 1024
//保存输出参数文件名称
#define PARAMFILE "./var/paramfile.bin"

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
    
    std::cout<<"========Test 6 LEFT&RIGHT FUN  Test======="<<std::endl;
    //is using  Array<M> Array<M>::operator*(Array<K>&& Value)
    Array<double> I = (1-H)*(H-1); 
    I.show();
    std::cout<<"====================END==================="<<std::endl;
    std::cout<<"======Test 7 assigment function  Test====="<<std::endl;
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

bool FCLNetTestFunc()
{
    /*做一个简单异或运算网络的demo,测试非线性拟合能力*/
    FCLNet<double> testNet;
    
    /*构建输入样本*/
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
    /*构建网络结构*/
    testNet.Addlayer(new NolinearUnit<double,2,3,4>(new SigmoidActivator<double>()));
    testNet.Addlayer(new NolinearUnit<double,3,1,4>(new SigmoidActivator<double>()));
    
    std::cout<<"=================TRAIN==================="<<std::endl;
    SquareLoss<double> lossFun;
    unsigned int startTime = DBGTime.GetITime();
    /*Stochastic gradient 随机梯度*/
    for(int i=0;i<TRAIN;i++)
    {
        LOG_INFO("["<<i<<"]["<<i*1.0/TRAIN*100<<"%]Loss Value "<<
            testNet.train(Input,Result,lossFun,4));
        //#TODO 输出训练的loss中间结果到文件，后续python做可视化
        
    }
    unsigned int endTime = DBGTime.GetITime();
    std::cout<<"===========RESULT used "<<endTime-startTime<<"ms==========="<<std::endl;
    /*重新输出训练样本做结果测试*/
    for(int i=0;i<4;i++)
    {
        testNet.run(Input[i]).show();
    }
    std::cout<<"================END==================="<<std::endl;
    /*网络参数导入导出测试*/
    std::cout<<"=====PARAM OUTPUT&INSTALL TEST========"<<std::endl;
    unsigned int left = OUTMEMLEN;
    unsigned int used = 0;
    char* paramMem = (char*)malloc(OUTMEMLEN);
    char* offset = paramMem;
    int fd=-1;

    //依次导出2层参数
    for(int i=0;i<testNet.getLayerNum();i++)
    {
        left = OUTMEMLEN - used;
        if(!testNet[i]->ParamSave(offset,left))
        {
            LOG_ERR("["<<i<<"]Output Net Param failed!");
            break;
        }
        else
        {
            //left 存储当前使用的长度
            used += left; 
        }
    }
    
    fd = open(PARAMFILE,O_RDWR|O_CREAT,0600);
    if((offset-paramMem) == write(fd,paramMem,offset-paramMem))
    {
        LOG_INFO("Save Param successfully!");
    }
    else
    {
        LOG_INFO("Save Param failed! err"<<errno);
    }

    if(paramMem)
    {
        free(paramMem);
        paramMem = NULL;
    }
    if(fd >=0 )
    {
        close(fd);
        fd = -1;
    }
    std::cout<<"===================================="<<std::endl;
    /*参数导入*/ 
    //构建新网络
    FCLNet<double> testNet2;
    testNet2.Addlayer(new NolinearUnit<double,2,3,4>(new SigmoidActivator<double>()));
    testNet2.Addlayer(new NolinearUnit<double,3,1,4>(new SigmoidActivator<double>()));

    fd = open(PARAMFILE,O_RDONLY|O_EXCL,0600);
    paramMem = (char*)malloc(1024);
    offset = paramMem;
    //读取参数
    if(read(fd,paramMem,1024)>0)
    {
        LOG_INFO("Read Param successfully!");
    }
    else
    {
        LOG_INFO("Read Param failed!");
    }

    for(int i=0;i<testNet2.getLayerNum();i++)
    {
        if(!testNet2[i]->ParamInstall(offset))
        {
            LOG_ERR("Output Net Param failed!");
            break;
        }
    }
    /*新网络测试输入参数是否有效*/
    for(int i=0;i<4;i++)
    {
        testNet2.run(Input[i]).show();
    }
    std::cout<<"================END==================="<<std::endl;
    /*#TODO 网络参数调整做梯度检查*/
    std::cout<<"============GRADIENT CHECK============"<<std::endl;
    std::cout<<"================END==================="<<std::endl;
    /*#TODO 二维全局范围的数据可视化*/
    std::cout<<"===============VISUAL================="<<std::endl;
    std::cout<<"================END==================="<<std::endl;
    return true;
}














