#include "conveyor.h"
#include <QSettings>

CConveyor::CConveyor(QObject *parent) :
    QThread(parent)
{
//    qDebug()<<"---------init CConveyor-------------";
    m_runtype = WAIT_RUN;
    m_curstate = 0;
    m_prestate = 0;
    m_runnstep = 0;
    m_bshake = false;
    m_pastshake = 0;
    m_shield = 0;
    init_conveyor();

}

void CConveyor::init_conveyor()
{

    XIoctl::InitMotor2();
    XIoctl::InitOPConveyor();

    m_rundir = CSysInfo::get_cfg(CFG_PD_RUNDIR).toInt();
    m_checkdir = m_rundir;
    m_runspeed = CSysInfo::get_cfg(CFG_PD_RUNSPE).toInt();
    m_checkspeed = m_runspeed;
    m_detetstep = CSysInfo::get_cfg(CFG_PD_RUNACC).toInt();
    m_checkstep = m_detetstep;
    m_runstep = CSysInfo::get_cfg(CFG_PD_INTERV).toInt();
}



bool CConveyor::start_check()
{
    /*
    if(m_runtype != WAIT_RUN)
        return false;
    if(TIMER_IN_USED == XIoctl::TimeState())
        XIoctl::StopTime();

    m_cvyrun.nDirect = m_rundir;
    m_cvyrun.nSpeed = m_checkspeed;
    m_cvyrun.nStepCount = m_checkstep;
    */
    m_runtype = CHECK_RUN;

    return true;
}

bool CConveyor::isend_check()
{
    if(m_runtype == WAIT_RUN)
        return true;
    return false;
}

bool CConveyor::state_check()
{
    return true;
}


void CConveyor::run()
{
    while(1)
    {
        //QThread::usleep(100);

        switch(m_runtype)
        {
        case CHECK_RUN:
        {
            /*
            if(TIMER_UN_USED == XIoctl::TimeState())
                XIoctl::Motor2RunN(m_cvyrun);
            if(detetIRM())
            {
                m_runtype = WAIT_RUN;
                emit sigcheckover();
            }
            */
            m_runnstep = 18;
            m_runtype = STEPN_RUN;

        }break;
        case STEPN_RUN:
        {
#if 1
            if(m_shield > 50) //屏蔽检测的次数
            {
                m_curstate = XIoctl::GetOPConveyorData();//卡槽运行步数检测光耦

                if(m_bshake)
                {
                    if(m_pastshake<=0)
                    {
                        //qDebug()<<"------------------1";
                        m_bshake = false;
                        XIoctl::StopTime();
                        m_runnstep--;
                        m_shield = 0;
                        emit sigrun1step();

                        //break;
                    }

                    if(m_curstate == 1)
                    {
                        m_pastshake--;
                    }
                }
                else
                {
                    if((m_curstate == 1) && (m_prestate == 0))
                    {
                        //qDebug()<<"------------------0";

                        m_bshake = true;
                        m_pastshake = 5;
                    }

                    m_prestate = m_curstate;
                }

            }
#else
            if(m_pastshake > 50)
            {
                m_curstate = XIoctl::GetOPConveyorData();
                if((m_curstate == 1) && (m_prestate == 0))
                {
                    qDebug()<<"------------------0";
                    m_pastshake = 0;
                    XIoctl::StopTime();
                    m_runnstep--;
                    emit sigrun1step();

                }
                m_prestate = m_curstate;
            }
#endif
            if(TIMER_UN_USED == XIoctl::TimeState())
            {
                if(m_runnstep>0)
                {
                    m_shield++;
                    m_cvyrun.nDirect    = m_rundir;
                    m_cvyrun.nSpeed     = m_checkspeed;
                    m_cvyrun.nStepCount = m_checkstep;
                    XIoctl::Motor2RunN(m_cvyrun);
                }
                else
                {
                    emit sigrunend();
                    //QThread::usleep(100000);
                    m_runtype = WAIT_RUN;
                    break;
                }
            }
            /*
            if(detetIRM())
            {
                m_runnstep--;
                emit sigrun1step();
            }
            */
            QThread::usleep(100);
        }break;
        case WAIT_RUN:
        {
            QThread::usleep(1000);
            if(m_runnstep>0)
            {
                m_runtype = STEPN_RUN;
            }
        }break;
        default:
            break;
        }
    }
}

bool CConveyor::detetIRM()
{
    m_curstate = XIoctl::GetOPConveyorData();
    if((m_curstate == 0) && (m_prestate == 1))
    {
        XIoctl::StopTime();
        /*
        if(m_detetstep > 0)
        {
            m_cvyrun.nDirect = m_rundir;
            m_cvyrun.nSpeed = m_checkspeed;
            m_cvyrun.nStepCount = m_detetstep;
            XIoctl::Motor2RunN(m_cvyrun);
        }
        */
        m_prestate = m_curstate;
        return true;
    }
    m_prestate = m_curstate;
    return false;
}

bool CConveyor::run1step()
{
    m_runnstep++;
    return true;
}

bool CConveyor::runnstep(int nstep)
{
    m_runnstep += nstep;
    return true;
}

bool CConveyor::conveyorend()
{
    if(m_runtype == WAIT_RUN)
        //if(TIMER_UN_USED == XIoctl::TimeState())
            return true;

    return false;
}

int CConveyor::getdetetstep()
{
    return m_detetstep;
}



bool CConveyor::verifyrunstep(int dir, int nminsetp)
{
    if(TIMER_UN_USED == XIoctl::TimeState())
    {
        m_cvyrun.nDirect = dir;
        m_cvyrun.nSpeed = m_runspeed;
        m_cvyrun.nStepCount = nminsetp;
        XIoctl::Motor2RunN(m_cvyrun);
        return true;
    }
    else
        return false;
}


