#ifndef DETECTION_H
#define DETECTION_H

#include <QObject>
#include <QThread>
#include "SerialPort/SerialPort.h"
#include "xioctl.h"
#include "sysinfo.h"
#include "common/global_define.h"
#include "Formula.h"


//#define YG_VALID_LEN    (DATA_MAX_SIZE * sizeof(unsigned int))   //荧光值有效长度 DATA_MAX_SIZE 个int

#define AD_TIMEOUT  (16000) //采集超时时间


//后台检测
class CDetection : public QThread
{
    Q_OBJECT
public:
    typedef enum
    {
        CHECK_RUN,
        WAIT_RUN
    }RUNTYPE;

    typedef enum
    {
        WRIT_COM,
        READ_COM,
        WAIT_COM
    }COMTYPE;

    typedef enum
    {
        WAITN_TYPE,
        CHECK_TYPE,     //自检
        DETET_TYPE,     //通知采集
        DTEND_TYPE,     //通知采集结束
        DATAN_TYPE,     //MSP430发送数据给ARM 荧光值
        REDOK_TYPE,     //接收成功  05
        RDFAIL_TYPE,    //接收失败  06
        ERR_TYPE,       //错误代码
        COM_STATE_TYPE, //检查通讯
        CARD_SLOT_VERIFY//卡槽校正值
    }DATATYPE;

    explicit CDetection(QObject *parent = 0);
    void init_detection();

    bool start_check(); //开始检测
    bool isend_check(); //检测状态 结束true
    bool state_check(); //检测结果

    void run();

    void dispose_fldate();
    void detetstart();
    bool detetend();
    void getfldata(unsigned int *data);
    unsigned int * getfldata();

    void checkCom();//检查通讯
    bool checkComResult();//检查通讯结果

    void updateCardSlotVerify();
    bool isCardSlotVerify();

    void cancelResend();

private:    
    bool comm_430cmd(DATATYPE type, char * pcData = NULL);
    bool comm_430recv();
    bool comm_430overtime();

    void convert1PkgFluData(int pkgIndex, char * pData);

signals:
    void sigstartsucc();//通知启动测试成功
    void sigrecvsucc(); //通知荧光接收已经完成

    void sigRecvErr(int);

//    void sigRecv1Pkg(uint start, uint * pData, uint len);

public slots:

private:
    RUNTYPE m_runtype;
    int m_resetpos; //复位时检测模块的位置 里面 0 ，外面 1
    int m_routestep;  //一个行程有几步
    int m_resetspeed; //复位时速度
    int m_detetspeed; //运行扫描时速度
    STEP_RUN_INFO_T m_detetrun;
    int m_runstate;

    COMTYPE m_comtype;
    DATATYPE m_datatype;

    QextSerialPort *m_pUart430; // 串口指针

    bool m_ReNewPack;
    QByteArray m_baBuf;//串口接收缓冲器
    int m_recd ;//接收多少数据

    bool ackSelfCheck;
    ERR_NO errNo;

    DATATYPE m_spondata; //发起通信类型
    QTime m_curtime; //当前时间
    int m_overtime;  //通信超时时间 ms
    int m_repeat;    //重复通信次数 -1表示直到通信成功

//    QTime m_beforetime;

    QByteArray m_fldata; //荧光接收数据
    QList<unsigned int> m_flint;  //荧光处理后数据

    unsigned int m_auFlData[DATA_MAX_SIZE];
    char m_caRecvBuf[512];

    bool comState;  //true:通讯成功; false:通讯失败
    bool m_cardSlotVerify;

    int m_jiffies;
};

#endif // DETECTION_H
