#include "qrscan.h"
#include <QDebug>

CQrScan::CQrScan(QObject *parent) :
    QThread(parent)
{
    qDebug()<<"---------init CQrScan-------------";
    init_qrscan();
    m_scansucc = false;
    m_runtype = WAIT_RUN;
    m_ReNewPack = false;
    m_reply = RP_NAN;
    m_waitreply = false;
}

void CQrScan::init_qrscan()
{
    m_qrnum = CSysInfo::get_cfg(CFG_SM_QRLEND).toInt();
    m_overtime = CSysInfo::get_cfg(CFG_SM_SCANTIM).toInt();

    if(!CSerialPort::OpenSerialPort(CSerialPort::UART_2_SCAN))
    {
        qDebug()<<"*********SCAN SerialPort init ERR********";
    }
    m_pUartscan = CSerialPort::GetSerialPort(CSerialPort::UART_2_SCAN); // 串口指针
}

bool CQrScan::start_check()
{
    start_scan();
    return true;
}

bool CQrScan::isend_check()
{
    if(m_runtype == WAIT_RUN)
        return true;
    return false;
}

bool CQrScan::state_check()
{
    return true;
}

void CQrScan::run()
{
    char temReBuf[1024];
    while(1)
    {
        QThread::usleep(100);
        switch(m_runtype)
        {
        case SCAN_RUN:
        {
            if(m_pUartscan != NULL)
            {
                int nRetVar = m_pUartscan->read(temReBuf, 1024);
                if(nRetVar > 0)
                {
                    m_qByData.append(temReBuf,nRetVar);
                    if(m_waitreply)
                    {
                        m_waitreply = false;
                        if(m_qByData.at(0) == RP_ACK)
                        {
                            m_qByData.remove(0,1);
                            m_reply = RP_ACK;
                            qDebug()<< "scan recv reply ACK ---OK";
                        }
                        else if(m_qByData.at(0) == RP_NAK)
                        {
                            m_reply = RP_NAK;
                            m_scansucc = true;
                            m_runtype = WAIT_RUN;
                            qDebug()<< "scan recv reply NAK ---ERR";
                        }
                        else
                        {
                            qDebug()<< "scan recv reply NULL";
                        }
                    }
                    if((m_reply == RP_ACK) && (m_qByData.length() >= m_qrnum))
                    {
                        m_qrcode.clear();
                        m_scansucc = true;
                        m_runtype = WAIT_RUN;
                        int mt = m_qByData.left(3).toInt();
                        int mon = (mt%12)?(mt%12):12;
                        int year = (15+(mt-1)/12)%100;

                        m_qrcode.append(QString::number(year));
                        if(mon<10)
                            m_qrcode.append('0');
                        m_qrcode.append(QString::number(mon));
                        m_qrcode.append(m_qByData.left(6).right(3));
                        //m_qrcode.remove("\n");
                        //m_qrcode.remove("\r");
                        m_qByData.clear();
                        qDebug()<<"qr = "<<m_qByData<<m_qrcode;
                        emit sigqrscanend();
                    }

                    /*
                    m_ReNewPack = false;
                    int len = m_qByData.length();
                    if(len >= 4)
                    {
                        int fh = m_qByData.indexOf(0x02);
                        int fe = m_qByData.indexOf(0x03);

                        if((fh>=0) && (fe>=0))
                        {
                            while((fe >= 0) && (fe < (len-1)))
                            {
                                char x1 = m_qByData.at(fe+1);
                                char x2 = getxordata(m_qByData.mid(fh,fe-fh+1));
                                if(x1 == x2)
                                {
                                    m_ReNewPack = true;
                                    break;
                                }
                                fe = m_qByData.indexOf(0x03,fe+1);
                            }
                        }

                        if(m_ReNewPack == true)
                        {

                            m_qrcode.clear();
                            m_scansucc = true;
                            m_runtype = WAIT_RUN;
                            m_qrcode.append(m_qByData.mid(fh,fe-fh+1));
                            m_qByData.clear();

                            emit sigqrscanend();
                            end_scan();
                        }
                    }
                    */

                }
            }
            if(!m_scansucc && (m_curtime.msecsTo(QTime::currentTime()) > m_overtime))
            {
                qDebug()<<"qrscan over time "<<QTime::currentTime();
                qDebug()<<"qrscan time out "<<m_reply<<m_qByData.length();
                m_qByData.clear();
                m_scansucc = false;
                m_runtype = WAIT_RUN;

                if((m_reply == RP_ACK) && (m_qByData.length() == 0)) //应答成功，但是没收到数据 发送关闭
                    end_scan();
                emit sigqrscanend();
            }
            QThread::usleep(1000);
        }break;
        case WAIT_RUN:
        {
            QThread::usleep(1000);
        }break;
        default:
            break;
        }
    }
}

bool CQrScan::start_scan()
{
    QByteArray ArrayStart;
    ArrayStart.append(SCAN_STX);
    ArrayStart.append(char(COM_82H));
    ArrayStart.append(SCAN_ETX);
    ArrayStart.append(getxordata(ArrayStart));

    m_qByData.clear();
    m_waitreply = true;
    m_reply = RP_NAN;
    m_pUartscan->readAll();
    m_pUartscan->flush();
    m_pUartscan->write(ArrayStart);
    m_curtime = QTime::currentTime();
    qDebug()<<"qrscan time "<<m_curtime;
    m_runtype = SCAN_RUN;
    m_scansucc = false;
}

bool CQrScan::end_scan()
{
    /*
    QByteArray ArrayStart;
    ArrayStart.append(SCAN_STX);
    ArrayStart.append(char(COM_82H));
    ArrayStart.append(SCAN_ETX);
    ArrayStart.append(getxordata(ArrayStart));
    m_pUartscan->write(ArrayStart);
    */
    m_pUartscan->readAll();
    m_pUartscan->flush();

}


bool CQrScan::qrscanend()
{
    if(m_runtype == WAIT_RUN)
        return true;
    return false;
}

bool CQrScan::getqrcode(QString *qrcode)
{
    if(qrcode != NULL)
        qrcode->append(m_qrcode);
    return m_scansucc;
}

char CQrScan::getxordata(QByteArray data)
{
    char ret = data.at(0);
    int len = data.length();
    for(int i=1;i<len;i++)
    {
        ret ^= data.at(i);
    }
    qDebug()<<"scan send xor = "<<ret;
    return ret;
}
