#include "optbuzzer.h"

XOptBuzzer::XOptBuzzer(QObject *parent): QThread(parent)
{
    m_times = 0;
    m_ring = 0;
    m_quiet = 0;
    stop_ring();
}

void XOptBuzzer::run()
{
    while(1)
    {
        QThread::msleep(200);
        if(m_times > 0)
        {
            while(m_times-- > 0)
            {
                int tmpring = m_ring,tmpquiet = m_quiet;
                XIoctl::set_gpdat('g',2,0);
                while(tmpring--)
                {
                    QThread::msleep(1);
                }
                XIoctl::set_gpdat('g',2,1);
                while(tmpquiet--)
                {
                    QThread::msleep(1);
                }
            }
        }
    }
}

void XOptBuzzer::start_ring(int times, int ringtime, int quiettime)
{
    m_ring = ringtime;
    m_quiet = quiettime;
    m_times = times;
}

void XOptBuzzer::stop_ring()
{
    m_ring = 0;
    XIoctl::set_gpdat('g',2,1);
}

void XOptBuzzer::ringdi()
{
    start_ring(1,30,10);
}

void XOptBuzzer::ringdidi()
{
    start_ring(2,30,10);
}

void XOptBuzzer::ringdidididi()
{
    start_ring(4,30,10);
}

void XOptBuzzer::ringdiii()
{
    start_ring(1,80,10);
}

