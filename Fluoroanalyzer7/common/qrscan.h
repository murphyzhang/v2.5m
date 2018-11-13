#ifndef QRSCAN_H
#define QRSCAN_H

#include <QObject>
#include <QThread>
#include <QSettings>
#include <QTime>
#include "SerialPort/SerialPort.h"
#include "sysinfo.h"

#define SCAN_STX 0x02
#define SCAN_ETX 0x03

class CQrScan : public QThread
{
    Q_OBJECT

    typedef enum
    {
        COM_00H = 0X00, //Send configuration data
        COM_01H = 0X01, //Send and save configuration data
        COM_02H = 0X02, //Send Status Bytes
        COM_03H = 0X03, //Specify device to communicate
        COM_04H = 0X04, //PC Download to PKB
        COM_05H = 0X05, //PKB Upload to PC
        COM_06H = 0X06, //Send Version Data
        COM_80H = 0X80, //PC Upload
        COM_81H = 0X81, //PC Download
        COM_82H = 0X82, //Trigger
        COM_83H = 0X83, //Get Status Command
        COM_84H = 0X84, //ISP Command
        COM_85H = 0X85  //Read Version Data Command
    }SCAN_COMID;

    typedef enum
    {
        RP_NAN = 0x00,  //无回复
        RP_ACK = 0x06,  //OK
        RP_NAK = 0x15   //ERR
    }REPLY;

    typedef enum
    {
        SCAN_RUN,
        WAIT_RUN
    }RUNTYPE;
public:
    explicit CQrScan(QObject *parent = 0);

    void init_qrscan();

    bool start_check(); //开始检测
    bool isend_check(); //检测状态 结束true
    bool state_check(); //检测结果

    void run();

    bool start_scan();
    bool end_scan();
    bool qrscanend();
    bool getqrcode(QString *qrcode);

    char getxordata(QByteArray data);
signals:
    void sigqrscanend();
public slots:
private:
    QextSerialPort *m_pUartscan; // 串口指针
    QByteArray m_qByData;//串口接收缓冲器
    int m_recd ;//接收多少数据
    bool m_ReNewPack;
    bool m_waitreply; //等待回复
    REPLY m_reply;

    RUNTYPE m_runtype;
    int m_qrnum;
    int m_overtime;  //条码检测超时时间
    QTime m_curtime;
    bool m_scansucc;

    QString m_qrcode;
};

#endif // QRSCAN_H
