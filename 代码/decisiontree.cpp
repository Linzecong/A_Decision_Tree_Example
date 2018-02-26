#include<vector>
#include<math.h>
#include"decisiontree.h"

#include<QMessageBox>
using namespace std;
Tree::Tree(vector<int> AttrNum,TrainData data){
    /*输入属性列表，输入每个属性的分类属性个数即可*/

    for(int aa=0;aa<AttrNum.size();aa++){
        vector<int> temp;
        for(int i=1;i<=AttrNum[aa];i++)
            temp.push_back(i);
        AttrData.push_back(temp);
    }

    /*输入训练数据，直接按顺序输入分类属性序号*/
    //    TrainData data;
    //    stop=0;
    //    while(!stop){
    //        vector<int> train;
    //        cout<<"TrainData:";
    //        int aa=0;
    //        for(unsigned int i=0;i<AttrData.size();i++){
    //            cin>>aa;
    //            train.push_back(aa);
    //        }
    //        cout<<"OutPut:";
    //        bool aaa;
    //        cin>>aaa;
    //        data.InSertData(train,aaa);
    //        cout<<"Stop?"<<endl;
    //        cin>>stop;
    //    }

    vector<int> temp2;
    Root=CreateTree(data,temp2);
}

Node* Tree::CreateTree(TrainData data,vector<int> usedAttr){

    Node* root=new Node(0,0);//创建根节点

    /*如果输出都一样，则创建一个节点，值为该输出，且为叶节点*/
    int stop=1;
    for(unsigned int i=1;i<data.OutPut.size();i++){
        if(data.OutPut[i]!=data.OutPut[i-1])
            stop=0;
    }
    if(stop)
        return new Node(data.OutPut[0],1);

    /*如果所有属性都用过，那么返回的节点的值为最普遍的输出，且为叶节点*/
    if(usedAttr.size()==AttrData.size())
        return new Node(MostNormalOutPut(data),1);

    /*选出信息增益最高的属性作为节点*/
    pair<int,double> A=Best(data,usedAttr);
    usedAttr.push_back(A.first);//加入已使用属性
    root->Attribute=A.first;
    root->Entropy=A.second;

    /*递归的在每一个分类属性下新建一棵树*/
    for(unsigned int i=0;i<AttrData[A.first].size();i++){
        TrainData tempExample;
        for(unsigned int j=0;j<data.OutPut.size();j++){
            if(i==data.Input[j][A.first]){
                tempExample.InSertData(data.Input[j],data.OutPut[j]);
            }
        }
        if(tempExample.OutPut.empty()){
            root->Num.push_back(new Node(MostNormalOutPut(data),1));
        }
        else{
            root->Num.push_back(CreateTree(tempExample,usedAttr));
        }
    }
    return root;
}

int Tree::MostNormalOutPut(TrainData data){
    vector<int> out;//记录输出的种类
    vector<int> count;//记录种类的数量
    for(unsigned int i=0;i<data.OutPut.size();i++){
        bool ex=0;
        int index=0;
        for(unsigned int j=0;j<out.size();j++){
            if(out[j]==data.OutPut[i]){
                ex=1;
                index=j;
            }
        }
        if(ex){
            count[index]++;
        }
        else{
            out.push_back(data.OutPut[i]);
            count.push_back(1);
        }
    }
    /*计算数量最多的种类*/
    int maxi=0;
    int max=0;
    for(unsigned int i=0;i<count.size();i++){
        if(count[i]>max){
            maxi=i;
            max=count[i];
        }
    }
    return out[maxi];
}

double Tree::Entropy(TrainData data){
    /*计算输出种类和数量*/
    vector<double> out;
    vector<double> count;
    for(unsigned int i=0;i<data.OutPut.size();i++){
        bool ex=0;
        int index=0;
        for(unsigned int j=0;j<out.size();j++){
            if(out[j]==data.OutPut[i]){
                ex=1;
                index=j;
            }
        }
        if(ex){
            count[index]++;
        }
        else{
            out.push_back(data.OutPut[i]);
            count.push_back(1);
        }
    }
    /*计算信息熵*/
    double total=0;
    for(unsigned int i=0;i<count.size();i++)
        total+=count[i];
    double sum=0;
    for(unsigned int i=0;i<count.size();i++){
        double a=0;
        if((count[i]/total)!=0)
            a=log2((count[i]/total));
        sum-=(count[i]/total)*a;
    }
    return sum;
}

pair<int,double> Tree::Best(TrainData data,vector<int> usedAttr){
    vector<double>  Gain;//记录每一个属性的信息增益

    bool used;
    /*将使用过的属性的信息增益设置为0*/
    for(unsigned int i=0;i<AttrData.size();i++){
        used=0;
        for(unsigned int k=0;k<usedAttr.size();k++)
            if(i==usedAttr[k]){
                Gain.push_back(0.0);
                used=1;
            }
        if(used)
            continue;
        /*计算信息增益*/
        else{
            double es=Entropy(data);
            for(unsigned int j=0;j<AttrData[i].size();j++){
                TrainData tempData;
                for(unsigned int k=0;k<data.Input.size();k++){
                    if(j==data.Input[k][i]){
                        tempData.InSertData(data.Input[k],data.OutPut[k]);
                    }
                }
                if(!tempData.Input.empty()){
                    es-=(double(tempData.Input.size())/double(data.Input.size()))*Entropy(tempData);
                }
            }
            Gain.push_back(es);
        }
    }

    /*计算信息增益最高的属性*/

    pair<int,double> aaa;
    int maxi=0;
    double max=0;
    for(unsigned int i=0;i<Gain.size();i++){
        if(Gain[i]>max){
            aaa.first=maxi=i;
            aaa.second=max=Gain[i];
        }
    }
    return aaa;
}

int Tree::GetOutPut(vector<int> data,bool debug){

    if(Root->IsLeaf){
        return Root->Attribute;
    }

    map<int,QString> mm;
    mm[0]="左队";
    mm[1]="左赔率";
    mm[2]="右队";
    mm[3]="右赔率";

    Node* current=Root;
    while(!current->IsLeaf){
        current=current->Num[data[current->Attribute]];
        if(debug&&!current->IsLeaf)
            QMessageBox::information(0,"决策树选择","属性："+mm[current->Attribute]+"\n\n"+"信息增益："+QString::number(current->Entropy)+"\n\n");
    }
    return current->Attribute;
}
