#include "cincard.h"


CInCard::CInCard(QObject *parent): QThread(parent)
{
//    qDebug()<<"---------init CInCard-------------";
    m_runtype = WAIT_RUN;
    m_chstate = false;
    m_inallow = false;
    m_outallow = false;

    m_dcrun = false;
    init_incard();
}

void CInCard::init_incard()
{
    XIoctl::InitDcMotor();

    XIoctl::InitOPCard();

    XIoctl::InitLED();
    XIoctl::SetLEDUNAllow();
    m_indelaytime = CSysInfo::get_cfg(CFG_JC_TIMEDLY).toInt();
    m_inspacetime = CSysInfo::get_cfg(CFG_JC_TIMESPE).toInt();
    m_inchecktime = CSysInfo::get_cfg(CFG_JC_TIMEON).toInt();
    m_inovertime = CSysInfo::get_cfg(CFG_JC_TIMEOVR).toInt();

    //door_pt = open("/dev/beep", 0);
}

bool CInCard::start_check()
{
    if(m_runtype != WAIT_RUN)
        return false;

    m_curtime = QTime::currentTime();
    m_pretime = QTime::currentTime();

    //qDebug()<<"incard CHECK_RUN m_curtime = "<<m_curtime;
    XIoctl::SetDcMotorRun();
    m_runtype = CHECK_RUN;

    return true;
}

bool CInCard::isend_check()
{
    //qDebug()<<"incard CHECK_RUN m_runtype = "<<m_runtype;
    if(m_runtype == WAIT_RUN)
        return true;

    return false;
}

bool CInCard::state_check()
{
    //qDebug()<<"incard CHECK_RUN m_chstate = "<<m_chstate;
    return m_chstate;
}

void CInCard::run()
{
    int deley = 0;

    while(1)
    {
        QThread::usleep(1000);

        switch(m_runtype)
        {
        case CHECK_RUN:
        {
            if(XIoctl::GetOPCardData() == 1) //开机检测时 检测口有卡 进卡延时
            {
                m_curtime = QTime::currentTime();
            }

            if(m_curtime.msecsTo(QTime::currentTime()) > m_inchecktime) //电机继续运行一段时间检测
            {
                qDebug()<<"incard CHECK_RUN stop";
                XIoctl::SetDcMotorStop();
                m_runtype = WAIT_RUN;
                m_chstate = true;
            }
            else if(m_pretime.msecsTo(QTime::currentTime()) > m_inovertime) //一直检测到有卡超过规定时间说明检测口有问题
            {
                qDebug()<<"incard CHECK_RUN"<<m_pretime<<QTime::currentTime()<<m_inovertime;
                XIoctl::SetDcMotorStop();
                m_runtype = WAIT_RUN;
                m_chstate = false;
            }
        }break;
        case INSER_RUN:
        {
            if(m_outallow && !m_inallow && !m_dcrun && (XIoctl::GetOPConveyorData() == 1))//外部允许进样 且 内部未设置允许进样 且 电机未运行
            {
                XIoctl::SetLEDAllow();
                m_inallow = true; //内部允许进样
                open_door();
            }

            if(XIoctl::GetOPCardData() == 1)
                deley++;
            else
                deley = 0;

            if(m_inallow && !m_dcrun && (deley >= 50))//内部允许进样 且 电机没运行 且 检测到卡
            {
                XIoctl::SetLEDUNAllow();
                m_inallow = false; //内部不允许进样
                m_dcrun = true;    //标记电机运行
                XIoctl::SetDcMotorRun();    //驱动电机运行

                qDebug() << "in card ..." << QTime::currentTime().toString("hh:mm:ss.zzz");
                emit sigining();    //发出进卡信号

                QThread::msleep(2000);
            }

            if(m_dcrun && (deley == 0))//电机在运行 且 光耦未检测到  (说明卡已经完全进入)
            {
                m_curtime = QTime::currentTime();
                m_runtype = DELAY_RUN;//处于延时进卡状态
                close_door();
            }

            if(!m_outallow && !m_dcrun)//外部不允许进卡 且 电机未运行
            {
                XIoctl::SetLEDUNAllow();
                m_runtype = WAIT_RUN;//处于运行等待状态
            }
        }break;
        case DELAY_RUN://电机正在进卡中
        {
            if(m_curtime.msecsTo(QTime::currentTime()) > m_indelaytime) //电机继续运行一段时间
            {
                m_runtype = SPACE_RUN;
                m_curtime = QTime::currentTime();
                XIoctl::SetDcMotorStop();
                m_dcrun = false;

                qDebug() << "in card done" << QTime::currentTime().toString("hh:mm:ss.zzz");
                emit siginsucc(); //发出进卡完成消息
            }
        }break;
        case SPACE_RUN://线程空转
        {
            if(m_curtime.msecsTo(QTime::currentTime()) > m_inspacetime) //进卡时间间隔等待
            {
                m_runtype = WAIT_RUN;
            }
        }break;
        case WAIT_RUN://等待进卡
        {
            if(m_outallow)//外部允许进卡
            {
                m_inallow = false; //内部未设置允许进样
                m_runtype = INSER_RUN;
            }
        }break;
        default:
            break;
        }

    }
}

void CInCard::set_inallow(bool allow)
{
    m_outallow = allow;
}

bool CInCard::get_instate()
{
    if(m_dcrun)
        return true;
    else
        return false;
}

bool CInCard::incardend()
{
    if(m_dcrun)
        return false;
    else
        return true;
}

void CInCard::close_door()
{
    //ioctl(door_pt, 1, 10);
    /*
    int irun=50;
    qDebug()<<"close_door ";
    while(irun--)
    {
        XIoctl::set_gpdat('f',4,1);
        QThread::usleep(10000);
        XIoctl::set_gpdat('f',4,0);
        QThread::usleep(90000);
    }
    */
}

void CInCard::open_door()
{
    //ioctl(door_pt, 1, 50);
    /*
    int irun=50;
    qDebug()<<"open_door ";
    while(irun--)
    {
        XIoctl::set_gpdat('f',4,1);
        QThread::usleep(50000);
        XIoctl::set_gpdat('f',4,0);
        QThread::usleep(50000);
    }
    */
}






