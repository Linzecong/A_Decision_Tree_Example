#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QtNetwork/QNetworkReply>
#include<QtNetwork/QNetworkRequest>
#include<QEventLoop>
#include<QByteArray>
#include<QTimer>
#include<map>
#include"decisiontree.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public:
    /*爬虫部分函数*/
    void getLTeam();
    void getLType();
    void getLOdd();
    void getScore();
    void getRTeam();
    void getRType();
    void getROdd();
    void getWinner();
    void getTitle();
    void start();
    void end();
    bool grap();
    void go();

    /*数据清洗部分*/
    void loadData();
    void fliteData();

    /*ID3部分*/
    void ID3();
    void testID3();
    void dantestID3();

    /*ANN部分*/
    void ANN();
    void testANN();
    void dantestANN();


    map<QString,int> nn;
    void setrow();
    void DT();
public:

    int currentUrl;
    QString Data1;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QTimer Timer;

    QStringList LTeamList;
    QStringList LTypeList;
    QStringList LOddList;
    QStringList ScoreList;
    QStringList RTeamList;
    QStringList RTypeList;
    QStringList ROddList;
    QStringList WinnerList;
    QStringList TitleList;

    std::map<QString,int> NameMap;
    int OddMap(double a);
    int ScoreMap(QString a);
    QString MapScore(int a);

    Tree* mytree;
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
