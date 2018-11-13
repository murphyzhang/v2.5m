#ifndef TESTCARD_H
#define TESTCARD_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
#include "argdecode.h"
#include "Formula.h"
#include "global_define.h"

class XTestCard : public QObject
{
    Q_OBJECT
public:
    explicit XTestCard();
    explicit XTestCard(int ser);
    ~XTestCard();
    void initcard();
    void setsernum(int ser);
    int getsernum();
    bool setbatch(QString batch);   //设置批号
    QString getbatch();             //获取批号
    bool hasbatch();
    bool canscan();
    void setnoscan();

    bool setcode(QByteArray code); //设置条码
    bool hascode();
    XArgDecode *getargdecode();     //获取解析后到信息
    Pn_INFO *getpninfo(int pro);    //获取项目计算参数
    PF_DATA *getpfinfo(int pro);    //获取项目常规参数

    int getawaytime();  //获得已经插入多久
    int getsurptime();  //获得还剩多少时间

    void setfldata(unsigned int *data); //设置荧光数据
    CFormula* getformula();

    bool cantest();

    void settestend(bool end = true);
    bool gettestend();

    QDateTime getinserttime();
    QDateTime getendtime();
    void setendtime(QDateTime time);
    void setsavepos(int pos);
    int getsavepos();


    void setstatestr(QString str);
    QString getstatestr();

    bool getneedtest();
    void setneedtest(bool need = false);


    RES_DATA* getresdata();
    void setresdata(int at,QString res);
    void truntoresdata();

    void setnotes(QString qStrID);
    void setnotes(QString qStrID, QString qStrName, QString qStrSex, QString qStrAge, QString qStrSmpNum, QString qStrSmpType);
    QString getnotes();
    void setupdatedtime(QDateTime time);
    QDateTime getupdatedtime();
    void SetAutoID(bool bAuto);
    bool getAutoID();
    QString GetPatientID();

    QString id;

signals:
    void sigreadytest(int ser);
    void sigstarttest(int ser);
public slots:
    void slot1sec();

public:
    //患者信息
    bool m_bIsSave;
    QString m_sID;
    QString m_sName;
    QString m_sSex;
    QString m_sAge;
    QString m_sSmpNum;
    QString m_sSmpType;

private:
    int m_sernum;          //卡编号
    QTimer m_timer;        //定时器 用于时间判断
    QDateTime m_inserttime;     //进卡时间
    int m_awaytime;         //插入多久
    int m_surplustime;      //剩余时间
    bool m_scancode;        //是否扫描条码
    bool m_hasbatch;        //是否有批号
    QString m_batch;        //批号信息
    bool m_hascode;         //是否有条码
    QByteArray m_code;      //对应条码
    XArgDecode m_decode;    //解析条码
    bool m_testend;         //是否已经测试
    QDateTime m_endtime;    //测试结束时间
    CFormula m_formula;     //
    int m_savepos;          //存储位置
    QString m_statestr;
    QStringList m_detetstate;
    bool m_needtest;        //标记是否测试
    QString m_stres[5];

public:
    RES_DATA m_res;
    bool m_bIsAutoID;       //标记是否需要ID自增
};

#endif // TESTCARD_H
