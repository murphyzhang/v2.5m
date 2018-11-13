#include "detection.h"


CDetection::CDetection(QObject *parent) :
    QThread(parent)
{
    //qDebug()<<"---------init CDetection-------------";
    m_runtype = WAIT_RUN;
    m_runstate = 0;
    m_datatype = WAITN_TYPE;
    m_comtype = WAIT_COM;

    init_detection();
}

void CDetection::init_detection()
{
    XIoctl::InitMotor1();
    m_resetpos = CSysInfo::get_cfg(CFG_FY_DETEDIR).toInt();
    m_routestep = CSysInfo::get_cfg(CFG_FY_TRAVEL).toInt();
    m_detetspeed = CSysInfo::get_cfg(CFG_FY_DETESPE).toInt();
    m_resetspeed = CSysInfo::get_cfg(CFG_FY_RESTSPE).toInt();

    if(!CSerialPort::OpenSerialPort(CSerialPort::UART_3_430))
    {
        //qDebug()<<"*********430 SerialPort init ERR********";
    }
    m_pUart430 = CSerialPort::GetSerialPort(CSerialPort::UART_3_430); // 串口指针

}

bool CDetection::start_check()
{
    if(m_runtype != WAIT_RUN)
        return false;

    if(TIMER_IN_USED == XIoctl::TimeState())
        XIoctl::StopTime();

    if(m_resetpos == 0)
    {
        m_detetrun.nCount = 2;
        m_detetrun.astSpeedSteps[0].nDirect = 0;
        m_detetrun.astSpeedSteps[0].nSpeed = m_resetspeed;
        m_detetrun.astSpeedSteps[0].nStepCount = m_routestep+200;
        m_detetrun.astSpeedSteps[1].nDirect = 1;
        m_detetrun.astSpeedSteps[1].nSpeed = m_resetspeed;
        m_detetrun.astSpeedSteps[1].nStepCount = m_routestep;
    }
    else
    {
        m_detetrun.nCount = 1;
        m_detetrun.astSpeedSteps[0].nDirect = 0;
        m_detetrun.astSpeedSteps[0].nSpeed = m_resetspeed;
        m_detetrun.astSpeedSteps[0].nStepCount = m_routestep+200;
    }

    //qDebug()<<"detection motor count = "<<m_detetrun.nCount;
    //    qDebug()<<"step1 ("
    //           <<m_detetrun.astSpeedSteps[0].nDirect
    //          <<m_detetrun.astSpeedSteps[0].nSpeed
    //         <<m_detetrun.astSpeedSteps[0].nStepCount
    //        <<")";
    //    qDebug()<<"step2 ("
    //           <<m_detetrun.astSpeedSteps[1].nDirect
    //          <<m_detetrun.astSpeedSteps[1].nSpeed
    //         <<m_detetrun.astSpeedSteps[1].nStepCount
    //        <<")";

    XIoctl::Motor1RunN(m_detetrun);
    XIoctl::Motor1Stop();
    XIoctl::Motor1RunN(m_detetrun);
    m_runstate = 0;
    m_runtype = CHECK_RUN;

    comm_430cmd(CHECK_TYPE);
    return true;
}

bool CDetection::isend_check()
{
    if(m_runtype == WAIT_RUN)
    {
        XIoctl::Motor1Stop();
        return true;
    }

    return false;
}

bool CDetection::state_check()
{
    if(m_runstate == 2)
        return true;
    return false;
}

void CDetection::run()
{
    while(1)
    {
        QThread::usleep(100);

        switch(m_runtype)
        {
        case CHECK_RUN:
            QThread::usleep(1000);
            if(TIMER_UN_USED == XIoctl::Motor1IsRun())
            {
                m_runstate++;
                m_runtype = WAIT_RUN;
            }
            break;
        case WAIT_RUN:
            QThread::usleep(1000);
            break;
        default:
            break;
        }

        switch(m_comtype)
        {
        case WRIT_COM:
            break;
        case READ_COM:
            comm_430recv();
            comm_430overtime();
            break;
        case WAIT_COM:
            QThread::usleep(1000);
            break;
        default:
            break;
        }
    }
}
//type:命令类型
bool CDetection::comm_430cmd(CDetection::DATATYPE type)
{
    m_spondata = type;
    m_curtime = QTime::currentTime();
    m_ReNewPack = true;

    QByteArray array;
    switch(type)
    {
    case CHECK_TYPE:        
        m_overtime = 500;
        array.append("1#");
        break;
    case DETET_TYPE:        
        m_overtime = 500;
        array.append("2#");
        m_fldata.clear();
        break;
    case DTEND_TYPE:        
        m_overtime = 500;
        array.append("3#");
        break;
    case DATAN_TYPE:        
        array.append("4#");
        break;
    case REDOK_TYPE:        
        m_overtime = 180000;//0xffffffff;
        array.append("5#");
        break;
    case RDFAIL_TYPE:
        m_overtime = 180000;//0xffffffff;
        array.append("6#");        
        break;
    case ERR_TYPE:
        break;
    case COM_STATE_TYPE: //检查通讯
        m_overtime = 500;
        array.append("8#");
        break;
    case CARD_SLOT_VERIFY:
    {
        m_overtime = 500;

        int verify = CSysInfo::get_cfg(CFG_QT_CSVERIFY).toInt();        
        if (verify < -200 || 650 < verify)
            verify = 0;

        char * pcVerify = (char *)&verify;
#if 0
        array.append("92");
        array.append(pcVerify[0]);
        array.append(pcVerify[1]);
        array.append("00000");
        array.append(0xFF);
#else
        array.append(0x09);
        array.append(0x02);
        array.append(pcVerify[0]);
        array.append(pcVerify[1]);
        array.append((char)0);
        array.append((char)0);
        array.append((char)0);
        array.append((char)0);
        array.append(0xFF);
#endif
        m_cardSlotVerify = false;
        break;
    }
    default:
        break;
    }

    m_pUart430->flush();
    m_pUart430->readAll();

    if (m_pUart430->write(array) > 0)
    {
        //        m_pUart430->flush();
        qDebug() << "send to 430" << QString::number(type);
        m_comtype = READ_COM;
    }
    else
    {
        qDebug() << "send 430" << QString::number(type) << "fail !";
    }

    return true;
}

//接收处理数据
bool CDetection::comm_430recv()
{
    if(m_pUart430 == NULL)
        return false;

    int nRetVar = m_pUart430->read(m_caRecvBuf, 512);

    if(nRetVar <= 0)
        return false;

    if(m_ReNewPack)
    {
        m_ReNewPack = false;
        m_qByData.clear();
    }
    m_qByData.append(m_caRecvBuf, nRetVar);

    int revtype = m_qByData.at(0);

    if (revtype != DATAN_TYPE)//以免干扰其它日志记录
        qDebug() << "recv from 430" << (int)(revtype);

    switch((DATATYPE)revtype)
    {
    case CHECK_TYPE:    //01 自检
        if(m_qByData.length()>=9)
        {
            m_runstate++;
            //接收到一个完整的包
            if((m_qByData.at(1) == 0x01) && (m_qByData.at(8) == 0xff))
            {
                m_spondata = WAITN_TYPE;
                m_comtype = WAIT_COM;
            }

            m_ReNewPack = true;
        }
        break;
    case DETET_TYPE:    //02 通知采集
        if(m_qByData.length()>=9)
        {
            //接收到一个完整的包
            if((m_qByData.at(1) == 0x01) && (m_qByData.at(8) == 0xff))
            {
                m_detetrun.nCount = 2;
                if(m_resetpos == 0)
                {
                    m_detetrun.astSpeedSteps[0].nDirect = 0;
                    m_detetrun.astSpeedSteps[0].nSpeed = m_detetspeed;
                    m_detetrun.astSpeedSteps[0].nStepCount = m_routestep+200;
                    m_detetrun.astSpeedSteps[1].nDirect = 1;
                    m_detetrun.astSpeedSteps[1].nSpeed = m_resetspeed;
                    m_detetrun.astSpeedSteps[1].nStepCount = m_routestep;
                }
                else
                {
                    m_detetrun.astSpeedSteps[0].nDirect = 1;
                    m_detetrun.astSpeedSteps[0].nSpeed = m_detetspeed;
                    m_detetrun.astSpeedSteps[0].nStepCount = m_routestep;
                    m_detetrun.astSpeedSteps[1].nDirect = 0;
                    m_detetrun.astSpeedSteps[1].nSpeed = m_resetspeed;
                    m_detetrun.astSpeedSteps[1].nStepCount = m_routestep+200;
                }

                XIoctl::Motor1RunN(m_detetrun);
                //qDebug()<<"Motor1RunN";
                emit sigstartsucc();

                m_spondata = WAITN_TYPE;//等待采集结束
            }
            else
            {
                qDebug() << "recv pkg 02 err ! [1] =" << m_qByData.at(1) << "[8] =" << m_qByData.at(8);
            }

            m_ReNewPack = true;
        }
        break;
    case DTEND_TYPE:    //03 通知采集结束
        if(m_qByData.length()>=9)
        {
            //接收到一个完整的包
            if((m_qByData.at(1) == 0x01) && (m_qByData.at(8) == 0xff))
            {
            }

            m_ReNewPack = true;
        }
        break;
    case DATAN_TYPE:    //04 MSP430发送数据给ARM 荧光值

        if(m_qByData.length() >= 412)
        {
            //接收到一个完整的包
            bool packok = false;
            if((m_qByData.at(411) == 0xff) &&
                    (m_qByData.at(410) == 0xff) &&
                    (m_qByData.at(409) == 0xff) &&
                    (m_qByData.at(408) == 0xff) )
            {
                int check = 0;
                for(int i=0;i<4;i++)
                {
                    check+=m_qByData.at(i);
                }
                for(int i=0;i<400;i++)
                {
                    check+=m_qByData.at(8+i);
                }

                //                            qDebug("calculate crc = %08x",check);
                //                            qDebug("recv pack crc = %02x%02x%02x%02x",\
                //                                   m_qByData.at(4),m_qByData.at(5),\
                //                                   m_qByData.at(6),m_qByData.at(7));

                //校验正确
                if((m_qByData.at(4) == ((check>>24)&0xff)) &&
                        (m_qByData.at(5) == ((check>>16)&0xff)) &&
                        (m_qByData.at(6) == ((check>>8)&0xff)) &&
                        (m_qByData.at(7) == (check&0xff)))
                {
                    //这个包正是所需要的
                    if(m_qByData.at(1) == (m_fldata.length()/400+1))
                        m_fldata.append(m_qByData.right(404).left(400));

                    m_spondata = WAITN_TYPE;//只要收到荧光值，即使之前的02指令没收到答复也不再重发
                    comm_430cmd(REDOK_TYPE);
                    packok = true;

                    qDebug() << "recv succ  [" << (int)(m_qByData.at(1)) << "]  pkg";
                }
            }

            if(!packok)//包有问题
            {
                qDebug() << "recv" << m_qByData.at(1) << "pkg check fail, retry. len=" << m_qByData.length()
                         << ", " << QTime::currentTime().toString("hh:mm:ss:zzz");

                comm_430cmd(RDFAIL_TYPE);
            }

            m_ReNewPack = true;
        }

        dispose_fldate();
        break;
    case ERR_TYPE:
        //        errNo = (ERR_NO)m_qByData.at(1);

        //        if (ackSelfCheck)
        //            emit sigRecvErr((int)errNo);
        //        else
        //            ackSelfCheck = true;

        //        m_pUart430->readAll();
        //        m_pUart430->flush();

        m_spondata = WAITN_TYPE;
        m_comtype = WAIT_COM;
        m_ReNewPack = true;
        break;
    case CARD_SLOT_VERIFY:
        m_cardSlotVerify = true;

        m_spondata = WAITN_TYPE;
        m_comtype = WAIT_COM;
        m_ReNewPack = true;
        break;
    case COM_STATE_TYPE: //检查通讯
        m_spondata = WAITN_TYPE;
        m_comtype = WAIT_COM;
        m_ReNewPack = true;
        break;
    case REDOK_TYPE:
    case RDFAIL_TYPE:
    default://包不是所需要的
        m_ReNewPack = true;
        break;
    }

    return true;
}

bool CDetection::comm_430overtime()//超时重发
{
    if((m_spondata != WAITN_TYPE) && (m_curtime.msecsTo(QTime::currentTime()) > m_overtime))
    {
        m_curtime = QTime::currentTime();

        qDebug() << "send 430 timeout, retry" << m_spondata;

        switch(m_spondata)
        {
        case CHECK_TYPE: //自检 答复超时
            comm_430cmd(CHECK_TYPE);
            break;
        case DETET_TYPE: //通知采集 答复超时
            comm_430cmd(RDFAIL_TYPE);
            break;
        case DTEND_TYPE://通知采集结束 答复超时
            comm_430cmd(DTEND_TYPE);
            break;
        case DATAN_TYPE:
            //comm_430cmd(RDERR_TYPE);
            break;
        case REDOK_TYPE:
            break;
        case RDFAIL_TYPE:
            break;
        default:
            break;
        }
    }
    return true;
}
void CDetection::dispose_fldate()
{
    if(m_fldata.length() == 2400) //荧光数据接收完成
    {
        m_flint.clear();
        for(int i=0;i<600;i++)
        {
            int tmp = 0;
            for(int j=0;j<4;j++)
            {
                tmp<<=8;
                tmp += m_fldata.at(i*4+j);
            }
            m_flint.append(tmp);
        }
        /*
        for(int i=0;i<60;i++)
        {
            qDebug("%02d %08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x",
                   i,m_flint.at(i*10+0),m_flint.at(i*10+1),m_flint.at(i*10+2),m_flint.at(i*10+3),
                   m_flint.at(i*10+4),m_flint.at(i*10+5),m_flint.at(i*10+6),m_flint.at(i*10+7),
                   m_flint.at(i*10+8),m_flint.at(i*10+9));
        }
        */
        m_spondata = WAITN_TYPE;
        m_comtype = WAIT_COM;
        emit sigrecvsucc();
    }
}

void CDetection::detetstart()
{
    comm_430cmd(DETET_TYPE);
}

bool CDetection::detetend()
{
    if((m_comtype == WAIT_COM) && (TIMER_UN_USED == XIoctl::Motor1IsRun()))
        return true;
    return false;
}

void CDetection::getfldata(unsigned int *data)
{
    for(int i=0;i<600;i++)
    {
        data[i] = m_flint.at(i);
    }
}

void CDetection::updateCardSlotVerify()
{
    comm_430cmd(CARD_SLOT_VERIFY);
}

bool CDetection::isCardSlotVerify()
{
    return m_cardSlotVerify;
}






