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
#define TRAIN   1000//100000
//导出的预设内存大小
#define OUTMEMLEN 1024
//保存输出参数文件名称
#define PARAMFILE "./var/paramfile.bin"
//梯度检查调整值
#define Δ 0.00001
//梯度检查输入个数
#define NUM 4

enum Type{
    ArrayTest=0,
    FCLNetTrain,
    GradientCheck,
    TempCheck,
    MAXType
};


bool FCLNetTestFunc();
bool ArrayTestFunc();
bool GradientCheckFun();
bool tempTest();

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
        case TempCheck:{
            tempTest();
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
    std::cout<<"======Test 8 BIG Array Cache hit&miss Test====="<<std::endl;
    Array<double> L(2048,2048,1.5);
    Array<double> M(2048,2048,3.1);
    LOG_INFO("start Cache miss");
    Array<double>::dot(L,M);
    LOG_INFO("end Cache miss");
    LOG_INFO("start Cache hit");
    Array<double>::dot(L,M.T());
    LOG_INFO("end Cache hit");
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
            testNet.train(Input,Result,lossFun,NUM));
        //打印梯度
#if 1
        for(int j=0;j<2;j++)
        {
            Array<double> ΔNet = (testNet[j]->getΔweight(NUM));
            //ΔNet.show();
        }
#endif
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
    unsigned int used = 0;
    char* paramMem = (char*)malloc(OUTMEMLEN);
    memset(paramMem,0,OUTMEMLEN);
    int fd=-1;

    used = testNet.ParamSave(paramMem,OUTMEMLEN);
    LOG_ERR("used "<<used);
    
    fd = open(PARAMFILE,O_RDWR|O_CREAT,0600);
    if(used == write(fd,paramMem,used))
    {
        LOG_INFO("Save Param successfully!");
    }
    else
    {
        LOG_INFO("Save Param failed! errno="<<errno);
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
    paramMem = (char*)malloc(OUTMEMLEN);
    memset(paramMem,0,OUTMEMLEN);
    //读取参数
    if(read(fd,paramMem,OUTMEMLEN)>0)
    {
        LOG_INFO("Read Param successfully!");
    }
    else
    {
        LOG_INFO("Read Param failed! errno="<<errno);
    }

    testNet2.ParamInstall(paramMem);
    /*新网络测试输入参数是否有效*/
    for(int i=0;i<4;i++)
    {
        testNet2.run(Input[i]).show();
    }
    std::cout<<"================END==================="<<std::endl;
    return true;
}
//#TODO check error
bool tempTest()
{
    char* paramMem = NULL;
    int fd = -1;
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
    
    FCLNet<double> testNet;
    testNet.Addlayer(new NolinearUnit<double,2,3,4>(new SigmoidActivator<double>()));
    testNet.Addlayer(new NolinearUnit<double,3,1,4>(new SigmoidActivator<double>()));

    fd = open(PARAMFILE,O_RDONLY|O_EXCL,0600);
    paramMem = (char*)malloc(OUTMEMLEN);
    memset(paramMem,0,OUTMEMLEN);
    //读取参数
    int ret = 0;
    if((ret=read(fd,paramMem,OUTMEMLEN))>0)
    {
        LOG_INFO("Read Param successfully!"<<ret);
    }
    else
    {
        LOG_INFO("Read Param failed! errno="<<errno);
    }
    close(fd);
    testNet.ParamInstall(paramMem);
    /*新网络测试输入参数是否有效*/
    for(int i=0;i<4;i++)
    {
        testNet.run(Input[i]).show();
    }
    return true;
}

bool GradientCheckFun()
{   
    char* paramMem = NULL;
    int fd = -1;
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
    //std::unique_ptr<NolinearUnit<double,2,3,4>> layer1(new NolinearUnit<double,2,3,4>(new SigmoidActivator<double>()));
    //std::unique_ptr<NolinearUnit<double,3,1,4>> layer2(new NolinearUnit<double,3,1,4>(new SigmoidActivator<double>()));
    testNet2.Addlayer(new NolinearUnit<double,2,3,4>(new SigmoidActivator<double>()));
    testNet2.Addlayer(new NolinearUnit<double,3,1,4>(new SigmoidActivator<double>()));

    /*随机梯度下降单样本输入做网络参数调整做梯度检查*/
    //构建新网络
    /*w矩阵重构*/
    std::cout<<"============GRADIENT CHECK============"<<std::endl;
    paramMem = (char*)malloc(OUTMEMLEN);
    fd = open(PARAMFILE,O_RDONLY|O_EXCL,0600);
    memset(paramMem,0,OUTMEMLEN);
    int ret =0;
    //从文件加载参数
    if((ret=read(fd,paramMem,OUTMEMLEN))>0)
    {
        LOG_INFO("Read Param successfully!");
    }
    else
    {
        LOG_INFO("Read Param failed! errno="<<errno);
    }
    testNet2.ParamInstall(paramMem);
    //先跑一段，降一下下梯度
#if 1
    for(int j=0;j<TRAIN;j++)
        testNet2.train(Input,Result,lossFun,NUM);
    //需要将整个网络的参数导出,做梯度检查
    memset(paramMem,0,OUTMEMLEN);
    testNet2.ParamSave(paramMem,OUTMEMLEN);
#endif
    for(int j=0;j<testNet2.getLayerNum();j++)
    {
        std::cout<<"===============Layer "<<j<<"================="<<std::endl;
        //STEP1:保存当前weight值，并且进行一次反向计算 
        //只能改其中一支的值，因为按照微分计算出的倒数只是一个值
        //需要手动指定层的子类,获取子类独有成员
        char* paramMem1 = NULL;
        char* paramMem2 = NULL;
        char* paramMem3 = paramMem;
        double loss1=0;
        double loss2=0;
#if 0
        /*手动计算的梯度值*/
        double ΔManual=0;
        /*网络训练的梯度值*/
        double ΔNet=0;
#endif

        paramMem1 = (char*)malloc(OUTMEMLEN);
        paramMem2 = (char*)malloc(OUTMEMLEN);

        //需要将整个网络的参数导出,先恢复原始参数
        if(!testNet2.ParamInstall(paramMem3))
        {
            LOG_ERR("testNet2 P Install failed!");
        }

        testNet2.CheckSave(paramMem1,OUTMEMLEN,Δ,j);
        testNet2.CheckSave(paramMem2,OUTMEMLEN,-Δ,j);
        
        LOG_INFO("===property Gradient======");
        for(int k=0;k<1;k++)
            testNet2.train(Input,Result,lossFun,NUM);
        //STEP2:使用w1和w2下同样本输入的loss值计算梯度
        LOG_INFO("===increase Gradient======");
        testNet2.ParamInstall(paramMem1);
        for(int i=0;i<NUM;i++)
        {
            loss1 = loss1+lossFun.front(testNet2.run(Input[i]),Result[i]);
        }

        LOG_INFO("===descrease Gradient======");
        testNet2.ParamInstall(paramMem2);
        for(int i=0;i<NUM;i++)
        {
            loss2 = loss2+lossFun.front(testNet2.run(Input[i]),Result[i]);
        }

        double ΔManual = (loss1-loss2)/(2*Δ);
        //STEP3:对比反向计算和微分计算出的梯度
        Array<double> ΔNet = (testNet2[j]->getΔweight(NUM));

        LOG_INFO("Δweight=>["<<ΔManual<<"]");
        LOG_INFO("=======ΔNet============");
        ΔNet.show();
        //LOG_INFO("Δweight=>["<<ΔManual<<"]ΔNet=>["<<ΔNet<<"] diff=>["<<ΔManual-ΔNet<<"]");
        if(NULL != paramMem1)
            free(paramMem1);
        if(NULL != paramMem2)
            free(paramMem2);
    }
    if(NULL != paramMem)
        free(paramMem);
    std::cout<<"================END==================="<<std::endl;
    /*#TODO 借助python实现二维全局范围的数据可视化,后面做pythonAPI后直接在python实现*/
    std::cout<<"===============VISUAL================="<<std::endl;
    std::cout<<"================END==================="<<std::endl;
    return true;
}












