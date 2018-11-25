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
//梯度检查调整值
#define Δ 0.000000000001
//梯度检查输入个数
#define NUM 3

enum Type{
    ArrayTest=0,
    FCLNetTrain,
    GradientCheck,
    MAXType
};


bool FCLNetTestFunc();
bool ArrayTestFunc();
bool GradientCheckFun();

int main(int argc,char* argv[])
{
    int type = 0;
    if(argc < 2)
    {
        do{
            printf("Illigal Test Type,please choose a unit of test:\n");
            printf("                                0 Array Test\n");
            printf("                                1 FCLNet Train Test\n");
            printf("                                2 Gradient Check\n");
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
        case FCLNetTrain:{
            FCLNetTestFunc();
            break;
        }
        case GradientCheck:{
            GradientCheckFun();
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
        //后面做pythonAPI后直接在python实现
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
    std::cout<<"==============NEW NET================="<<std::endl;
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
    
    return true;
}

bool GradientCheckFun()
{   
    /*参数导入*/ 
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
    SquareLoss<double> lossFun;
    //构建新网络
    FCLNet<double> testNet2;
    NolinearUnit<double,2,3,4>* layer1 = NULL;
    NolinearUnit<double,3,1,4>* layer2 = NULL;
    //std::unique_ptr<NolinearUnit<double,2,3,4>> layer1(new NolinearUnit<double,2,3,4>(new SigmoidActivator<double>()));
    //std::unique_ptr<NolinearUnit<double,3,1,4>> layer2(new NolinearUnit<double,3,1,4>(new SigmoidActivator<double>()));
    testNet2.Addlayer(layer1 = new NolinearUnit<double,2,3,4>(new SigmoidActivator<double>()));
    testNet2.Addlayer(layer2 = new NolinearUnit<double,3,1,4>(new SigmoidActivator<double>()));

    /*随机梯度下降单样本输入做网络参数调整做梯度检查*/
    /*w矩阵重构*/
    std::cout<<"============GRADIENT CHECK============"<<std::endl;
    for(int j=0;j<testNet2.getLayerNum();j++)
    {
        //STEP1:保存当前weight值，并且进行一次反向计算 
        //只能改其中一支的值，因为按照微分计算出的倒数只是一个值
        //需要手动指定层的子类,获取子类独有成员
        char* paramMem1 = NULL;
        char* paramMem2 = NULL;
        char* offset1 = NULL;
        char* offset2 = NULL;
    
        paramMem1 = (char*)malloc(1024);
        paramMem2 = (char*)malloc(1024);

        offset1 = paramMem1;
        offset2 = paramMem2;   

        if(0==j)
        {
            unsigned int left = 1024;
            Array<double>& Weight = dynamic_cast<NolinearUnit<double,2,3,4>*>(testNet2[j])->weight;
            Weight[0][0] = Weight[0][0]+Δ;
            testNet2[j]->ParamSave(offset1,left);

            left=1024;
            Weight[0][0] = Weight[0][0]-2*Δ;
            testNet2[j]->ParamSave(offset2,left);
            //还原
            Weight[0][0] = Weight[0][0]+Δ;
        }
        else if(1==j)
        {
            unsigned int left = 1024;
            Array<double>& Weight = dynamic_cast<NolinearUnit<double,3,1,4>*>(testNet2[j])->weight;
            Weight[0][0] = Weight[0][0]+Δ;
            testNet2[j]->ParamSave(offset1,left);

            left=1024;
            Weight[0][0] = Weight[0][0]-2*Δ;
            testNet2[j]->ParamSave(offset2,left);
            //还原
            Weight[0][0] = Weight[0][0]+Δ;
        }
        else
        {
            LOG_ERR("break");
            break;
        }
        
        double loss1=0;
        double loss2=0;
        double Δweight=0;
        double ΔNet=0;

        offset1 = paramMem1;
        offset2 = paramMem2;

        testNet2.train(Input,Result,lossFun,NUM);
        //STEP2:使用w1和w2下同样本输入的loss值计算梯度
        testNet2[j]->ParamInstall(offset1);
        for(int i=0;i<NUM;i++)
        {
            loss1 = loss1+lossFun.front(testNet2.run(Input[i]),Result[i]);
        }

        testNet2[j]->ParamInstall(offset2);
        for(int i=0;i<NUM;i++)
        {
            loss2 = loss2+lossFun.front(testNet2.run(Input[i]),Result[i]);
        }

        Δweight = (loss1-loss2)/(2*Δ);
        //STEP3:对比反向计算和微分计算出的梯度
        ΔNet = (testNet2[j]->getΔweight(NUM))[0][0];
        LOG_INFO("Δweight=>["<<Δweight<<"]ΔNet=>["<<ΔNet<<"] diff=>["<<Δweight-ΔNet<<"]");
    }
    std::cout<<"================END==================="<<std::endl;
    /*#TODO 借助python实现二维全局范围的数据可视化,后面做pythonAPI后直接在python实现*/
    std::cout<<"===============VISUAL================="<<std::endl;
    std::cout<<"================END==================="<<std::endl;
    return true;
}












