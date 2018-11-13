#ifndef CINCARD_H
#define CINCARD_H

#include <QObject>
#include <QThread>
#include <QTime>
#include "sysinfo.h"
#include "xioctl.h"
class CInCard : public QThread
{
    Q_OBJECT

    typedef enum
    {
        CHECK_RUN,
        INSER_RUN,
        DELAY_RUN,
        SPACE_RUN,
        WAIT_RUN
    }RUNTYPE;

public:
    explicit CInCard(QObject *parent = 0);
    void init_incard();

    bool start_check(); //开始检测
    bool isend_check(); //检测状态 结束true
    bool state_check(); //检测结果

    void run();

    void set_inallow(bool allow);//设置允许进卡
    bool get_instate(); //查询是否正在进卡
    bool incardend();

    void close_door();
    void open_door();
signals:
    void sigining();  //发现进卡
    void siginsucc(); //进卡完成
public slots:

private:
    int m_indelaytime;  //电机延时停止
    int m_inspacetime;  //进卡间隔时间
    int m_inchecktime;  //检测时间
    int m_inovertime;   //进卡超时时间

    QTime m_curtime;
    QTime m_pretime;

    bool m_chstate;
    RUNTYPE m_runtype;

    bool m_inallow; //内部程序设置是否允许进卡
    bool m_outallow; //外部标记是否允许进卡
    bool m_dcrun;

    int door_pt;

};

#endif // CINCARD_H
