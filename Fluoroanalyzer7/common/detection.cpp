#include "detection.h"


CDetection::CDetection(QObject *parent): QThread(parent)
{
    //    qDebug()<<"---------init CDetection-------------";
    m_runtype = WAIT_RUN;
    m_runstate = 0;
    m_datatype = WAITN_TYPE;
    m_comtype = WAIT_COM;

    init_detection();

    ackSelfCheck = false;
    errNo = ERR_NONE;
    comState = false;

//    memset(m_auFlData, 0, START_POINT_3_UNITE_CARD);  //m_auFlData 二维码专用, 初始化 0 ~ START_POINT_3_UNITE_CARD 部份为0

    m_jiffies = 0;
    //    m_beforetime = QTime::currentTime();
}

void CDetection::init_detection()
{
    XIoctl::InitMotor1();
    m_resetpos = CSysInfo::get_cfg(CFG_FY_DETEDIR).toInt();
    m_routestep = CSysInfo::get_cfg(CFG_FY_TRAVEL).toInt();
    m_detetspeed = CSysInfo::get_cfg(CFG_FY_DETESPE).toInt();
    m_resetspeed = CSysInfo::get_cfg(CFG_FY_RESTSPE).toInt();

    if(!CSerialPort::initUART3(BAUD9600))
        qDebug()<<"initUART3 ERR!";
    m_pUart430 = CSerialPort::GetSerialPort(CSerialPort::UART_3_430); // 串口指针

}

bool CDetection::start_check()
{
    if(m_runtype != WAIT_RUN)
        return false;

    if(TIMER_IN_USED == XIoctl::TimeState())
        XIoctl::StopTime();

    m_detetrun.nCount = 1;
    m_detetrun.astSpeedSteps[0].nDirect = 0;
    m_detetrun.astSpeedSteps[0].nSpeed = m_resetspeed;
    m_detetrun.astSpeedSteps[0].nStepCount = m_routestep+200;
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
#if 0
    if(m_runtype == WAIT_RUN)
        return true;

    return false;
#endif

    if (ackSelfCheck)
        return true;

    return false;
}

bool CDetection::state_check()
{
#if 0
    if(m_runstate == 2)
        return true;
    return false;
#else
    if (errNo == ERR_NONE)
        return true;
    return false;
#endif
}

void CDetection::run()
{
    while(1)
    {
        QThread::msleep(20);

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
            QThread::msleep(10);
            break;

        default:
            break;
        }

        m_jiffies++;
    }
}

//type:命令类型
bool CDetection::comm_430cmd(CDetection::DATATYPE type, char * pcData)
{
    m_spondata = type;
    m_curtime = QTime::currentTime();
    m_ReNewPack = true;

    QByteArray array;
    switch(type)
    {
    case CHECK_TYPE:
        m_overtime = 180000;//开机自检机制已经有重发功能,这里设置180000实际是为了在本线程中不重发.
        array.append("1#");
        break;
    case DETET_TYPE:
        m_overtime = 5000;
        array.append("2#");
        m_fldata.clear();
        break;
    case DTEND_TYPE:
        m_overtime = 2000;
        array.append("3#");
        break;
    case DATAN_TYPE:
        m_overtime = 180000;
        array.append("4#");
        break;
    case REDOK_TYPE:
        m_overtime = 180000;
        array.append("5#");
        break;
    case RDFAIL_TYPE:
        m_overtime = 180000;
        array.append("6#");
        break;
    case ERR_TYPE:
        break;
    case COM_STATE_TYPE: //检查通讯
        m_overtime = 1000;
        array.append("8#");
        break;
    case CARD_SLOT_VERIFY:
    {
        m_overtime = 1000;
        
        int verify = CSysInfo::get_cfg(CFG_QT_CSVERIFY).toInt();
        if (verify < -200 || 200 < verify)
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
        //    qDebug() << "CDetection::updateCardSlotVerify() =" << QString::number(pcVerify[0]) << QString::number(pcVerify[1]);

        m_cardSlotVerify = false;
        break;
    }
    default:
        break;
    }

#if 0
    while(m_beforetime.msecsTo(QTime::currentTime()) < 300)//500
    {
        QThread::msleep(50);
    }
    m_beforetime = QTime::currentTime();
#endif

    m_pUart430->flush();

    if (m_pUart430->write(array) > 0)
    {
        qDebug("send to 430 %d#, myJiffy = %d", type, m_jiffies);
        m_comtype = READ_COM;
    }
    else
    {
        qDebug("send to 430 %d# fail! myJiffy = %d", type, m_jiffies);
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
        m_baBuf.clear();
    }

    m_baBuf.append(m_caRecvBuf, nRetVar);
    int revtype = m_baBuf.at(0);

    if ( revtype != DATAN_TYPE )//以免干扰其它日志记录
        qDebug() << "recvLen ="  << nRetVar
                 << ", datalen =" << m_baBuf.length()
                 << ", myJiffy =" << m_jiffies;
#if 1
    if (m_baBuf.length() < 9 || (revtype != DATAN_TYPE && m_baBuf.at(8) != 0xff))
        return false;
#else
    if (m_baBuf.length() < 9 && m_baBuf.at(8) != 0xff)
        return false;
#endif

    if ( revtype != DATAN_TYPE )//以免干扰其它日志记录
        qDebug() << "ack cmd:" << (int)(revtype) << ", myJiffy =" << m_jiffies;

    switch((DATATYPE)revtype)
    {
    case CHECK_TYPE://0x01
        m_runstate++;
        //接收到一个完整的包
        if((m_baBuf.at(1) == 0x01))
        {
            m_spondata = WAITN_TYPE;
            m_comtype = WAIT_COM;
        }

        ackSelfCheck = true;
        m_ReNewPack = true;
        break;
    case DETET_TYPE://0x02
        //接收到一个完整的包
        if((m_baBuf.at(1) == 0x01))
        {
            m_detetrun.nCount = 2;

            if(m_resetpos == 0)
            {
                    m_detetrun.astSpeedSteps[0].nDirect = 0;            // 方向
                    m_detetrun.astSpeedSteps[0].nSpeed = m_detetspeed;	// 检测速度 400
                    m_detetrun.astSpeedSteps[0].nStepCount = m_routestep+200; // 行程 4248
                    m_detetrun.astSpeedSteps[1].nDirect = 1;
                    m_detetrun.astSpeedSteps[1].nSpeed = m_resetspeed; // 复位速度 200
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
            emit sigstartsucc();
            m_spondata = WAITN_TYPE;//等待采集结束
        }
        else
        {
            qDebug() << "recv pkg 02 err ! [1] =" << m_baBuf.at(1) << "[8] =" << m_baBuf.at(8) << " retry";
            comm_430cmd(RDFAIL_TYPE);
        }

        m_ReNewPack = true;
        break;
    case DTEND_TYPE://0x03
        //接收到一个完整的包
        if((m_baBuf.at(1) == 0x01))
        {
        }
        m_ReNewPack = true;
        break;
    case DATAN_TYPE://0x04
        if(m_baBuf.length() >= 412)
        {
            //接收到一个完整的包
            bool packok = false;

            if( (m_baBuf.at(411) == 0xff)
                    && (m_baBuf.at(410) == 0xff)
                    && (m_baBuf.at(409) == 0xff)
                    && (m_baBuf.at(408) == 0xff))
            {
                int check = 0;
                for(int i = 0; i < 4; i++)
                    check += m_baBuf.at(i);

                for(int i = 0; i < 400; i++)
                    check += m_baBuf.at(8 + i);

                //                qDebug("calculate crc = %08x",check);
                //                qDebug("recv pack crc = %02x%02x%02x%02x",
                //                       m_baBuf.at(4),m_baBuf.at(5),
                //                       m_baBuf.at(6),m_baBuf.at(7));
                //校验正确
                if(  (m_baBuf.at(4) == ((check>>24)&0xff))
                     && (m_baBuf.at(5) == ((check>>16)&0xff))
                     && (m_baBuf.at(6) == ((check>>8)&0xff))
                     && (m_baBuf.at(7) == (check&0xff)))
                {
                    //这个包正是所需要的
                    if(m_baBuf.at(1) == (m_fldata.length() / 400 + 1))
                        m_fldata.append(m_baBuf.right(404).left(400));

                    int nPkgIndex = (int)(m_baBuf.at(1));
                    qDebug("recv succ [%d] pkg", nPkgIndex);

                    convert1PkgFluData(nPkgIndex, m_baBuf.data() + 8);

                    m_spondata = WAITN_TYPE;//只要收到荧光值，即使之前的02指令没收到答复也不再重发
                    comm_430cmd(REDOK_TYPE);
                    packok = true;
                }
            }

            if(!packok)//包有问题
            {
                qDebug("recv %d pkg check fail, retry. len=%d, myJiffy = %d"
                       , m_baBuf.at(1)
                       , m_baBuf.length()
                       , m_jiffies );
                comm_430cmd(RDFAIL_TYPE);
            }

            m_ReNewPack = true;
        }

        dispose_fldate();
        break;
    case ERR_TYPE:  //0x07
        errNo = (ERR_NO)m_baBuf.at(1);

        if (ackSelfCheck)
            emit sigRecvErr((int)errNo);
        else
            ackSelfCheck = true;
        
        m_spondata = WAITN_TYPE;
        m_comtype = WAIT_COM;
        m_ReNewPack = true;
        break;
    case CARD_SLOT_VERIFY:  // 0x09
        m_cardSlotVerify = true;
        
        m_spondata = WAITN_TYPE;
        m_comtype = WAIT_COM;
        m_ReNewPack = true;
        break;
    case COM_STATE_TYPE: // 0x08 检查通讯
        comState = true;

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

    comState = true;
    return true;
}

bool CDetection::comm_430overtime()//超时重发
{
    if((m_spondata != WAITN_TYPE) && (m_curtime.msecsTo(QTime::currentTime()) > m_overtime))
    {
        m_curtime = QTime::currentTime();
        qDebug("recv 430 %d# ACK timeout, retry. myJiffy = %d", m_spondata, m_jiffies);
#if 0
        m_pUart430->flush();
        m_pUart430->readAll();
#endif
        switch(m_spondata)
        {
        case CHECK_TYPE: //检测回应超时
            comm_430cmd(CHECK_TYPE);
            break;
        case DETET_TYPE: //采集指令接收超时
            comm_430cmd(RDFAIL_TYPE);
            comm_430cmd(DETET_TYPE);
            break;
        case DTEND_TYPE://通知采集结束            
            comm_430cmd(DTEND_TYPE);
            break;
        case DATAN_TYPE:
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
    if(m_fldata.length() == DATA_MAX_SIZE * 4) //荧光数据接收完成
    {
        m_flint.clear();
        int tmp = 0;
        int index = 0;

        for(int i = 0; i < DATA_MAX_SIZE; i++)
        {
            tmp = 0;
            index = i * 4 ;

            for(int j = 0; j < 4; j++)
            {
                tmp <<= 8;
                tmp += m_fldata.at(index + j);
            }

            m_flint.append(tmp);
        }

        //        // 2016-08-09 xufan
        //        char * pc = m_fldata.data();

        //        for(int i = 0; i < FLU_DATA_LENGTH; i++)//START_POINT_3_UNITE_CARD
        //        {
        //            index = i * 4;
        //            m_auFlData[i] = (pc[index] << 24) | (pc[index + 1] << 16) | (pc[index + 2] << 8) | pc[index + 3];
        //        }

        m_spondata = WAITN_TYPE;
        m_comtype = WAIT_COM;        
        emit sigrecvsucc();
    }
}

void CDetection::convert1PkgFluData(int pkgIndex, char *pData)
{
    int i       = (pkgIndex - 1) * 100;
    int end     = i + 100;
    int index   = 0;
    double d = CSysInfo::get_cfg(CFG_FY_CORRECT).toDouble();

    for(int j = 0; i < end; i++, j++)
    {
        index = j * 4;
        m_auFlData[i] = (pData[index] << 24) | (pData[index + 1] << 16) | (pData[index + 2] << 8) | pData[index + 3];
        m_auFlData[i] *= d;
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
    for(int i = 0; i < DATA_MAX_SIZE; i++)
    {
        data[i] = m_flint.at(i);
    }
}

unsigned int * CDetection::getfldata()
{
    return m_auFlData;
}

void CDetection::checkCom()
{
    comState = false;
    comm_430cmd(COM_STATE_TYPE);
}

bool CDetection::checkComResult()
{
    return comState;
}

void CDetection::updateCardSlotVerify()
{
    comm_430cmd(CARD_SLOT_VERIFY);
}

bool CDetection::isCardSlotVerify()
{
    return m_cardSlotVerify;
}

void CDetection::cancelResend()
{
    m_spondata = WAITN_TYPE;
    //    m_pUart430->flush();
    //    m_pUart430->readAll();
}








