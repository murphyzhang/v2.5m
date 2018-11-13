/*
 * 卡槽皮带运行线程
*/

#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <QObject>
#include <QThread>
#include <QtCore>
#include "xioctl.h"
#include "sysinfo.h"


class CConveyor : public QThread
{
    Q_OBJECT

    typedef enum
    {
        CHECK_RUN,
        STEPN_RUN,
        WAIT_RUN
    }RUNTYPE;

public:
    explicit CConveyor(QObject *parent = 0);
    void init_conveyor();


    bool start_check(); //开始检测
    bool isend_check(); //检测状态 结束true
    bool state_check(); //检测结果


    void run();

    bool detetIRM(); //检测光电开关

    bool run1step();
    bool runnstep(int nstep);

    bool conveyorend();

    int getdetetstep();

    bool verifyrunstep(int dir,int nminsetp);
signals:
    void sigrun1step();
    void sigrunend();
    void sigcheckover();
public slots:

private:

    int m_checkdir;       // 自检方向
    int m_checkspeed;     // 自检速度
    int m_checkstep;      // 自检步数

    int m_rundir;       // 运行方向
    int m_runspeed;     // 运行速度
    int m_runstep;      // 一格步数

    int m_detetstep;   //检测到光偶后走多少

    RUNTYPE m_runtype;

    SPEED_STEP_T m_cvyrun;

    unsigned int m_curstate;
    unsigned int m_prestate;
    int m_runnstep;

    int m_shield;  //每格开始走时 屏蔽检测的次数
    int m_pastshake;    //去抖动 连续检测到稳定的次数
    bool m_bshake;


};

#endif // CONVEYOR_H
