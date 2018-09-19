/******************************************************/
//-*-coding-utf-8-*-
//File:nlp.cpp
//Date:2018-09-11
//Author:Ayden.Shu
//brief:ANN base
/******************************************************/
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

#include <Array.h>
#include <Matrix.h>

//激活函数抽象
template<class T>
class ActiveFun{
        ActiveFun(){};
        ~ActiveFun(){};

        //前向运算
        virtual T front(T value)=0;
        virtual Array<T> front(Array<T> value)=0;
        //反响传播
        virtual T back(T value)=0;
        virtual Array<T> back(Array<T> array)=0;
};

/*tanh激活函数*/
template<class T>
class TanhActivator:ActiveFun<T>{
        TanhActivator(){};
        ~TanhActivator(){};
        //前向运算
        T front(T value); 
        Array<T> front(Array<T> array);
        //反向传播
        T back(T value);
        Array<T> back(Array<T> array);
};
template<class T>
T TanhActivator<T>::front(T value){
    return (T)(2,0/(1.0 + std::exp(-2*value))-1.0);
}

template<class T>
Array<T> TanhActivator<T>::front(Array<T> array){
    return 2,0/(1.0 + (-2*array).exp())-1.0;
}

template<class T>
T TanhActivator<T>::back(T value){
    return (T)(1-value*value);
}

template<class T>
Array<T> TanhActivator<T>::back(Array<T> array){
    return  (1-array*array); 
}

/*Sigmoid激活函数*/
template<class T>
class SigmoidActivator:ActiveFun<T>{
    SigmoidActivator();
    ~SigmoidActivator();
    //前向运算
    T front(T value);
    Array<T> front(Array<T> array);
    //反向传播
    T back(T value);
    Array<T> back(Array<T> array);
};
template<class T>
T SigmoidActivator<T>::front(T value){
   return (T)(1.0/(1.0+std::exp(-value))); 
}

template<class T>
Array<T> SigmoidActivator<T>::front(Array<T> array){
   return 1.0/(1.0+array.exp()); 
}

template <class T>
T SigmoidActivator<T>::back(T value){
    return (T)(front(value)*(1-front(value)));
}

template <class T>
Array<T> SigmoidActivator<T>::back(Array<T> array){
    return front(array)*(1-front(array));
}


//学习率
#define η   0.1

//基本单元抽象
template <class T>
class BaseUnit{
    public:
        BaseUnit(){};
        virtual ~BaseUnit(){};
        //方法
        virtual Array<T> UnitFront(Array<T> input)=0;
        virtual Array<T> UnitBack(Array<T> Nextδ,Array<T> NextWeight)=0;
};

//训练的根本是根据误差项对w的梯度，对w进行调整
//1全连接网络组件：
//A 非线形感知层 接受m个输入，进行n输出
template <class T,int m,int n>
class NolinearUnit:public BaseUnit<T>{
    private:
        //单元输入个数，对应权重数
        const int InputSize;
        //单元权重,m*n矩阵,代表n个单元,每个对m个输入的权值
        Array<T>  weight;
        //单元偏置,1*n矩阵，表示n个单元
        Array<T>  bias;
        //单元输出,1*n个输出
        Array<T>  Out;
        //单元误差项,loss对单元输入的导数,1*n个误差项
        Array<T>   δ;
        //单元当前输入值,m*1的数组
        Array<T>  InputValue;
        //单元激活函数
        ActiveFun<T> *Active;
        //单元初始化函数
        bool Paraminit(Array<T> value);
    public:
        NolinearUnit(ActiveFun<T> *fun);
        ~NolinearUnit();

        //单元流程
        Array<T> UnitFront(Array<T> input);
        Array<T> UnitBack(Array<T> Nextδ,Array<T> NextWeight);
};

template <class T,int m,int n>
NolinearUnit<T,m,n>::NolinearUnit(ActiveFun<T> *fun):InputSize(n),
                                                    weight(m,n,0),
                                                    bias(1,n,0),
                                                    Out(1,n,0),
                                                    δ(1,n,0),
                                                    InputValue(m,1,0),
                                                    Active(fun)
{
    //初始化单元权重和状态
    Paraminith(weight);
}
template <class T,int m,int n>
bool NolinearUnit<T,m,n>::Paraminit(Array<T> value)
{
    return true;
}


template <class T,int m,int n>
Array<T> NolinearUnit<T,m,n>::UnitFront(const Array<T> input)
{
    //记录输入值
    InputValue = input;
    //输入进行矩阵运算
    Array<T> result = Matrix.dot(weight.T()).T()+bias;
    return Active->front(result);
}

template <class T,int m,int n>
Array<T> NolinearUnit<T,m,n>::UnitBack(Array<T> Nextδ,Array<T> NextWeight)
{
    //更新权重,权重为和此节点下游连接所有节点的权重和δ值,假设下游节点k个，权重矩阵为n*k，误差项为1*k个
    //step 1:计算loss对单元输入导数的误差项
    δ = Active->back(Out)*Matrix.dot(NextWeight,Nextδ.T()).T();
    //step 2:更新权重矩阵 m*1 dot 1*n 为梯度
    weight = weight-η*InputValue*δ;
    //返回本节点误差项
    return this->δ;
}
//Fully Connected Layer
class FCLNet{
    private:
        //所有层之间，
        vector<BaseUnit<double>*> layerVector;

    public:
        FCLNet();
        ~FCLNet();
        
        //前向传播
        run
        //反向传播
        train
};
//全连接网络生成
FCLNet::FCLNet():layerVector()
{
}
//添加序号以及隐藏节点个数
bool FCLNet::Addlayer(int num,int Index)
{
    iterator it=layerVector.head();
    for(it=layerVector.head();it!=layerVector.end();it++)
    {
        if(it == Index)
        {
            break;
        }
    }
       layerHead = new NolinearUnit<double,1>[10];
}
//输入数据，做全连接层计算
bool run()
{
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



