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
#define η   0.0001

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
        virtual Array<T> getδ() const =0;
        virtual Array<T> getbias() const =0;
        virtual Array<T> UnitFront(const Array<T>& input)=0;
        virtual Array<T> UnitBack(Array<T>& Nextδ,Array<T>& NextWeight)=0;
};

//训练的根本是根据误差项对w的梯度，对w进行调整
//1全连接网络组件：
//A 非线形感知层 接受m个输入，进行n输出
template <class T,int m,int n>
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
        //单元激活函数输入,1*n个单元
        Array<T>  Out;
        //单元误差项,loss对单元输入的导数,1*n个误差项
        Array<T>   δ;
        //单元当前输入值,1*m的数组
        Array<T>  InputValue;
        //单元激活函数
        ActiveFun<T> *Active;
        //单元初始化函数
        bool ParamInit(Array<T>& value);
    public:
        NolinearUnit(ActiveFun<T> *fun,int M=m,int N=n);
        ~NolinearUnit();

        //单元流程
        unsigned int getOutSize() const {return OutputSize;};
        Array<T> getw() const {return weight;};
        Array<T> getδ() const {return δ;};
        Array<T> getbias() const {return bias;};
        Array<T> UnitFront(const Array<T>& input);
        Array<T> UnitBack(Array<T>& Nextδ,Array<T>& NextWeight);
};

template <class T,int m,int n>
NolinearUnit<T,m,n>::NolinearUnit(ActiveFun<T> *fun,int M,int N):BaseUnit<T>("NolinearUnit"),
                                                            InputSize(M),
                                                            OutputSize(N),
                                                            weight(M,N,0),
                                                            bias(1,N,0),
                                                            Out(1,N,0),
                                                            δ(1,N,0),
                                                            InputValue(1,M,0),
                                                            Active(fun)
{
    //初始化单元权重和状态
    ParamInit(weight);
}

template <class T,int m,int n>
NolinearUnit<T,m,n>::~NolinearUnit()
{
    if(NULL != Active)
    {
        delete Active;
    }
}

//确认矩阵初始化的值
template <class T,int m,int n>
bool NolinearUnit<T,m,n>::ParamInit(Array<T>& value)
{
    //矩阵随机初始化,范围内均匀分布随机数,先随便定个范围
    value.random(-2,2);
    return true;
}


template <class T,int m,int n>
Array<T> NolinearUnit<T,m,n>::UnitFront(const Array<T>& input)
{
    //记录输入值
    InputValue = input;
    //输入进行矩阵运算 
    Out = Array<T>::dot(InputValue,weight)+bias;
    return Active->front(Out);
}

template <class T,int m,int n>
Array<T> NolinearUnit<T,m,n>::UnitBack(Array<T>& Nextδ,Array<T>& NextWeight)
{
    //更新权重,权重为和此节点下游连接所有节点的权重和δ值,假设下游节点k个，权重矩阵为n*k，误差项为1*k个
    //step 1:计算loss对单元输入导数的误差项
    δ = Active->back(Out)*Array<T>::dot(Nextδ,NextWeight.T());
    //step 2:更新权重矩阵 输入是1*m dot 1*n 为梯度 weight m*n
    weight = weight-η*Array<T>::dot(InputValue.T(),δ);
    //step 3:更新偏置矩阵 
    bias = bias - η*δ;
    //更新输入矩阵
    Nextδ = δ;
    NextWeight = weight;
    //返回本节点误差项
    return δ;
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
        bool Addlayer(BaseUnit<M>* newLayer,int Index=0);
        //前向传播
        Array<M> run(Array<M>& indata);
        //反向传播,模版成员函数
        M train(Array<M>& X,Array<M>& Y,LossFun<M>& lossFun,bool enable=true);
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
bool FCLNet<M>::Addlayer(BaseUnit<M>* newLayer,int Index)
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
            if(i == Index)
            {
                break;
            }
        }
        //插入
        layerVector.insert(it,newLayer);
    }

    return true;
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
M FCLNet<M>::train(Array<M>& X,Array<M>& Y,LossFun<M>& lossFun,bool enable)
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
    Array<M> tempδ(1,outSize,0);
    Array<M> tempw(outSize,outSize,0);
    //step 3:初始化输入矩阵
    tempw.diag(1);//W用1进行对角化
    //对矩阵Y每个元素执行func后，赋值给tempδ
    tempδ = lossFun.back(run(X),Y);
    //step 4:反向传播迭代 of course from back to begin
    for(i=layerVector.size()-1;i>=0;i--)
    {
        //Now tempw&tempw update in UnitBack
        layerVector[i]->UnitBack(tempδ,tempw);
    }
    //返回本次训练后的当前的loss值
    return enable?lossFun.front(run(X),Y):0;
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
