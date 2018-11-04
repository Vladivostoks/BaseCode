/******************************************************/
//-*-coding-utf-8-*-
//File:Ann.cpp
//Date:2018-11-4
//Author:Ayden.Shu
//brief:ANN base
/******************************************************/
#include "Ann.h"
//网络层向量初始化
FCLNet::FCLNet():layerVector()
{
}

FCLNet::~FCLNet()
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
bool FCLNet::Addlayer(BaseUnit<double>* newLayer,int Index)
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
Array<double> FCLNet::run(Array<double>& indata)
{
    int i = 0;
    Array<double> tempdata = indata;
    for(i=0;i<layerVector.size();i++)
    {
       tempdata = layerVector[i]->UnitFront(tempdata); 
    }
    return tempdata;
}

//训练方法,输入训练数据和损失函数
template<class M>
M FCLNet::train(Array<double>& X,Array<double>& Y,BaseFun<M>* lossFun,bool enable)
{
    int i = 0;
    //step 1:获取输出个数
    BaseUnit<double>* outlayer = layerVector[layerVector.size()]; 
    int outSize = outlayer->GOutSize();
    //step 2:从损失函数，推倒出末端节点输入的Nextδ和NextWeight
    //最后一层节点衍生一层，用来包括进统一框架做迭代处理
    //  *->o 直接关系如左，o节点的δ由损失函数对o求导，并在y点处的值
    //  *->o 而w，实际上可以看作衍生全连接层，对应关联的节点连接权值为1，其余的为0
    //  *->o 因此构造的w是一个diag（1）单位矩阵，输出为最后一层输出的个数
    Array<double> tempδ(1,outSize,0);
    Array<double> tempw(outSize,outSize,0);
    //step 3:初始化输入矩阵
    tempw.diag(1);//W用1进行对角化
    tempδ = lossFun->back(Y);//对矩阵Y每个元素执行func后，赋值给tempδ
    
    //step 4:反向传播迭代 of course from back to begin
    for(i=layerVector.size();i>0;i--)
    {
        layerVector[i]->UnitBack(tempδ,tempw);
        //Now tempw&tempw update in UnitBack
        //tempδ = layerVector[i]->getδ();
        //tempw = layerVector[i]->getw();
    }
    //返回本次训练后的当前的loss值
    return enable?lossFun->front(run(X)):0;
}


BaseUnit<double>* FCLNet::operator[](unsigned int index)
{
    return  index>=layerVector.size()?NULL:layerVector[index]; 
}
