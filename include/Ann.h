/******************************************************/
//-*-coding-utf-8-*-
//File:nlp.cpp
//Date:2018-09-11
//Author:Ayden.Shu
//brief:ANN base
/******************************************************/
#ifndef __ANN__H__
#define __ANN__H__
/*
Α α [alpha] 阿尔法
Β β [beta] 贝塔
Γ γ [gamma] 伽马
Δ δ [delta] 德尔塔
Ε ε [epsilon] 伊普西龙
Ζ ζ [zeta] 截塔
Η η [eta] 艾塔
Θ θ [sita] 西塔
Ι ι [iota] 约塔
Κ κ [kappa] 卡帕
Λ λ [lambda] 兰布达
Μ μ [mu] 缪
Ν ν [nu] 纽
Ξ ξ [xi] 柯西
Ο ο [omicron] 奥密克戎
Π π [pi] 派
Ρ ρ [rho] 柔
Σ σ ς [sigma] 西格玛
Τ τ [tau] 套
Y υ [upsilon] 宇普西龙
Φ φ Φ[phi] 佛爱
Χ χ [chi] 西
Ψ ψ [psi] 普西
Ω ω [omega] 欧米伽
*/
#include <cmath>
#include <vector>
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>

#include "Matrix.h"
#include "sysTool.h"
#include "General.h"

/*********************************配置选项**********************************/
//学习率
#define η   0.05

/*********************************激活函数**********************************/
//激活函数和损失函数抽象
template<class T>
class ActiveFun{
    public:
        ActiveFun(){};
        virtual ~ActiveFun(){};

        //前向运算
        virtual T front(T value)=0;
        virtual Array<T> front(Array<T>& value)=0;
        //反响传播
        virtual T back(T value)=0;
        virtual Array<T> back(Array<T>& array)=0;
};

/*tanh激活函数*/
template<class T>
class TanhActivator:public ActiveFun<T>{
        TanhActivator(){};
        ~TanhActivator(){};
        //前向运算
        T front(T value); 
        Array<T> front(Array<T>& array);
        //反向传播
        T back(T value);
        Array<T> back(Array<T>& array);
};
template<class T>
T TanhActivator<T>::front(T value){
    return (T)(2,0/(1.0 + std::exp(-2*value))-1.0);
}

template<class T>
Array<T> TanhActivator<T>::front(Array<T>& array){
    return 2,0/(1.0 + (-2*array).exp())-1.0;
}

template<class T>
T TanhActivator<T>::back(T value){
    return (T)(1-value*value);
}

template<class T>
Array<T> TanhActivator<T>::back(Array<T>& array){
    return  (1-array*array); 
}

/*Sigmoid激活函数*/
template<class T>
class SigmoidActivator:public ActiveFun<T>{
    public:
        SigmoidActivator(){};
        ~SigmoidActivator(){};
        //前向运算
        T front(T value);
        Array<T> front(Array<T>& array);
        //反向传播
        T back(T value);
        Array<T> back(Array<T>& array);
};
template<class T>
T SigmoidActivator<T>::front(T value){
   return (T)(1.0/(1.0+std::exp(-value))); 
}

template<class T>
Array<T> SigmoidActivator<T>::front(Array<T>& array){
   return 1.0/(1.0+array.exp()); 
}

template <class T>
T SigmoidActivator<T>::back(T value){
    return (T)(front(value)*(1-front(value)));
}

template <class T>
Array<T> SigmoidActivator<T>::back(Array<T>& array){
    return front(array)*(1-front(array));
}

/*********************************损失函数**********************************/
template<class T>
class LossFun{
    public:
        LossFun(){};
        virtual ~LossFun(){};
        //前向运算
        T front(T value,T result); 
        virtual T front(Array<T>& array,Array<T>& result)=0;
        virtual T front(Array<T>& array,Array<T>&& result)=0;
        virtual T front(Array<T>&& array,Array<T>& result)=0;
        virtual T front(Array<T>&& array,Array<T>&& result)=0;
        //反向运算
        T back(T value,T result);
        virtual Array<T> back(Array<T>& array,Array<T>& result)=0;
        virtual Array<T> back(Array<T>& array,Array<T>&& result)=0;
        virtual Array<T> back(Array<T>&& array,Array<T>& result)=0;
        virtual Array<T> back(Array<T>&& array,Array<T>&& result)=0;
};


/*平方差函数*/
template<class T>
class SquareLoss:public LossFun<T>{
    public:
        SquareLoss(){};
        ~SquareLoss(){};
        //前向运算
        T front(T value,T result); 
        T front(Array<T>& array,Array<T>& result);
        T front(Array<T>& array,Array<T>&& result);
        T front(Array<T>&& array,Array<T>& result);
        T front(Array<T>&& array,Array<T>&& result);
        //反向运算
        T back(T value,T result);
        Array<T> back(Array<T>& array,Array<T>& result);
        Array<T> back(Array<T>& array,Array<T>&& result);
        Array<T> back(Array<T>&& array,Array<T>& result);
        Array<T> back(Array<T>&& array,Array<T>&& result);
};

template<class T>
T SquareLoss<T>::front(T value,T result)
{
    return (value-result)*(value-result)/2;
}

template<class T>
T SquareLoss<T>::front(Array<T>& array,Array<T>& result)
{
    return ((array-result)*(array-result)).squareSum()/2;
}
template<class T>
T SquareLoss<T>::front(Array<T>&& array,Array<T>& result)
{
    return ((array-result)*(array-result)).squareSum()/2;
}
template<class T>
T SquareLoss<T>::front(Array<T>& array,Array<T>&& result)
{
    return ((array-result)*(array-result)).squareSum()/2;
}
template<class T>
T SquareLoss<T>::front(Array<T>&& array,Array<T>&& result)
{
    return ((array-result)*(array-result)).squareSum()/2;
}

template<class T>
T SquareLoss<T>::back(T value,T result)
{
    return -(result-value);
}

template<class T>
Array<T> SquareLoss<T>::back(Array<T>& array,Array<T>& result)
{
    return -(result-array);
}
template<class T>
Array<T> SquareLoss<T>::back(Array<T>&& array,Array<T>& result)
{
    return -(result-array);
}
template<class T>
Array<T> SquareLoss<T>::back(Array<T>& array,Array<T>&& result)
{
    return -(result-array);
}
template<class T>
Array<T> SquareLoss<T>::back(Array<T>&& array,Array<T>&& result)
{
    return -(result-array);
}

/*********************************基本单元**********************************/
//基本单元抽象
template <class T>
class BaseUnit{
    protected: 
        //层属性
        std::string property;
    public:
        BaseUnit(){};
        BaseUnit(const char* name):property(name){};
        virtual ~BaseUnit(){};
        //方法
        virtual unsigned int getOutSize() const=0;
        virtual Array<T> getw() const =0;
        virtual Array<T> getbias() const =0;
        virtual Array<T> UnitFront(const Array<T>& input)=0;
        virtual bool UnitBack(Array<T>* Nextδ,Array<T>& NextWeight,unsigned int num=1)=0;
        //单元参数导入方法
        virtual bool ParamInstall(char *mem)=0;
        //单元参数导出方法
        virtual bool ParamSave(char *&mem,unsigned int& size)=0;
};

//训练的根本是根据误差项对w的梯度，对w进行调整
//1全连接网络组件：
//A 非线形感知层 接受m个输入，进行n输出,一个单元缓存最近的IndexSum组数据
template <class T,int m,int n,int IndexSum=1>
class NolinearUnit:public BaseUnit<T>{
    private:
        //单元输入个数，对应权重数
        const int InputSize;
        //单元输出个数
        const int OutputSize;
        //单元权重,m*n矩阵,代表n个单元,每个对m个输入的权值
        Array<T>  weight;
        //单元偏置,1*n矩阵，表示n个单元
        Array<T>  bias;
        //当前缓存序号
        unsigned int Index;
        //单元激活函数输入,1*n个单元
        Array<T>*  Out;
        //单元误差项,loss对单元输入的导数,1*n个误差项
        Array<T>*   δ;
        //单元当前输入值,1*m的数组
        Array<T>*  InputValue;
        //单元激活函数
        ActiveFun<T> *Active;
        //单元初始化函数
        bool ParamInit(Array<T>& value);

        //导入导出结构体
        typedef struct{
            //md5摘要
            unsigned char Digest[16];
            unsigned int dataType;
            //长度
            unsigned int memSize;
        }Param;
    public:
        NolinearUnit(ActiveFun<T> *fun,int M=m,int N=n);
        ~NolinearUnit();

        //单元流程
        unsigned int getOutSize() const {return OutputSize;};
        Array<T> getw() const {return weight;};
        Array<T> getbias() const {return bias;};
        //参数导入
        bool ParamInstall(char *mem);
        //参数导出
        bool ParamSave(char *&mem,unsigned int& size);
        //前向计算
        Array<T> UnitFront(const Array<T>& input);
        //反响计算
        bool UnitBack(Array<T>* Nextδ,Array<T>& NextWeight,unsigned int num=1);
};

template <class T,int m,int n,int IndexSum>
NolinearUnit<T,m,n,IndexSum>::NolinearUnit(ActiveFun<T> *fun,int M,int N):BaseUnit<T>("NolinearUnit"),
                                                            InputSize(M),
                                                            OutputSize(N),
                                                            weight(M,N,0),
                                                            bias(1,N,0),
                                                            Index(0),
                                                            Out(NULL),
                                                            δ(NULL),
                                                            InputValue(NULL),
                                                            Active(fun)
{
    //初始化单元权重和状态
    ParamInit(weight);
    //计算数组缓存初始化
    Out = new Array<T>[IndexSum];
    δ = new Array<T>[IndexSum];
    InputValue = new Array<T>[IndexSum];
    if(NULL == Out || NULL == δ || NULL == InputValue)
    {
        LOG_ERR("NolinearUnit Creat failed");
    }
    else
    {
        for(int i=0;i<IndexSum;i++)
        {
            Out[i] = Array<T>(1,N);
            δ[i] = Array<T>(1,N);
            InputValue[i] = Array<T>(1,M);
        }
    }
}

template <class T,int m,int n,int IndexSum>
NolinearUnit<T,m,n,IndexSum>::~NolinearUnit()
{
    if(NULL != Active)
    {
        delete Active;
    }
}

//确认矩阵初始化的值
template <class T,int m,int n,int IndexSum>
bool NolinearUnit<T,m,n,IndexSum>::ParamInit(Array<T>& value)
{
    //矩阵随机初始化,范围内均匀分布随机数,先随便定个范围
    value.random(-2,2);
    return true;
}

//#TODO 参数导入
template <class T,int m,int n,int IndexSum>
bool NolinearUnit<T,m,n,IndexSum>::ParamInstall(char *mem)
{
    //step1:校验
    Param* Head=reinterpret_cast<Param*>(mem);
    char* dataMem=mem+sizeof(Param);

    unsigned char tempMD5[16]={0};
    unsigned int tempdataType = Head->dataType;

    //设置当前的类型
    Head->dataType = sizeof(T);
    //计算内容摘要 
    MD5_CTX md5;
	MD5Init(&md5);         		
	MD5Update(&md5,(unsigned char*)mem,Head->memSize);
	MD5Final(&md5,tempMD5);

    if(0 != memcmp(tempMD5,Head->Digest,16))
    {
        LOG_ERR("Param install failed!");
        Head->dataType = tempdataType;
        return false;
    }
    //step2:导入
    weight = Array<T>::MemOut(dataMem);  
    bias = Array<T>::MemOut(dataMem);

    return true;
}

//参数导出
template <class T,int m,int n,int IndexSum>
bool NolinearUnit<T,m,n,IndexSum>::ParamSave(char *&mem,unsigned int& size)
{
    Param* Head=reinterpret_cast<Param*>(mem);
    char* dataMem=mem+sizeof(Param);
    //step1:生成校验参数
    if(size < sizeof(weight)+weight.getSize()+sizeof(bias)+bias.getSize()+sizeof(Param) || NULL == mem)
    {
        goto ERR;
    }
    //step2:导出,先导出数组管理对象，然后导出数组具体内容
    memset(Head,0,sizeof(Param));

    if(!weight.MemSave(dataMem))
    {
        goto ERR;
    }
    
    if(!bias.MemSave(dataMem))
    {
        goto ERR;
    }
    //MD5校验,不包括头部
    Head->memSize = dataMem-mem;
    Head->dataType = sizeof(T);

    //计算内容摘要,不包括MD5校验字段
    MD5_CTX md5;
	MD5Init(&md5);         		
	MD5Update(&md5,(unsigned char*)mem+16,Head->memSize-16);
	MD5Final(&md5,Head->Digest);        
   
    //返回长度,更新内存地址
    size = Head->memSize;
    mem += size;
    return true; 
ERR:
    LOG_ERR("ParamSave failed,need more space!");
    size = sizeof(weight)+weight.getSize()+sizeof(bias)+bias.getSize()+sizeof(Param);
    return false;
}

//单元前向计算
template <class T,int m,int n,int IndexSum>
Array<T> NolinearUnit<T,m,n,IndexSum>::UnitFront(const Array<T>& input)
{
    //记录输入值
    InputValue[Index] = input;
    //输入进行矩阵运算,并记录线形计算输出值
    Out[Index] = Array<T>::dot(InputValue[Index],weight)+bias;
    Array<T> rc = Active->front(Out[Index]); 
    //更新当前缓存数组索引
    Index = (Index+1)%IndexSum;
    return rc; 
}

//单元反向计算
template <class T,int m,int n,int IndexSum>
bool NolinearUnit<T,m,n,IndexSum>::UnitBack(Array<T>* Nextδ,Array<T>& NextWeight,unsigned int num)
{
    //更新权重,权重为和此节点下游连接所有节点的权重和δ值,假设下游节点k个，权重矩阵为n*k，误差项为1*k个
    //step 1:计算loss对单元输入导数的误差项
    Array<T> tempWeight= weight;
    Array<T> tempBias = bias; 
    tempWeight.clear();
    tempBias.clear(); 
    if(NULL == Nextδ)
    {
        LOG_ERR("Nextδ is NULL!!");
        return false;
    }
    if(num > IndexSum)
    {
        //防止溢出
        LOG_INFO("Warming data trained overflow IndexSum,should reduce num or increase IndexSum");
        num = IndexSum;
    }
    //从最后一个开始,向前循环
    for(int i=Index-1,j=0;j<num;j++)
    {
        i=i<0?i+IndexSum:i;
        //δ存对应索引计算出的数组
        δ[i] = Active->back(Out[i])*Array<T>::dot(Nextδ[i],NextWeight.T());
        NextWeight.T();
        tempWeight = tempWeight+η*Array<T>::dot(InputValue[i].T(),δ[i]);
        InputValue[i].T();
        tempBias = tempBias+η*δ[i];
        //循环
        i--;
    }
    //step 2:更新权重矩阵 输入是1*m dot 1*n 为梯度 weight m*n
    weight = weight-tempWeight;
    //step 3:更新偏置矩阵 
    bias = bias-tempBias;
    //更新输入矩阵
    for(int i=0;i<num;i++)
    {
        Nextδ[i] = δ[i];
    }
    NextWeight = weight;
    //返回本节点误差项
    return true;
}

//Network Connected Module
//数据传播网络抽象，主要成员是层组件向量

template<class M>
class FCLNet{
    private:
        //vector存储所有层
        std::vector<BaseUnit<M>*> layerVector;
    public:
        FCLNet();
        ~FCLNet();
        //增加层 
        bool Addlayer(BaseUnit<M>* newLayer,int index=0);
        //网络包含层数
        unsigned int getLayerNum();
        //前向传播
        Array<M> run(Array<M>& indata);
        //反向传播,模版成员函数
        M train(Array<M>* X,Array<M>* Y,LossFun<M>& lossFun,const unsigned int num=1,bool enable=true);
        //快速索引
        BaseUnit<M>* operator[](unsigned int index);
};

//网络层向量初始化
template<class M>
FCLNet<M>::FCLNet():layerVector()
{
}

template<class M>
FCLNet<M>::~FCLNet()
{
    //析构所有层
    for(int i=0;i<layerVector.size();i++)
    {
        if(NULL != layerVector[i])
        {
           delete layerVector[i]; 
        }
    }
}
//添加一个层,默认在尾部增加
template<class M>
bool FCLNet<M>::Addlayer(BaseUnit<M>* newLayer,int index)
{
    if(NULL == newLayer)
    {
        LOG_ERR("Add layer is empty!")
    }

    if(true == layerVector.empty())
    {
        layerVector.push_back(newLayer);
    }
    else
    {
        std::vector<BaseUnit<double>*>::iterator it=layerVector.end();
        for(int i=0;it!=layerVector.begin();it--,i++)
        {
            if(i == index)
            {
                break;
            }
        }
        //插入
        layerVector.insert(it,newLayer);
    }

    return true;
}
    
//返回总层数
template<class M>
unsigned int FCLNet<M>::getLayerNum()
{
    return layerVector.size();
}

//输入数据，做网络计算,返回最后一个计算数组
template<class M>
Array<M> FCLNet<M>::run(Array<M>& indata)
{
    int i = 0;
    Array<M> tempdata = indata;
    for(i=0;i<layerVector.size();i++)
    {
       tempdata = layerVector[i]->UnitFront(tempdata); 
    }
    return tempdata;
}

//训练方法,输入训练数据和损失函数,支持一次多组数据
template<class M>
M FCLNet<M>::train(Array<M>* X,Array<M>* Y,LossFun<M>& lossFun,const unsigned int num,bool enable)
{
    int i = 0;
    //step 1:获取输出个数
    BaseUnit<M>* outlayer = layerVector[layerVector.size()-1]; 
    int outSize = outlayer->getOutSize();
    //step 2:从损失函数，推倒出末端节点输入的Nextδ和NextWeight
    //最后一层节点衍生一层，涉及激活函数包括进统一过程做迭代处理
    //  *->o 直接关系如左，o节点的δ由损失函数对o求导，并在y点处的值
    //  *->o 而w，实际上可以看作衍生全连接层，对应关联的节点连接权值为1，其余的为0
    //  *->o 因此构造的w是一个diag（1）单位矩阵，输出为最后一层输出的个数
    Array<M> tempw(outSize,outSize,0);
    //step 3:初始化输入矩阵
    tempw.diag(1);//W用1进行对角化
    //对矩阵Y每个元素执行func后，赋值给tempδ
    Array<M>* tempδ = new Array<M>[num];
    for(i=0;i<num;i++)
    {
        tempδ[i] = lossFun.back(run(X[i]),Y[i]);
    }
    //step 4:反向传播迭代 of course from back to begin
    for(i=layerVector.size()-1;i>=0;i--)
    {
        //Now tempw&tempw update in UnitBack
        layerVector[i]->UnitBack(tempδ,tempw,num);
    }
    //返回本次训练后的当前的loss值
    if(enable)
    {
        M ret = 0;
        for(i=0;i<num;i++)
        {
            ret += lossFun.front(run(X[i]),Y[i]);
        }
        return ret;
    }
    
    return 0;
}


template<class M>
BaseUnit<M>* FCLNet<M>::operator[](unsigned int index)
{
    return  index>=layerVector.size()?NULL:layerVector[index]; 
}

//2卷积网络组件：
//A卷积单元
//B池化单元
//1-A全连接网络

//3LTSM单元
//A遗忘门
//1-A 非线性单元
//B输入门
//1-A 非线性单元
//C输出门
//1-A 非线性单元

//4其他中间兼容转换层组件
#endif
