#include "widget.h"
#include "ui_widget.h"

#include<QString>
#include<QtNetwork/QNetworkAccessManager>
#include<QTextEdit>
#include<QMessageBox>
#include<QFile>
#include<QDateTime>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlquery.h>
#include <QtSql/QSqlError>
#include <iostream>
#include <iomanip>
#include <set>
#include <map>
#include "decisiontree.h"
#include <QMessageBox>
#include "NeuralNet.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager();
    currentUrl=4;
    connect(&Timer,&QTimer::timeout,this,&Widget::go);
    connect(ui->pushButton,&QPushButton::clicked,this,&Widget::start);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&Widget::end);
    connect(ui->LoadButton,&QPushButton::clicked,this,&Widget::loadData);
    connect(ui->fliteButton,&QPushButton::clicked,this,&Widget::fliteData);
    connect(ui->YuceAllButton,&QPushButton::clicked,this,&Widget::testID3);
    connect(ui->ID3Button,&QPushButton::clicked,this,&Widget::ID3);
    connect(ui->YuceButton,&QPushButton::clicked,this,&Widget::dantestID3);

    connect(ui->ANNYuceButton,&QPushButton::clicked,this,&Widget::testANN);
    connect(ui->ANNButton,&QPushButton::clicked,this,&Widget::ANN);
    connect(ui->YuceANNButton,&QPushButton::clicked,this,&Widget::dantestANN);

    connect(ui->LTeam,&QListWidget::clicked,this,&Widget::setrow);
    connect(ui->LType,&QListWidget::clicked,this,&Widget::setrow);
    connect(ui->LOdd,&QListWidget::clicked,this,&Widget::setrow);
    connect(ui->Score,&QListWidget::clicked,this,&Widget::setrow);
    connect(ui->RTeam,&QListWidget::clicked,this,&Widget::setrow);
    connect(ui->RType,&QListWidget::clicked,this,&Widget::setrow);
    connect(ui->ROdd,&QListWidget::clicked,this,&Widget::setrow);
    connect(ui->Winner,&QListWidget::clicked,this,&Widget::setrow);
    connect(ui->Title,&QListWidget::clicked,this,&Widget::setrow);


    std::cout.setf(std::ios::left);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::start()
{
    Timer.stop();
    reply = manager->get(QNetworkRequest(QUrl("http://10.188.2.20:47182/article/"+QString::number(currentUrl)+"/")));
    QByteArray responseData;
    QEventLoop eventLoop;
    connect(manager,SIGNAL(finished(QNetworkReply *)),&eventLoop,SLOT(quit()));
    eventLoop.exec();			// block until finish
    responseData = reply->readAll();
    Data1=QString(responseData);

    qDebug()<<currentUrl;
    if(Data1.indexOf("class=\"page-header\"")>=0){
        qDebug()<<Data1;
    }

    //爬虫部分代码已被注释
    //    if(grap()){
    //        xia=10000;
    //        money-=xia;

    //        xia2=10000;
    //        money2-=xia2;

    //        getLTeam();
    //        getLType();
    //        getLOdd();
    //        getScore();

    //        getRTeam();
    //        getRType();
    //        getROdd();
    //        getWinner();
    //        getTitle();

    //        ui->Money->setText(QString::number(money));
    //        ui->money2->setText(QString::number(money2));
    //        ui->LTeam->setCurrentRow(ui->LTeam->count()-1);
    //        ui->LType->setCurrentRow(ui->LTeam->count()-1);
    //        ui->LOdd->setCurrentRow(ui->LTeam->count()-1);
    //        ui->Score->setCurrentRow(ui->LTeam->count()-1);
    //        ui->RTeam->setCurrentRow(ui->LTeam->count()-1);
    //        ui->RType->setCurrentRow(ui->LTeam->count()-1);
    //        ui->ROdd->setCurrentRow(ui->LTeam->count()-1);
    //        ui->Winner->setCurrentRow(ui->LTeam->count()-1);
    //        ui->Title->setCurrentRow(ui->LTeam->count()-1);

    //        std::cout<<TitleList.last().toStdString()<<"{|}"
    //                <<LTeamList.last().toStdString()<<"{|}"
    //               <<LTypeList.last().toStdString()<<"{|}"
    //              <<LOddList.last().toStdString()<<"{|}"
    //             <<ScoreList.last().toStdString()<<"{|}"
    //            <<RTeamList.last().toStdString()<<"{|}"
    //           <<RTypeList.last().toStdString()<<"{|}"
    //          <<ROddList.last().toStdString()<<"{|}"
    //         <<WinnerList.last().toStdString()<<"{|}"<<currentUrl<<std::endl<<"[|]";

    //    }
    currentUrl++;
    //    if(currentUrl<100)
    //        ok=0;

    Timer.start(50);
}

void Widget::go(){
    // if(ok)
    //     start();
}

void Widget::loadData()
{
    ui->LTeam->clear();
    ui->LType->clear();
    ui->LOdd->clear();
    ui->Score->clear();
    ui->RTeam->clear();
    ui->RType->clear();
    ui->ROdd->clear();
    ui->Winner->clear();
    ui->Title->clear();

    TitleList.clear();
    LTeamList.clear();
    LTypeList.clear();
    LOddList.clear();
    ScoreList.clear();
    RTeamList.clear();
    RTypeList.clear();
    ROddList.clear();
    WinnerList.clear();


    QFile file("1.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QByteArray line = file.readAll();
    QString str(line);

    QStringList list=str.split("[|]");
    list.pop_back();


    QFile file2("2.txt");
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QByteArray line2 = file2.readAll();
    QString str2(line2);
    list.append(str2.split("[|]"));
    list.pop_back();


    QFile file3("3.txt");
    if(!file3.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QByteArray line3 = file3.readAll();
    QString str3(line3);
    list.append(str3.split("[|]"));
    list.pop_back();


    for(int i=0;i<list.length();i++){
        QStringList temp=list[i].split("{|}");
        TitleList.append(temp[0]);
        LTeamList.append(temp[1]);
        LTypeList.append(temp[2]);
        LOddList.append(temp[3]);
        ScoreList.append(temp[4]);
        RTeamList.append(temp[5]);
        RTypeList.append(temp[6]);
        ROddList.append(temp[7]);
        WinnerList.append(temp[8]);


    }
    ui->LTeam->addItems(LTeamList);
    ui->LType->addItems(LTypeList);
    ui->LOdd->addItems(LOddList);
    ui->Score->addItems(ScoreList);
    ui->RTeam->addItems(RTeamList);
    ui->RType->addItems(RTypeList);
    ui->ROdd->addItems(ROddList);
    ui->Winner->addItems(WinnerList);
    ui->Title->addItems(TitleList);


    file.close();
    file2.close();
    file3.close();
    ui->LTeam->setCurrentRow(0);
    ui->LType->setCurrentRow(0);
    ui->LOdd->setCurrentRow(0);
    ui->Score->setCurrentRow(0);
    ui->RTeam->setCurrentRow(0);
    ui->RType->setCurrentRow(0);
    ui->ROdd->setCurrentRow(0);
    ui->Winner->setCurrentRow(0);
    ui->Title->setCurrentRow(0);
}

void Widget::fliteData()
{

    ui->LTeam->clear();
    ui->LType->clear();
    ui->LOdd->clear();
    ui->Score->clear();
    ui->RTeam->clear();
    ui->RType->clear();
    ui->ROdd->clear();
    ui->Winner->clear();
    ui->Title->clear();

    TitleList.clear();
    LTeamList.clear();
    LTypeList.clear();
    LOddList.clear();
    ScoreList.clear();
    RTeamList.clear();
    RTypeList.clear();
    ROddList.clear();
    WinnerList.clear();

    QFile file("1.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QByteArray line = file.readAll();
    QString str(line);

    QStringList list=str.split("[|]");
    list.pop_back();


    QFile file2("2.txt");
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QByteArray line2 = file2.readAll();
    QString str2(line2);
    list.append(str2.split("[|]"));
    list.pop_back();


    QFile file3("3.txt");
    if(!file3.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QByteArray line3 = file3.readAll();
    QString str3(line3);
    list.append(str3.split("[|]"));
    list.pop_back();


    for(int i=0;i<list.length();i++){
        QStringList temp=list[i].split("{|}");
        nn[temp[1]]++;
        nn[temp[5]]++;
    }

    std::set<QString> sss;
    for(int i=0;i<list.length();i++){
        QStringList temp=list[i].split("{|}");

        int ll=temp[4].split(":")[0].toInt();
        int rr=temp[4].split(":")[1].toInt();

        if(temp[2]==""&&temp[6]==""&&ll<5&&rr<5&&nn[temp[1]]>50&&nn[temp[5]]>50){
            sss.insert(temp[1]);
            sss.insert(temp[5]);
            TitleList.append(temp[0]);
            LTeamList.append(temp[1]);
            LTypeList.append(temp[2]);
            LOddList.append(temp[3]);
            ScoreList.append(temp[4]);
            RTeamList.append(temp[5]);
            RTypeList.append(temp[6]);
            ROddList.append(temp[7]);
            WinnerList.append(temp[8].split(" ")[0]);
        }
    }
    ui->LTeam->addItems(LTeamList);
    ui->LType->addItems(LTypeList);
    ui->LOdd->addItems(LOddList);
    ui->Score->addItems(ScoreList);
    ui->RTeam->addItems(RTeamList);
    ui->RType->addItems(RTypeList);
    ui->ROdd->addItems(ROddList);
    ui->Winner->addItems(WinnerList);
    ui->Title->addItems(TitleList);

    QMessageBox::information(0,"训练样例数量：","训练样例数量：\n"+QString::number(WinnerList.length()-ui->lineEdit_9->text().toInt()));
    QMessageBox::information(0,"队伍数量：","队伍数量：\n"+QString::number(sss.size()));

    file.close();
    file2.close();
    file3.close();

    ui->LTeam->setCurrentRow(0);
    ui->LType->setCurrentRow(0);
    ui->LOdd->setCurrentRow(0);
    ui->Score->setCurrentRow(0);
    ui->RTeam->setCurrentRow(0);
    ui->RType->setCurrentRow(0);
    ui->ROdd->setCurrentRow(0);
    ui->Winner->setCurrentRow(0);
    ui->Title->setCurrentRow(0);
}

int Widget::OddMap(double a){
    if(a<0.10)return 1;
    if(a<0.24)return 2;
    if(a<0.37)return 3;
    if(a<0.49)return 4;
    if(a<0.60)return 5;
    if(a<0.70)return 6;
    if(a<0.79)return 7;
    if(a<0.87)return 8;
    if(a<0.94)return 9;
    if(a<1.00)return 10;
    if(a<1.05)return 11;
    if(a<1.09)return 12;
    if(a<1.13)return 13;
    if(a<1.17)return 14;
    if(a<1.21)return 15;
    if(a<1.25)return 16;
    if(a<1.29)return 17;
    if(a<1.33)return 18;
    if(a<1.37)return 19;
    if(a<1.41)return 20;
    if(a<1.45)return 21;
    if(a<1.50)return 22;
    if(a<1.55)return 23;
    if(a<1.60)return 24;
    if(a<1.65)return 25;
    if(a<1.71)return 26;
    if(a<1.78)return 27;
    if(a<1.86)return 28;
    if(a<1.95)return 29;
    if(a<2.05)return 30;
    if(a<2.16)return 31;
    if(a<2.28)return 32;
    if(a<2.41)return 33;
    if(a<2.55)return 34;
    if(a<2.70)return 35;
    if(a<2.86)return 36;
    if(a<3.03)return 37;
    if(a<3.21)return 38;
    if(a<3.40)return 39;
    if(a<3.60)return 40;
    if(a<3.81)return 41;
    if(a<4.03)return 42;
    if(a<4.26)return 43;
    if(a<4.50)return 44;
    if(a<4.75)return 45;
    if(a<5.01)return 46;
    if(a<5.28)return 47;
    if(a<5.56)return 48;
    if(a<5.85)return 49;
    if(a<6.15)return 50;
    if(a<6.46)return 51;
    if(a<6.78)return 52;
    if(a<7.11)return 53;
    if(a<7.45)return 54;
    if(a<7.80)return 55;
    if(a<8.16)return 56;
    if(a<8.53)return 57;
    if(a<8.91)return 58;
    if(a<9.30)return 59;
    return 60;
}
int Widget::ScoreMap(QString a){
    if(a=="0:0")return 1;
    if(a=="1:0")return 2;
    if(a=="0:1")return 3;
    if(a=="1:1")return 4;
    if(a=="2:0")return 5;
    if(a=="0:2")return 6;
    if(a=="2:1")return 7;
    if(a=="1:2")return 8;
    if(a=="2:2")return 9;
    if(a=="3:0")return 10;
    if(a=="0:3")return 11;
    if(a=="3:1")return 12;
    if(a=="1:3")return 13;
    if(a=="3:2")return 14;
    if(a=="2:3")return 15;
    return 16;
}

QString Widget::MapScore(int a){
    if(a==1)return "0:0";
    if(a==2)return "1:0";
    if(a==3)return "0:1";
    if(a==4)return "1:1";
    if(a==5)return "2:0";
    if(a==6)return "0:2";
    if(a==7)return "2:1";
    if(a==8)return "1:2";
    if(a==9)return "2:2";
    if(a==10)return "3:0";
    if(a==11)return "0:3";
    if(a==12)return "3:1";
    if(a==13)return "1:3";
    if(a==14)return "3:2";
    if(a==15)return "2:3";
    if(a==16)return "3:3";
    return "3:3";
}

void Widget::ID3()
{
    int cnt=1;
    for(int i=0;i<LTeamList.size();i++){
        if(!NameMap[LTeamList[i]])
            NameMap[LTeamList[i]]=cnt++;
    }
    for(int i=0;i<RTeamList.size();i++){
        if(!NameMap[RTeamList[i]])
            NameMap[RTeamList[i]]=cnt++;
    }

    vector<int> AttrNum;
    AttrNum.push_back(cnt);
    AttrNum.push_back(61);
    AttrNum.push_back(61);
    AttrNum.push_back(cnt);

    TrainData data;
    vector<int> in;

    for(int i=0;i<LTeamList.size()-ui->lineEdit_9->text().toInt();i++){
        in.clear();
        in.push_back(NameMap[LTeamList[i]]);
        in.push_back(OddMap(LOddList[i].toDouble()));
        in.push_back(OddMap(ROddList[i].toDouble()));
        in.push_back(NameMap[RTeamList[i]]);
        data.InSertData(in,ScoreMap(ScoreList[i]));
    }

    mytree=new Tree(AttrNum,data);
    QMessageBox::information(NULL,"提示","训练完毕");

}

void Widget::testID3()
{
    vector<int> in;
    int sf=0;
    int hit=0;
    for(int i=LTeamList.size()-ui->lineEdit_9->text().toInt();i<LTeamList.size();i++){
        in.clear();
        in.push_back(NameMap[LTeamList[i]]);
        in.push_back(OddMap(LOddList[i].toDouble()));
        in.push_back(OddMap(ROddList[i].toDouble()));
        in.push_back(NameMap[RTeamList[i]]);
        qDebug()<<i;
        qDebug()<<LTeamList[i]<<" "<<LOddList[i]<<" "<<ROddList[i]<<" "<<RTeamList[i]<<" ";
        int temp=mytree->GetOutPut(in);
        qDebug()<<temp;
        if(MapScore(temp)==ScoreList[i])
            hit++;

        if(temp==2||temp==5||temp==7||temp==10||temp==12||temp==14){
            int n=ScoreMap(ScoreList[i]);
            if(n==2||n==5||n==7||n==10||n==12||n==14)
                sf++;
        }
        if(temp==3||temp==6||temp==8||temp==11||temp==13||temp==15){
            int n=ScoreMap(ScoreList[i]);
            if(n==3||n==6||n==8||n==11||n==13||n==15)
                sf++;
        }

    }

    QMessageBox::information(NULL,"比分命中率",QString::number(hit)+"/"+ui->lineEdit_9->text()+"\n"+QString::number(hit/ui->lineEdit_9->text().toDouble()));
    QMessageBox::information(NULL,"胜负命中率",QString::number(sf)+"/"+ui->lineEdit_9->text()+"\n"+QString::number(sf/ui->lineEdit_9->text().toDouble()));

}

void Widget::dantestID3()
{
    vector<int> in;

    if(nn[ui->lineEdit->text()]<50||nn[ui->lineEdit_4->text()]<50){
        QMessageBox::information(0,"提示","队伍不出名！");
        return;
    }

    in.push_back(NameMap[ui->lineEdit->text()]);
    in.push_back(OddMap(ui->lineEdit_2->text().toDouble()));
    in.push_back(OddMap(ui->lineEdit_3->text().toDouble()));
    in.push_back(NameMap[ui->lineEdit_4->text()]);

    int temp=mytree->GetOutPut(in,1);

    QMessageBox::information(NULL,"预测",MapScore(temp));

}

void Widget::ANN()
{
    int cnt=1;
    for(int i=0;i<LTeamList.size();i++){
        if(!NameMap[LTeamList[i]])
            NameMap[LTeamList[i]]=cnt++;
    }
    for(int i=0;i<RTeamList.size();i++){
        if(!NameMap[RTeamList[i]])
            NameMap[RTeamList[i]]=cnt++;
    }

    Data* MyData=new Data(4,16);
    vector<float> input1;
    vector<float> output1;

    for(int i=0;i<LTeamList.size()-1000;i++){
        output1.clear();
        input1.clear();
        for(int j=1;j<=16;j++)
            if(ScoreMap(ScoreList[i])==j)
                output1.push_back(1.0);
            else
                output1.push_back(0.0);

        input1.push_back(double(NameMap[LTeamList[i]])/double(NameMap.size()));
        input1.push_back(double(OddMap(LOddList[i].toDouble()))/61.0);
        input1.push_back(double(OddMap(ROddList[i].toDouble()))/61.0);
        input1.push_back(double(NameMap[RTeamList[i]])/double(NameMap.size()));

        qDebug()<<i<<LTeamList[i]<<LOddList[i]<<ROddList[i]<<RTeamList[i];
        MyData->AddData(input1,output1);//添加数据
    }


    NeuralNet* Brain=new NeuralNet(4,16,4,0.1,NeuralNet::ERRORSUM,true);//新建一个神经网络，输入神经元个数，输出神经元个数，隐藏层神经元个数，学习率，停止训练方法（次数或误差最小），是否输出误差值（用于观察是否收敛）
    Brain->SetErrorThrehold(100);//设置误差，默认0.01
    //Brain->SetCount(10000);设置次数，默认10000
    qDebug()<<"Train!!!!!";
    Brain->Train(MyData);//通过数据，开始训练
    qDebug()<<"Done!!!!!";
    Brain->saveNet("model1111.txt");//保存网络

}

void Widget::testANN()
{
    NeuralNet* Brain=new NeuralNet("model1111.txt");//通过文件读取网络
    vector<float> input1;

    int sf=0;
    int hit=0;
    for(int i=LTeamList.size()-1000;i<LTeamList.size();i++){
        input1.clear();
        input1.push_back(double(NameMap[LTeamList[i]])/double(NameMap.size()));
        input1.push_back(double(OddMap(LOddList[i].toDouble()))/61.0);
        input1.push_back(double(OddMap(ROddList[i].toDouble()))/61.0);
        input1.push_back(double(NameMap[RTeamList[i]])/double(NameMap.size()));

        vector<float> out=Brain->Update(input1);

        vector<pair<float,int> > li;
        for(int j=1;j<=16;j++)
            li.push_back(make_pair(out[j-1],j));
        sort(li.begin(),li.end());
        int temp=li[15].second;

        if(MapScore(temp)==ScoreList[i])
            hit++;

        if(temp==2||temp==5||temp==7||temp==10||temp==12||temp==14){
            int n=ScoreMap(ScoreList[i]);
            if(n==2||n==5||n==7||n==10||n==12||n==14)
                sf++;
        }
        if(temp==3||temp==6||temp==8||temp==11||temp==13||temp==15){
            int n=ScoreMap(ScoreList[i]);
            if(n==3||n==6||n==8||n==11||n==13||n==15)
                sf++;
        }

    }

    QMessageBox::information(NULL,"比分命中率",QString::number(hit)+"/1000\n"+QString::number(hit/1000.0));
    QMessageBox::information(NULL,"胜负命中率",QString::number(sf)+"/1000\n"+QString::number(sf/1000.0));


}

void Widget::dantestANN()
{
    if(nn[ui->lineEdit_5->text()]<50||nn[ui->lineEdit_8->text()]<50){
        QMessageBox::information(0,"提示","队伍不出名！");
        return;
    }
    NeuralNet* Brain=new NeuralNet("model1111.txt");//通过文件读取网络
    vector<float> input1;

    input1.push_back(double(NameMap[ui->lineEdit_5->text()])/double(NameMap.size()));
    input1.push_back(double(OddMap(ui->lineEdit_6->text().toDouble()))/61.0);
    input1.push_back(double(OddMap(ui->lineEdit_7->text().toDouble()))/61.0);
    input1.push_back(double(NameMap[ui->lineEdit_8->text()])/double(NameMap.size()));

    vector<float> out=Brain->Update(input1);

    vector<pair<float,int> > li;
    for(int i=1;i<=16;i++)
        li.push_back(make_pair(out[i-1],i));
    sort(li.begin(),li.end());
    int temp=li[15].second;


    QMessageBox::information(NULL,"预测",MapScore(temp));
}

void Widget::setrow()
{
    int cur=0;
    cur^=ui->LTeam->currentRow();
    cur^=ui->LType->currentRow();
    cur^=ui->LOdd->currentRow();
    cur^=ui->Score->currentRow();
    cur^=ui->RTeam->currentRow();
    cur^=ui->RType->currentRow();
    cur^=ui->ROdd->currentRow();
    cur^=ui->Winner->currentRow();
    cur^=ui->Title->currentRow();

    ui->LTeam->setCurrentRow(cur);
    ui->LType->setCurrentRow(cur);
    ui->LOdd->setCurrentRow(cur);
    ui->Score->setCurrentRow(cur);
    ui->RTeam->setCurrentRow(cur);
    ui->RType->setCurrentRow(cur);
    ui->ROdd->setCurrentRow(cur);
    ui->Winner->setCurrentRow(cur);
    ui->Title->setCurrentRow(cur);

}


void Widget::end()
{
    //ok=0;
}

bool Widget::grap()
{
    if(Data1.indexOf("winBigRight")!=-1||Data1.indexOf("winBigLeft")!=-1)
        return 1;
    else
        return 0;
}

void Widget::getLTeam()
{
    QStringList temp2=Data1.split("style=\"text-align: right; padding-top: 5px;\">");
    temp2=temp2[1].split(" <span class=\"spread\">");

    temp2[0].replace("\n","");
    temp2[0].replace("\t","");
    temp2[0].replace(" ","");
    LTeamList.append(temp2[0]);
    ui->LTeam->addItem(temp2[0]);

}

void Widget::getLType()
{
    QStringList temp2=Data1.split("<span class=\"spread\">");
    temp2=temp2[1].split("</span>");
    temp2[0].replace("\n","");
    temp2[0].replace("\t","");
    temp2[0].replace(" ","");
    LTypeList.append(temp2[0]);
    ui->LType->addItem(temp2[0]);
}

void Widget::getLOdd()
{
    QStringList temp2=Data1.split(" style=\"float: right\">");
    temp2=temp2[1].split("</div>");
    temp2[0].replace("\n","");
    temp2[0].replace("\t","");
    temp2[0].replace(" ","");
    LOddList.append(temp2[0]);
    ui->LOdd->addItem(temp2[0]);
}

void Widget::getScore()
{

    QStringList temp2=Data1.split("<div class=\"small\">");
    temp2=temp2[1].split("</div>");
    temp2[0].replace("\n","");
    temp2[0].replace("\t","");
    temp2[0].replace(" ","");
    ScoreList.append(temp2[0]);
    ui->Score->addItem(temp2[0]);
}

void Widget::getRTeam()
{
    QStringList temp2=Data1.split("<div class=\"big bold\">");
    temp2=temp2[1].split(" <span class=\"spread\">");
    temp2[0].replace("\n","");
    temp2[0].replace("\t","");
    temp2[0].replace(" ","");
    RTeamList.append(temp2[0]);
    ui->RTeam->addItem(temp2[0]);
}

void Widget::getRType()
{
    QStringList temp2=Data1.split("<span class=\"spread\">");
    temp2=temp2[2].split("</span>");
    temp2[0].replace("\n","");
    temp2[0].replace("\t","");
    temp2[0].replace(" ","");
    RTypeList.append(temp2[0]);
    ui->RType->addItem(temp2[0]);
}

void Widget::getROdd()
{
    QStringList temp2=Data1.split(" style=\"float: left\">");
    temp2=temp2[1].split("</div>");
    temp2[0].replace("\n","");
    temp2[0].replace("\t","");
    temp2[0].replace(" ","");
    ROddList.append(temp2[0]);
    ui->ROdd->addItem(temp2[0]);
}

void Widget::getTitle()
{
    QStringList temp2=Data1.split("<span id=\"match_note\" style=\"color: red; font-weight: bold;\">");
    temp2=temp2[1].split("</span>");
    temp2[0].replace("\n","");
    temp2[0].replace("\t","");
    temp2[0].replace(" ","");
    TitleList.append(temp2[0]);
    ui->Title->addItem(temp2[0]);
}

void Widget::getWinner()
{
    bool win;
    if(Data1.indexOf("winBigRight")==-1){
        WinnerList.append("left "+LOddList.last());
        ui->Winner->addItem("left "+LOddList.last());

        win=1;
    }
    else{
        WinnerList.append("right "+ROddList.last());
        ui->Winner->addItem("right "+ROddList.last());
        win=0;
    }
}



