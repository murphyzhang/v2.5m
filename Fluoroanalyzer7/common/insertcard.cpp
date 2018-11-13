#include "insertcard.h"

CInsertCard::CInsertCard(QObject *parent): QThread(parent)
{
    m_inserttime = QDateTime::currentDateTime();
    m_newinsert = false;
}

void CInsertCard::run()
{
    while(1)
    {
        QThread::msleep(10);

        if((m_newinsert==false) && get_isinsert())
        {
            m_newinsert = true;
            m_inserttime = QDateTime::currentDateTime();

            emit SigInsertCard(true);
        }
        else if((m_newinsert == true) && !get_isinsert())
        {
            m_newinsert = false;

            emit SigInsertCard(false);
        }
    }
}

QDateTime CInsertCard::get_inserttime()
{
    return m_inserttime;
}

int CInsertCard::get_inserthowsec()
{
    //return QDateTime::currentDateTime().secsTo(m_inserttime);
    return m_inserttime.secsTo(QDateTime::currentDateTime());
}

bool CInsertCard::get_isinsert()
{
    if(XIoctl::get_gpdat('g',4) == 1)
        return false;
    else
        return true;
}
