#ifndef DATACOM_H
#define DATACOM_H

#include <QObject>
#include <QThread>

#include <stdio.h>
#include <QByteArray>
#include <QTimer>
#include <QtCore>
#include <QObject>
#include <QMutex>

#include "packet_def.h"
//#include "SerialPort/SerialPortDialog.h"
#include "SerialPort/SerialPort.h"
#include "SerialPort/qextserialport.h"

#include "DataBase.h"
#include "argdecode.h"
#include "sysinfo.h"

#if 1
#   define START_PCK_STATE   0xFEFCFAF8
#   define END_PCK_STATE     0xFDFBF9F7
#endif

#define FRAME_MAX_LEN (512)

typedef enum ComType //通讯类型
{
    COM_NONE = 0x00000000, //
    COM_CNT = 0x00000001, //连接信号    1
    COM_HBT = 0x00000002, //心跳信号    2
    COM_GET = 0x00000010, //获取类型    16
    COM_SET = 0x00000020, //设置类型    32
    COM_CMD = 0x00000100, //命令模式    256
    COM_UPG = 0x00000200, //升级模式    512
    COM_RPT = 0x00001000  //提交报告    4096
}COM_TYPE;



typedef enum DataType //数据类型
{
    DATA_NONE = 0x00000000, //

    DATA_FLUO = 0x00000001, //荧光数据
    DATA_BARC = 0x00000002, //条码数据
    DATA_HIST = 0x00000004, //历史数据
    DATA_PARA = 0x00000008, //当前参数
    DATA_VERS = 0x00000010, //版本信息
    DATA_IFEI = 0x00000020, //唯一识别码
    DATA_TIME = 0x00000040, //时间
    DATA_SPCE = 0x00000080, //存储空间

    DATA_STAT = 0x00000100, //启动测试
    DATA_STOP = 0x00000200, //终止测试
    DATA_REST = 0x00000400, //重启系统
    DATA_RECO = 0x00000800, //恢复出厂
    DATA_TLAB = 0x00001000, //实验室
    DATA_DELN = 0x00002000, //删除

    DATA_BEGN = 0x00010000, //测试开始
    DATA_TSSP = 0x00020000, //测试结束
    DATA_DTER = 0x00040000, //检测出错
    DATA_NOCA = 0x00080000, //未检测到卡
    DATA_QRER = 0x00100000, //条码有错
    DATA_CUTS = 0x00200000, //当前正在测

    DATA_CARM = 0x01000000, //升级ARM
    DATA_C430 = 0x02000000, //升级430
    DATA_CCFG = 0x04000000, //升级配置文件

    DATA_BREK = 0x10000000, //连接
    DATA_CNNT = 0x20000000, //断开
    DATA_DERR = 0x40000000, //数据出错
    DATA_SUCC = 0x80000000  //成功
}DATA_TYPE;

typedef struct DelPackage
{
    int com;              //类型
    int pos;              //地址
    int num;              //个数
}DEL_PACKAGE;


typedef struct DataCom
{
    int head;               //包头
    int comT;               //通信类型
    int dataT;              //数据类型
    int packN;              //数据包总数
    int currN;              //当前包号数
    int data_length;        //当前包长度
    QByteArray data;        //数据
    int CRC_BT;             //校验值
    int end;                //包结束
}DATA_COM;

typedef struct File_Struct
{
    int name_len;
    QByteArray file_name;
    int data_len;
    QByteArray file_data;
    QByteArray file_MD5;
}FILE_ST;

typedef struct Send_Struct
{
    COM_TYPE comT;
    DATA_TYPE dataT;
    QByteArray data;
}SEND_ST;

class CDataCom : public QThread
{
    Q_OBJECT
public:
    typedef enum
    {
        COM_RUN,
        WAIT_RUN
    }RUNTYPE;
    explicit CDataCom(QObject *parent=0);
    //~CDataCom();
    void run();
    QByteArray  intToByte(int i);
    int ByteToint(QByteArray ba);
    int getCRC_BT(DATA_COM *data);//获取校验码
    bool SendData(QByteArray &data, COM_TYPE comT, DATA_TYPE dataT);
    bool orgSendData(DATA_COM *send,QByteArray *data, COM_TYPE comT, DATA_TYPE dataT,int packn=1,int currn=0);//组织发送数据

    bool anaRecvData(DATA_COM *data);//分析接收数据
    bool DataToSend(DATA_COM * data, QByteArray *send);
    bool RecvToData(DATA_COM * data, QByteArray *recData);

    bool FileToStruct(QByteArray &name,QByteArray &data,FILE_ST *file);
    bool StructToSend(FILE_ST &file,QByteArray *send);
    bool FileToSend(QByteArray &name,QByteArray &data,QByteArray *send);
    bool DataToStruct(QByteArray &data,FILE_ST *file);
    bool isSTFileTrue(FILE_ST &file);

    bool orgHistData(int ser,QByteArray *data);     //组织发送历史包
    void orgHistDatas(int from, int to);   //组织发送历史包

    bool RecvConnect(int dtp); //收到连接信号
    bool RecvHeartBeat();//收到心跳信号

    bool ConnectToSerPort();
    bool BreakToSerPort();

    bool isBuildCommt();//获得上位机与盒子的通讯是否建立

    bool RecvSetCom(DATA_COM *data);
    bool RecvCmdCom(DATA_COM *data);
    bool RecvGetCom(DATA_COM *data);
    bool RecvUpgCom(DATA_COM *data);

    void SlotRdLoop(); // 接收数据
    void SlotWdLoop(); // 发送数据

private:
    bool getBeginEnd(int * pBegin, int * pEnd, const char * dbTable);

    bool ResponseSucc(COM_TYPE comT);//应答正确
    bool ResponseErr(COM_TYPE comT);//应答错误
    bool subPackageData(SEND_ST * ts);//分包

private slots:

signals:
    //    void SigUpSysStatus(QString status);
    void SigCnntSucc(bool bsucc); //连接信号
    void SigNewData(QStringList hisdata);

    void SigNewBatch(QString batch);

private:

    RUNTYPE m_runtype;

    bool b_COMConnect;//连接标志

    QByteArray m_Datahead;
    QByteArray m_Dataend;
    /*
    QTimer *m_pRdTm; // 读取数据定时器
    QTimer *m_pWdTm; // 发送数据定时器
    */

    QextSerialPort * m_pSerialPort;
    //CSerialPortDialog *m_pSerialPortDialog; // 串口连接

    QByteArray m_qByData;//串口接收缓冲器
    int m_recd ;//接收多少数据
    bool m_ReNewPack;
    QList<SEND_ST *> m_qSdData;//待发送数据
    //QByteArray m_qSdData;//待发送数据

    QList<DATA_COM *> m_qSdBuff;  //发送队列

    QStringList m_HisData;

    QByteArray m_recvcode;

    QTime m_beforetime;

    QMutex mutex;

    char m_recvBuf[1024];

    unsigned int m_sleep;
    unsigned int myJiffy;
};

#endif // DATACOM_H
