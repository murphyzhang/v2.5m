#include "argdecode.h"
#include <QDebug>
XArgDecode::XArgDecode()
{
    is_valid = false;
    m_cardtype = 0;
    m_currcode.clear();
}

XArgDecode::XArgDecode(QByteArray &code)
{
    set_newcode(code);
}

bool XArgDecode::set_newcode(QByteArray &code)
{
    is_valid = false;
    m_cardtype = 0;
    m_currcode.clear();
    return arg_decode(code);
}

QByteArray XArgDecode::get_codestr()
{
    return m_currcode;
}

bool XArgDecode::arg_decode(QByteArray code)
{
    bool ret = true;
    is_valid = is_code_valid(code);
    if(!is_valid)
    {
        ret = false;
    }
    else
    {
        int pos = 0;
        pos=4;
        m_cardtype = code.at(pos);
        pos++;
        m_unite = code.at(pos);
        pos++;
        m_PDyear = code.at(pos);
        pos++;
        m_PDmon = code.at(pos);
        pos++;
        m_PDser = code.at(pos);
        pos++;
        m_IESser = (bool)(code.at(pos)&0x01);
        pos++;

        m_bcsera = code.at(pos);
        pos++;
        m_bcpeak = code.at(pos);
        pos++;
        m_bcbagr = code.at(pos);
        pos++;

        m_minarea = char_to_short(code.mid(pos,2));
        pos+=2;
        m_minpeak = char_to_short(code.mid(pos,2));
        pos+=2;

        m_bdtype = code.at(pos);
        pos++;
        m_bdAPos = char_to_short(code.mid(pos,2));
        pos+=2;
        m_bdBPos = char_to_short(code.mid(pos,2));
        pos+=2;

        m_reatime = char_to_short(code.mid(pos,2));
        pos+=2;

        for(int i=0;i<6;i++)
        {
            int peak = code.at(pos);

            if((peak-1) == i)
            {
                pos++;
                m_peak[i].isusd = true;
                m_peak[i].coord = char_to_short(code.mid(pos,2));
                pos+=2;
                m_peak[i].mf = code.at(pos);
                pos++;
            }
            else
            {
                m_peak[i].isusd = false;
            }
        }
        pos++;

        for(int i=0;i<m_unite;i++)
        {
            m_pro[i].isusd = true;
            for(int j=0;j<4;j++)
            {
                m_pro[i].pre[j].numerator = code.at(pos);
                pos++;
                m_pro[i].pre[j].denominator = code.at(pos);
                pos++;
                m_pro[i].pre[j].islog = (bool)(code.at(pos)&0x01);
                pos++;
            }
            m_pro[i].thup = char_to_float(code.mid(pos,4));
            pos+=4;
            m_pro[i].thdw = char_to_float(code.mid(pos,4));
            pos+=4;
            m_pro[i].islimit = (bool)(code.at(pos)&0x01);
            pos++;
            m_pro[i].limup = char_to_float(code.mid(pos,4));
            pos+=4;
            m_pro[i].limdw = char_to_float(code.mid(pos,4));
            pos+=4;
            m_pro[i].dtway = code.at(pos);
            pos++;

            for(int j=0;j<3;j++)   //3个后续公式
            {
                m_pro[i].fol[j].nfun = code.at(pos);
                pos++;
                m_pro[i].fol[j].avalue = char_to_float(code.mid(pos,4));
                pos+=4;
                m_pro[i].fol[j].bvalue = char_to_float(code.mid(pos,4));
                pos+=4;
                m_pro[i].fol[j].cvalue = char_to_float(code.mid(pos,4));
                pos+=4;
                m_pro[i].fol[j].dvalue = char_to_float(code.mid(pos,4));
                pos+=4;
                m_pro[i].fol[j].isrelog = (bool)(code.at(pos)&0x01);
                pos++;
                //qDebug()<<i<<m_pro[i].fol[j].nfun<<m_pro[i].fol[j].avalue<<m_pro[i].fol[j].bvalue<<m_pro[i].fol[j].cvalue<<m_pro[i].fol[j].dvalue;
            }

            int sect = code.at(pos);
            pos++;
            if(sect != 0xff)
            {
                for(int j=0;j<sect;j++)
                {
                    int len = 0;
                    PF_DATA *pftmp;
                    if((code.at(pos)&0x80) == 0)
                    {
                        pftmp = &m_pro[i].proinfo;
                        len = pf_decode(code.right(code.length()-pos),pftmp);
                        pos += len;
                        pftmp->nextsection = NULL;
                    }
                    else if((code.at(pos)&0x80) != 0)
                    {
                        pftmp = &m_pro[i].nextproinfo;
                        len = pf_decode(code.right(code.length()-pos),pftmp);
                        pos += len;
                        m_pro[i].proinfo.nextsection = pftmp;
                    }
                }
            }
            pos++;
        }
        for(int i=m_unite;i<5;i++)
        {
            m_pro[i].isusd = false;
            m_pro[i].proinfo.proname.clear();
            m_pro[i].proinfo.prounit.clear();
        }
        m_currcode.clear();
        m_currcode.append(code);
    }
    return ret;
}

bool XArgDecode::isvalid()
{
    return is_valid;
}
bool XArgDecode::isIESser()
{
    return m_IESser;
}
quint8 XArgDecode::getcardtype()
{
    return m_cardtype;
}

quint8 XArgDecode::getPDyear()
{
    return m_PDyear;
}

quint8 XArgDecode::getPDmon()
{
    return m_PDmon;
}

quint8 XArgDecode::getPDser()
{
    return m_PDser;
}

QString XArgDecode::getPDbatch()
{
    QString ret;
    ret.append("Y");
    if(m_PDyear < 10)
        ret.append("0");
    ret.append(QString::number(m_PDyear));
    if(m_PDmon < 10)
        ret.append("0");
    ret.append(QString::number(m_PDmon));
    if(m_PDser < 10)
        ret.append("00");
    else if(m_PDser < 100)
        ret.append("0");
    ret.append(QString::number(m_PDser));
    return ret;
}

quint8 XArgDecode::gethowpeak()
{
    quint8 ret = 0;
    for(quint32 i=0;i<6;i++)
    {
        if(m_peak[i].isusd)
            ret++;
    }
    return ret;
}

quint8 XArgDecode::getbdtype()
{
    return m_bdtype;
}

quint16 XArgDecode::getbdAPos()
{
    return m_bdAPos;
}

quint16 XArgDecode::getbdBPos()
{
    return m_bdBPos;
}

quint8 XArgDecode::getbcsera()
{
    return m_bcsera;
}

quint8 XArgDecode::getbcpeak()
{
    return m_bcpeak;
}

quint8 XArgDecode::getbcbagr()
{
    return m_bcbagr;
}

quint16 XArgDecode::getreatime()
{
    return m_reatime;
}

quint16 XArgDecode::getemptymin()
{
    if(m_peak[0].mf == 0)
    {
        return m_minarea;
    }
    else
        return m_minpeak;
}

Tn_INFO *XArgDecode::getTnPeak(quint8 peak)
{
    if(peak<6)
    {
        return &m_peak[peak];
    }
    else
        return &m_peak[0];
}

quint8 XArgDecode::getunite()
{
    return m_unite;
}

Pn_INFO *XArgDecode::getPnInfo(quint8 pn)
{
    if(pn<5)
    {
        return &m_pro[pn];
    }
    else
        return &m_pro[0];
}
/*
Tn_PEAK *XArgDecode::getTnPeak(quint8 peak)
{
    if(peak >= 6) return NULL;
    return &m_peak[peak];
}
*/



quint16 XArgDecode::char_to_short(QByteArray str)
{
    quint16 ret = 0;
    if(str.length() == 2)
    {
        ret = str.at(0);
        ret<<=8;
        ret+= (quint8)str.at(1);
    }
    return ret;
}

float XArgDecode::char_to_float(QByteArray str)
{
    float ret = 0;
    if(str.length() == 4)
    {
        memcpy(&ret,str.data(),4);
    }
    return ret;
}
quint16 XArgDecode::get_code_crc(QByteArray str)
{
    quint16 crc16;
    crc16 = qChecksum(str.constData(),str.length());
//    qDebug("Decode calculate crc16 = 0x%x ",crc16);
    return crc16;
}

bool XArgDecode::is_code_valid(QByteArray str)
{
    bool ret = true;
    quint16 strlen = str.length();
    if(strlen < 40)
    {
        ret = false;
    }
    else
    {
        if(strlen != char_to_short(str.mid(2,2)))
        {
            ret = false;
        }
        else
        {
            if(get_code_crc(str.left(strlen - 2)) != char_to_short(str.right(2)))
            {
                ret = false;
            }
        }
    }
    return ret;
}
#include <QTextCodec>
int XArgDecode::pf_decode(QByteArray str,PF_DATA *pftmp)
{
    quint8 lead = str.at(0);
    quint8 len = (lead&0xf)+1;
    quint8 stin = (lead&0x70)>>4;
    switch(stin)
    {
    case 0:
    {
        QTextCodec *gbk = QTextCodec::codecForName("GBK");
        QString g2u = gbk->toUnicode(str.mid(1,len));
        pftmp->proname = g2u;
        //qDebug()<<pftmp->proname;
    }break;
    case 1:
    {
        pftmp->prounit = str.mid(1,len);
        //qDebug()<<pftmp->prounit;
    }break;
    case 2:
    {

        pftmp->observedmin = char_to_float(str.mid(1,4));
        pftmp->observedmax = char_to_float(str.mid(1+4,4));
        //qDebug()<<pftmp->observedmin<<pftmp->observedmax;
    }break;
    case 3:
    {
        pftmp->refermin = char_to_float(str.mid(1,4));
        pftmp->refermax = char_to_float(str.mid(1+4,4));
        //qDebug()<<pftmp->refermin<<pftmp->refermax;
    }break;
    case 4:
    {
        pftmp->reactortime = char_to_short(str.mid(1,2));
        //qDebug()<<pftmp->reactortime;
    }break;
    default:
        break;
    }
    return len+1;
}

QString XArgDecode::getAllPnName(QString separate)
{
    QString ret;
    for(int i=0;i<m_unite;i++)
    {
        ret.append(m_pro[i].proinfo.proname);
        ret.append(separate);
        if(m_pro[i].proinfo.nextsection != NULL)
        {
            ret.append(m_pro[i].nextproinfo.proname);
            ret.append(separate);
        }
    }
    return ret.left(ret.length()-1);
}






