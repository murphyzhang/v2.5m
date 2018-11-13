#include "testcard.h"

XTestCard::XTestCard()
{
    initcard();
}
XTestCard::XTestCard(int ser)
{
    qDebug() << "new XTestCard" << ser;

    m_bIsAutoID = false;
    m_bIsSave = false;
    m_sID = tr("");
    m_sName = tr("");
    m_sSex = tr("");
    m_sAge = tr("");
    m_sSmpNum = tr("");
    m_sSmpType = tr("");

    initcard();
    setsernum(ser);
}
XTestCard::~XTestCard()
{
    qDebug() << "delete XTestCard" << m_sernum;
}
void XTestCard::initcard()
{
    m_detetstate<<tr("空卡")\
                <<tr("等待识别")\
                <<tr("批号识别")\
                <<tr("无法识别")\
                <<tr("插入芯片")\
                <<tr("批号冲突")\
                <<tr("等待反应")\
                <<tr("等待测试")\
                <<tr("正在测试")\
                <<tr("完成测试");
//    m_savepos = 0;
    m_savepos = -1;
    m_sernum = 0;
    m_inserttime = QDateTime::currentDateTime();
    m_awaytime = 0;
    m_surplustime = 0xffffffff;
    m_scancode = true;
    m_hasbatch = false;
    m_batch.clear();
    m_hascode = false;
    m_testend = false;
    m_needtest = true;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slot1sec()));
    m_timer.start(1000);
}

void XTestCard::setsernum(int ser)
{
    m_sernum = ser;
}
int XTestCard::getsernum()
{
    return m_sernum;
}
bool XTestCard::setbatch(QString batch)
{
    if(batch.length() != 8)
    {
        m_hasbatch = false;
        return false;
    }
    m_batch = batch;
    m_hasbatch = true;
    return true;
}
QString XTestCard::getbatch()
{
    if(!m_hasbatch) return "---";
    return m_batch;
}
bool XTestCard::hasbatch()
{
    return m_hasbatch;
}
bool XTestCard::canscan()
{
    return m_scancode;
}
void XTestCard::setnoscan()
{
    m_scancode = false;
}

bool XTestCard::setcode(QByteArray code)
{
    m_decode.set_newcode(code);

    if(m_decode.isvalid())
    {
        m_hascode = true;
        m_formula.Set_QRcode(code);
        return true;
    }
    else
    {
        qDebug() << m_sernum << "card code invalid!";
        m_hascode = false;
        return false;
    }
}

bool XTestCard::hascode()
{
    return m_hascode;
}
XArgDecode * XTestCard::getargdecode()
{
    return &m_decode;
}

Pn_INFO * XTestCard::getpninfo(int pro)
{
    return m_decode.getPnInfo(pro);
}

PF_DATA * XTestCard::getpfinfo(int pro)
{
    return &m_decode.getPnInfo(pro)->proinfo;
}

int XTestCard::getawaytime()
{
    //return m_inserttime.secsTo(QDateTime::currentDateTime());
    return m_awaytime;
}
int XTestCard::getsurptime()
{
    int ret = m_decode.getreatime() - m_awaytime;
    if(ret < 0) ret=0;
    return ret;
}
void XTestCard::slot1sec()
{
    m_awaytime++;
    if(m_hascode)
    {
        if(getsurptime() == 8)
        {
            emit sigreadytest(m_sernum);
        }
        else if(getsurptime() == 0)
        {
            emit sigstarttest(m_sernum);
            //m_statestr = tr("等待测试");
            m_timer.stop();
        }
    }
}

void XTestCard::setfldata(unsigned int *data)
{
    m_formula.Set_FLdata(data);
}

bool XTestCard::cantest()
{
    bool ret = false;

    if(!this->gettestend()) //还没测试
        if(this->hascode())
            if(this->getsurptime() == 0) //时间到
                ret = true;

    return ret;
}
void XTestCard::settestend(bool end)
{
    qDebug() << m_sernum << "card test end.";
    m_endtime = QDateTime::currentDateTime();
    m_testend = true;
    m_timer.stop();
}
bool XTestCard::gettestend()
{
    return m_testend;
}
CFormula * XTestCard::getformula()
{
    return &m_formula;
}

QDateTime XTestCard::getinserttime()
{
    return m_inserttime;
}

QDateTime XTestCard::getendtime()
{
    return m_endtime;
}

void XTestCard::setendtime(QDateTime time)
{
    m_endtime = time;
}

void XTestCard::setsavepos(int pos)
{
    m_savepos = pos;
    m_res.swinum = QString::number(m_savepos);
}

int XTestCard::getsavepos()
{
    return m_savepos;
}

void XTestCard::setstatestr(QString str)
{
    m_statestr = str;
}
QString XTestCard::getstatestr()
{
    return m_statestr;
}

bool XTestCard::getneedtest()
{
    return m_needtest;
}

void XTestCard::setneedtest(bool need)
{
    m_needtest = need;
}

RES_DATA * XTestCard::getresdata()
{
    return &m_res;
}

void XTestCard::setresdata(int at,QString res)
{
    if((at>=0)&&(at<5))
    {
        m_stres[at] = res;
    }
}

void XTestCard::truntoresdata()
{
    m_res.batch = m_batch;
    int howres = 0;
    for(int i=0;i<m_decode.getunite();i++)
    {
        Pn_INFO *tmpad = m_decode.getPnInfo(i);
        m_res.pro[howres].name = tmpad->proinfo.proname;
        m_res.pro[howres].unit = tmpad->proinfo.prounit;
        m_res.pro[howres].result = m_stres[i];
        if((m_res.pro[howres].result.indexOf("ERR")<0) &&
            (m_res.pro[howres].result != "NULL") &&
            (m_res.pro[howres].result.indexOf(">")<0)&&
            (m_res.pro[howres].result.indexOf("<")<0))
        {
            if(m_res.pro[howres].result.toDouble() < tmpad->proinfo.observedmin)
                m_res.pro[howres].result = "<"+QString::number(tmpad->proinfo.observedmin);
            else if(m_res.pro[howres].result.toDouble() > tmpad->proinfo.observedmax)
                m_res.pro[howres].result = ">"+QString::number(tmpad->proinfo.observedmax);
        }
        m_res.pro[howres].refer = QString::number(tmpad->proinfo.refermin)+"-"+\
                QString::number(tmpad->proinfo.refermax);
        howres++;
        if(tmpad->proinfo.nextsection != NULL)
        {

            m_res.pro[howres].name = tmpad->nextproinfo.proname;
            m_res.pro[howres].unit = tmpad->nextproinfo.prounit;
            m_res.pro[howres].result = m_stres[i];
            if((m_res.pro[howres].result.indexOf("ERR")<0) &&
                (m_res.pro[howres].result != "NULL") &&
                (m_res.pro[howres].result.indexOf(">")<0)&&
                (m_res.pro[howres].result.indexOf("<")<0))
            {
                if(m_res.pro[howres].result.toDouble() < tmpad->nextproinfo.observedmin)
                    m_res.pro[howres].result = "<"+QString::number(tmpad->nextproinfo.observedmin);
                else if(m_res.pro[howres].result.toDouble() > tmpad->nextproinfo.observedmax)
                    m_res.pro[howres].result = ">"+QString::number(tmpad->nextproinfo.observedmax);
            }
            m_res.pro[howres].refer = QString::number(tmpad->nextproinfo.refermin)+"-"+\
                    QString::number(tmpad->nextproinfo.refermax);
            howres++;
        }
    }
    m_res.unite = howres;

    m_res.endtime = m_endtime;
    m_res.code = m_decode.get_codestr();

    //加入患者信息
    m_res.patient.m_bIsSave = m_bIsSave;
    m_res.patient.m_sID = m_sID;
    m_res.patient.m_sName = m_sName;
    m_res.patient.m_sSex = m_sSex;
    m_res.patient.m_sAge = m_sAge;
    m_res.patient.m_sSmpNum = m_sSmpNum;
    m_res.patient.m_sSmpType = m_sSmpType;
}

void XTestCard::setnotes(QString qStrID)
{
    m_res.notes = qStrID;
}

void XTestCard::setnotes(QString qStrID,QString qStrName,QString qStrSex,QString qStrAge,QString qStrSmpNum,QString qStrSmpType)
{
    if(qStrID.isEmpty())
    {
        //
    }

    m_res.notes = qStrID;
    m_bIsSave = true;
    m_sID = qStrID;
    m_sName = qStrName;
    m_sSex = qStrSex;
    m_sAge = qStrAge;
    m_sSmpNum = qStrSmpNum;
    m_sSmpType = qStrSmpType;
}

QString XTestCard::getnotes()
{
    return m_res.notes;
}
void XTestCard::setupdatedtime(QDateTime time)
{
    m_res.updatedtime = time;
}
QDateTime XTestCard::getupdatedtime()
{
    return m_res.updatedtime;
}

void XTestCard::SetAutoID(bool bAuto)
{
    m_bIsAutoID = bAuto;
}

QString XTestCard::GetPatientID()
{
    return m_sID;
}

bool XTestCard::getAutoID()
{
    return m_bIsAutoID;
}

























