#include "xprintf.h"
#include <QDebug>
#include <QTextCodec>
CXPrintf::CXPrintf()
{
    m_bprintf = false;
    if(!CSerialPort::OpenSerialPort(CSerialPort::UART_1_PC_PRINTER))
    {
//        qDebug()<<"*********PC_PRINTER SerialPort init ERR********";
    }
    m_pUartprintf = CSerialPort::GetSerialPort(CSerialPort::UART_1_PC_PRINTER); // 串口指针
}

void CXPrintf::Send_printf_com(char arg1)
{
    QByteArray arrayBegin;
    arrayBegin.append(arg1);
    m_pUartprintf->write(arrayBegin);
}
void CXPrintf::Send_printf_com(char arg1, char arg2)
{
    Send_printf_com(arg1);
    Send_printf_com(arg2);
}
void CXPrintf::Send_printf_com(char arg1, char arg2, char arg3)
{
    Send_printf_com(arg1);
    Send_printf_com(arg2);
    Send_printf_com(arg3);
}
void CXPrintf::Send_printf_com(char arg1, char arg2, char arg3, char arg4)
{
    Send_printf_com(arg1);
    Send_printf_com(arg2);
    Send_printf_com(arg3);
    Send_printf_com(arg4);
}
void CXPrintf::Send_printf_com(char arg1, char arg2, char arg3, char arg4, char arg5)
{
    Send_printf_com(arg1);
    Send_printf_com(arg2);
    Send_printf_com(arg3);
    Send_printf_com(arg4);
    Send_printf_com(arg5);
}

unsigned char CXPrintf::Get_printf_status()
{

}
void CXPrintf::Printf_string(QString str)
{
    QTextCodec* codec = QTextCodec::codecForName("GBK");
    QByteArray array = codec->fromUnicode(str);
    m_pUartprintf->write(array); //
}
void CXPrintf::Printf_ENTRN()
{
    m_pUartprintf->write((char *)"\r\n");
}
QString CXPrintf::convert_3str(QString str1, QString str2, QString str3)
{
    QString tmpstr;

    QTextCodec *gbk = QTextCodec::codecForName("GBK");
    QByteArray u2g = gbk->fromUnicode(str1);

    int currpos = 0;
    int tmplen = (10-u2g.length())/2;
    for(int i=0;i<tmplen;i++)
    {
        tmpstr.append(" ");
        currpos++;
    }
    tmpstr.append(str1);
    currpos+=u2g.length();

    tmplen = 11-currpos+(10-str2.length())/2;
    for(int i=0;i<tmplen;i++)
    {
        tmpstr.append(" ");
        currpos++;
    }
    tmpstr.append(str2);
    currpos+=str2.length();

    tmplen = 22-currpos+(10-str3.length())/2;
    for(int i=0;i<tmplen;i++)
        tmpstr.append(" ");
    tmpstr.append(str3);

    return tmpstr;
}
QString CXPrintf::convert_res(int res, PF_DATA *proinfo)
{
    QString tmpstr;
    if(res < -1)
    {
        tmpstr.append(" ---");
    }
    else if(res < proinfo->observedmin)
    {
        tmpstr.append("<"+QString::number(proinfo->observedmin));
    }
    else if(res > proinfo->observedmax)
    {
        tmpstr.append(">"+QString::number(proinfo->observedmax));
    }
    else
    {
        tmpstr.append(QString::number(res/100.0));
    }
    return tmpstr;
}

void CXPrintf::Printf_res(QString name, QString res, QString unit)
{
    /*
    int idex = name.indexOf("β");
    if(idex>=0)
    {
        QString tmp;//=QString("%1c").arg(225);
        tmp.sprintf("%c",226);
        name.replace(idex,1,tmp);
    }
    */
    QString tmpstr = convert_3str(name,res,unit);

    // 取消汉字模式
    //Send_printf_com(0x1c,0x2e);

    Printf_string(tmpstr);

    // 设置汉字模式
    //Send_printf_com(0x1c,0x26);
}
void CXPrintf::Set_printfdata(RES_DATA *data)
{
    m_data.swinum = data->swinum;
    m_data.batch = data->batch;

    m_data.unite = data->unite;
    for(int i=0;i<m_data.unite;i++)
    {
        m_data.pro[i].name = data->pro[i].name;
        m_data.pro[i].result = data->pro[i].result;
        m_data.pro[i].unit = data->pro[i].unit;
        m_data.pro[i].refer = data->pro[i].refer;
    }
    m_data.endtime = data->endtime;
    m_data.code = data->code;
    m_data.notes = data->notes;
    //加入患者信息
    m_data.patient.m_bIsSave = data->patient.m_bIsSave;
    m_data.patient.m_sID = data->patient.m_sID;
    m_data.patient.m_sName = data->patient.m_sName;
    m_data.patient.m_sSex = data->patient.m_sSex;
    m_data.patient.m_sAge = data->patient.m_sAge;
    m_data.patient.m_sSmpNum = data->patient.m_sSmpNum;
    m_data.patient.m_sSmpType = data->patient.m_sSmpType;
}

void CXPrintf::Printf_topage()
{
    m_bprintf = true;
    // 设置打印机在线 start
    Send_printf_com(0x1b,0x3d,0x1);
    // 设置汉字模式
    Send_printf_com(0x1c,0x26);
    // 选择字符代码表
    Send_printf_com(0x1b,0x74,0x29);

    Printf_string(QObject::tr("           Test report"));
    Printf_ENTRN();

    qDebug()<<"-----bool print patient info:"<<m_data.patient.m_bIsSave;
    if(m_data.patient.m_bIsSave)
    {
        Printf_string(QObject::tr("No.："));
        Printf_string(m_data.patient.m_sID);
        Printf_ENTRN();
    }

    Printf_string(QObject::tr("Test No.："));
    Printf_string(m_data.swinum.left(6));
    Printf_string(QObject::tr("    Lot No.："));
    Printf_string(m_data.batch.left(8));

    if(m_data.patient.m_bIsSave)
    {
        Printf_ENTRN();
        Printf_string(QObject::tr("Name："));
        Printf_string(m_data.patient.m_sName);
        Printf_string(QObject::tr("  sex："));
        Printf_string(m_data.patient.m_sSex);
        Printf_string(QObject::tr("  age："));
        Printf_string(m_data.patient.m_sAge);
    }
/*
    if(m_data.proinfo->nextsection != NULL)
    {
        Printf_ENTRN();
        Printf_string("参考值: ");
        QString tmp = m_data.proinfo->proname + QString::number(m_data.proinfo->refermin)+"-"+QString::number(m_data.proinfo->refermax) +"|"+\
                      m_data.proinfo->nextsection->proname + QString::number(m_data.proinfo->nextsection->refermin)+"-"+QString::number(m_data.proinfo->nextsection->refermax);
        Printf_string(tmp);
    }
    else
    {
        Printf_string("  参考值: ");
        Printf_string(QString::number(m_data.proinfo->refermin)+"-"+QString::number(m_data.proinfo->refermax));
    }
*/
    Printf_ENTRN();
    Printf_string(QObject::tr("--------------------------------"));
    Printf_ENTRN();
    Printf_string(QObject::tr("  Item  |  Result  |  Unit"));
    Printf_ENTRN();

    for(int i=0;i<m_data.unite;i++)
    {
        Printf_res(m_data.pro[i].name,m_data.pro[i].result,m_data.pro[i].unit);
        Printf_ENTRN();
    }

    Printf_string(QObject::tr("--------------------------------"));
    Printf_ENTRN();

    Printf_string(QObject::tr("ID："));
    Printf_string(m_data.notes);
    Printf_ENTRN();

    //Printf_string(QObject::tr("日    期: "));
    Printf_string(QObject::tr("Date: "));
    Printf_string(m_data.endtime.toString("yyyy-MM-dd  hh:mm:ss"));
    Printf_ENTRN();

    //Printf_string(QObject::tr("          *本结果仅对此标本负责"));
    Printf_string(QObject::tr("* The test result is only\n responsible for this sample."));


    Printf_ENTRN();
    Printf_ENTRN();

    if(!m_data.pro[0].refer.isEmpty() && m_bpfrefer)
    {
        //Printf_string(QObject::tr("      免疫荧光项目参考值"));
        Printf_string(QObject::tr("         Reference Value"));
        Printf_ENTRN();
        Printf_string(QObject::tr("--------------------------------"));
        Printf_ENTRN();
        //Printf_string(QObject::tr("  项  目  |  参考值  |  单  位"));
        Printf_string(QObject::tr("  Item  |  RV  |  Unit"));
        Printf_ENTRN();
        for(int i=0;i<m_data.unite;i++)
        {
            Printf_res(m_data.pro[i].name,m_data.pro[i].refer,m_data.pro[i].unit);
            Printf_ENTRN();
        }
        Printf_string(QObject::tr("--------------------------------"));
        Printf_ENTRN();
        //Printf_string(QObject::tr("          *请综合个体情况判定"));
        Printf_string(QObject::tr("* The test result is only\n responsible for this sample."));
    }

    Printf_ENTRN();
    Printf_ENTRN();
    Printf_ENTRN();
    Printf_ENTRN();


    // 设置打印机离线 end
    Send_printf_com(0x1b,0x3d,0x0);
    m_bprintf = false;
}

void CXPrintf::Printf_topage(RES_DATA *data)
{
    Set_printfdata(data);
    Printf_topage();
}

bool CXPrintf::Get_bprintf()
{
    return m_bprintf;
}

void CXPrintf::set_bpfrefer(bool pf)
{
    m_bpfrefer = pf;
}













