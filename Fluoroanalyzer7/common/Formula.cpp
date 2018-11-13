#include "Formula.h"
#include "common/configurate.h"

CFormula::CFormula()
{


}
CFormula::CFormula(unsigned int *data,QByteArray &qrcode)
{

    Set_FLdata(data);
    Set_QRcode(qrcode);
}
void CFormula::Set_FLdata(unsigned int *data)
{
    if(data != NULL)
    {
        for(int i=0;i<DATA_MAX_SIZE;i++)
        {
            m_data[i] = data[i];
        }
    }
}

void CFormula::Set_QRcode(QByteArray qrcode)
{
    m_code.set_newcode(qrcode);
}

XArgDecode * CFormula::Get_QRcodeinfo()
{
    return &m_code;
}

#if 1
/********************************************************************************/
/*找到本底最小值*/
/********************************************************************************/
int CFormula::FindMinbd(int nstart, int nstop, unsigned int *data)
{
    if(nstart > nstop)
    {
        int tmp = nstart;
        nstart = nstop;
        nstop = tmp;
    }
    if((nstart<=MIN_POS)||(nstop>=MAX_POS))
        return 0;
    //int area = nstop - nstart + 1;
    int minpos = nstart;
    for(int i=nstart;i<=nstop;i++)
    {
        if(data[i] < data[minpos])
        {
            minpos = i;
        }
    }
    return minpos;
}
/********************************************************************************/
/*找到区间最大值*/
/********************************************************************************/
int  CFormula::FindMaxArea(int nPos,int area,unsigned int *data)
{
    int nStartPos = 0;
    int nEndPos   = 0;
    int nMaxPos = 0;
    int nMaxVar = 0;
    if((nPos<=MIN_POS)||(nPos>=MAX_POS))
        return 0;
    nStartPos =  nPos - area/2;
    nEndPos   =  nPos + area/2;
    if(nStartPos <  MIN_POS ) nStartPos = MIN_POS;
    if(nEndPos   >  MAX_POS)  nEndPos   = MAX_POS;
    nMaxVar = data[nStartPos];
    nMaxPos = nStartPos;
    for(int nIndex = nStartPos;  nIndex < nEndPos; nIndex++)
    {
        if(nMaxVar < data[nIndex])
        {
            nMaxVar = data[nIndex];
            nMaxPos = nIndex;
        }
    }
//    qDebug()<<"FindMaxArea at "<<nMaxPos;
    return nMaxPos;
}
/********************************************************************************/
/*找到区间最小值*/
/********************************************************************************/
int  CFormula::FindMinArea(int nPos,int area,unsigned int *data)
{
    int nStartPos = 0;
    int nEndPos   = 0;
    int nMinPos = 0;
    int nMinVar = 0;
    if((nPos<=MIN_POS)||(nPos>=MAX_POS))
        return 0;
    nStartPos =  nPos - area/2;
    nEndPos   =  nPos + area/2;
    if(nStartPos <  MIN_POS ) nStartPos = MIN_POS;
    if(nEndPos   >  MAX_POS)  nEndPos   = MAX_POS;
    nMinVar = data[nStartPos];
    nMinPos = nStartPos;
    for(int nIndex = nStartPos;  nIndex < nEndPos; nIndex++)
    {
        if(nMinVar > data[nIndex])
        {
            nMinVar = data[nIndex];
            nMinPos = nIndex;
        }
    }
//    qDebug()<<"FindMinArea at "<<nMinPos;
    return nMinPos;
}
/********************************************************************************/
/*计算本底值 AB最低点*/
/********************************************************************************/
double  CFormula::DataHORAB(int nMinPos,int area,unsigned int *data)
{
    if((nMinPos<=MIN_POS)||(nMinPos>=MAX_POS))
        return 0;
    double dBaseAverage = 0.0;
    int nMinForPos =  nMinPos - area/2;
    int nMaxForPos =  nMinPos + area/2;
    if(nMinForPos <  MIN_POS ) nMinForPos = MIN_POS;
    if(nMaxForPos >  MAX_POS)  nMaxForPos = MAX_POS;
    int nCurBase = 0; // 实际计算的面积 (去掉边界)
    for(int nIndex = nMinForPos; nIndex < nMaxForPos; nIndex++)
    {
        dBaseAverage += data[nIndex];
        nCurBase++;
    }
    dBaseAverage /= nCurBase/*g_nBase*/;

//    qDebug()<<"DataHORAB = "<<dBaseAverage;
    return dBaseAverage;
}
/********************************************************************************/
/*计算本底值 A点*/
/********************************************************************************/
double  CFormula::DataHORAP(int nPosA,int area,unsigned int *data)
{
    if((nPosA<=MIN_POS)||(nPosA>=MAX_POS))
        return 0;
    double dBaseAverage = 0.0;
    int nMinPos = nPosA;
    int nMaxPos = nPosA + area;
    if(nMinPos <  MIN_POS ) nMinPos = MIN_POS;
    if(nMaxPos >  MAX_POS)  nMaxPos  = MAX_POS;
    int nCurBase = 0; // 实际计算的面积 (去掉边界)
    for(int nIndex = nMinPos;nIndex < nMaxPos;nIndex++)
    {
        dBaseAverage += data[nIndex];
        nCurBase++;
    }
    dBaseAverage /= nCurBase;
//    qDebug()<<"DataHORAP = "<<dBaseAverage;
    return dBaseAverage;
}
/********************************************************************************/
/*计算T1...T6的 值*/
/********************************************************************************/
double CFormula::DataDealTN(int aol,int nMaxPos,int area,double dBase,unsigned int *data)
{
    if((nMaxPos<=MIN_POS)||(nMaxPos>=MAX_POS))
        return 0;
    double dEnergy = 0.0;
    double dOneCVal = 0.0;
    if(aol == 0) //面积法
    {
        int nMinForPos =  nMaxPos - area/2;
        int nMaxForPos =  nMaxPos + area/2;
        if(nMinForPos <  MIN_POS ) nMinForPos = MIN_POS;
        if(nMaxForPos >  MAX_POS)  nMaxForPos  = MAX_POS;
        for(int nIndex = nMinForPos; nIndex < nMaxForPos; nIndex++)
        {

            dOneCVal = data[nIndex] - dBase;
            if(dOneCVal < 0.0) dOneCVal = 0;
            dEnergy += dOneCVal;

        }
        if(dEnergy == 0) dEnergy = 10;
    }
    else
    {
        if((nMaxPos >= MIN_POS) && (nMaxPos <= MAX_POS))
        {
            dOneCVal = data[nMaxPos] - dBase;
            if(dOneCVal < 0.0) dOneCVal = 0;
            dEnergy = dOneCVal;
        }
        if(dEnergy == 0) dEnergy = 1;
    }
    return dEnergy;
}
/********************************************************************************/
/*预处理函数*/
/*tres:       前期处理得到的各个峰值（面积或者峰高）*/
/*numerator：预处理公式的分子参数*/
/*denominator：预处理公式的分母参数*/
/*isLOGten:   计算结果是否取对数*/

/*返回值：预处理结果*/
/********************************************************************************/
double CFormula::TNtoDTY(DT_ARG_t *tres,quint8 numerator,quint8 denominator, bool isLOGten)
{
    double ret = 0;
    double nt = 0,dt = 0;
    for(int i=0;i<6;i++)
    {
        if(((numerator>>i)&0x01) != 0)
            nt+=tres[i].dvT;
        if(((denominator>>i)&0x01) != 0)
            dt+=tres[i].dvT;
    }
    if(dt == 0)
        ret = nt*25/5242.87;
    else
        ret = nt/dt;

    if(isLOGten)
        ret = log(ret)/log(10);

    return ret;
}



/********************************************************************************/
/*公式判断*/
/********************************************************************************/
int CFormula::DTWitchOne(double fyres, double thup,double thdw)
{
    if(fyres >= thup)
    {
        return 1;
    }
    else if(fyres <= thdw)
    {
        return 3;
    }
    else
    {
        return 2;
    }
}
/********************************************************************************/
/*结果处理函数*/
/********************************************************************************/
QString CFormula::EndResult(double fyres, FOL_FUN *arg)
{
    QString res;
    double dResult = 0.0;
    switch(arg->nfun)
    {
    case 0: dResult = fyres;break;
    case 1:
    {
        if(arg->avalue == 0)
            res = QString("ERR");
        else
            dResult = (fyres - arg->bvalue)/arg->avalue;
        break;
    }
    case 2:
    {
        if((arg->avalue == 0)||(arg->bvalue == 0)||(arg->cvalue == 0))
            res = QString("ERR");
        else
            dResult = ( log((fyres-arg->dvalue)/arg->avalue)/log(arg->bvalue))/ arg->cvalue;
        break;
    }
    case 3:
    {
        if(arg->avalue == 0)
            res = QString("ERR");
        else
            dResult = pow(arg->bvalue, (fyres-arg->cvalue)/arg->avalue) ;
        break;
    }
    case 4:
    {
        if((arg->avalue == 0) || (arg->bvalue == 0))
            res = QString("ERR");
        else
            dResult = pow((fyres-arg->cvalue)/arg->avalue, 1/arg->bvalue);
        break;
    }
    case 5:
    {
        if((fyres == 1) || (arg->avalue == 0))
            res = QString("ERR");
        else
            dResult = pow(10, (log(fyres/(1-fyres))/log(10) - arg->bvalue) / arg->avalue);
        break;
    }
    case 6:
    {
        if((fyres == arg->dvalue) || (arg->avalue == 0))
            res = QString("ERR");
        else
            dResult = ( arg->bvalue - log((arg->dvalue-fyres)/(fyres-arg->cvalue))/log(10)) / arg->avalue;
        break;
    }
    case 7:
    {
        res = QString("ERR");
        break;
    }
    case 8:
    {
        if((fyres == arg->dvalue) || (arg->bvalue == 0))
            res = QString("ERR");
        else
            dResult = arg->cvalue * pow((arg->avalue-fyres)/(fyres-arg->dvalue), 1/arg->bvalue);
        break;
    }
    default: break;
    }

//    qDebug() << "dResult = "<<dResult;

    if(res.isEmpty())
    {
        if((!isnan(dResult))&&(!isinf(dResult)))
        {
            //if(dResult > 0)
            {
                if(arg->isrelog == true)
                    dResult = pow(10, dResult);
            }

#if defined(LONG_END_RESULT)
            int tmp = dResult * 1000.0;
            dResult = tmp / 1000.0;
#else
            int tmp = dResult * 100.0;
            dResult = tmp / 100.0;
#endif
            res = QString::number(dResult, 'g', 8);
        }
        else
            res = QString("ERR");
    }

    //    qDebug() << "dResult = "<<dResult;
    return res;
}


#endif

void CFormula::CalculateRes()
{
    /*清空结果*/
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<4;j++)
            m_DTFYres[i].DTY[j] = 0;
        m_DTFYres[i].foll = 0;
        m_DTFYres[i].fun = 0;
        m_DTFYres[i].result.clear();
    }

    //if(m_code.getcardtype() == 0)
    {
        if(m_code.getbdtype() == 0)
        {
            m_baseminpos = m_code.getbdAPos();
            m_baseres = DataHORAP(m_code.getbdAPos(),m_code.getbcbagr(),m_data);
        }
        else
        {
            m_baseminpos = FindMinbd(m_code.getbdAPos(),m_code.getbdBPos(),m_data);
            m_baseres = DataHORAB(m_baseminpos,m_code.getbcbagr(),m_data);
        }
    }

    for(int i=0;i<6;i++)
    {
        if(m_code.getTnPeak(i)->isusd)
        {
            m_DTres[i].maxPOS = FindMaxArea(m_code.getTnPeak(i)->coord,m_code.getbcsera(),m_data);
            m_DTres[i].value = m_data[m_DTres[i].maxPOS];
            m_DTres[i].dvT = DataDealTN(m_code.getTnPeak(i)->mf,m_DTres[i].maxPOS,m_code.getbcpeak(),m_baseres,m_data);
        }
        else
        {
            m_DTres[i].maxPOS = 0;
            m_DTres[i].value = 0;
            m_DTres[i].dvT = 0;
        }
    }
    /*是否空白卡*/
    int minval = m_code.getemptymin()*(524287.0/2500.0);
    if((m_DTres[0].dvT < minval) && (m_DTres[1].dvT < minval) && (m_DTres[2].dvT < minval) &&\
       (m_DTres[3].dvT < minval) && (m_DTres[4].dvT < minval) && (m_DTres[5].dvT < minval))
    {
        for(int i=0;i<5;i++)
            m_DTFYres[i].result = QString("NULL");
    }
    else
    {
//        qDebug()<<"unite = "<<m_code.getunite();
        for(int i=0;i<m_code.getunite();i++)
        {
            Pn_INFO *tmp_pi = m_code.getPnInfo(i);
            if(tmp_pi->isusd)
            {
                /*计算四个预处理的结果*/
                for(int j=0;j<4;j++)
                {
                    m_DTFYres[i].DTY[j] = TNtoDTY(m_DTres,tmp_pi->pre[j].numerator,\
                                                  tmp_pi->pre[j].denominator,\
                                                  tmp_pi->pre[j].islog);
                }
                /*预处理选择*/
                m_DTFYres[i].foll = DTWitchOne(m_DTFYres[i].DTY[0],tmp_pi->thup,tmp_pi->thdw);

                /*有设置有效值*/
                if(tmp_pi->islimit)
                {
                    if(tmp_pi->dtway == 0) //递增
                    {
                        if(m_DTFYres[i].DTY[m_DTFYres[i].foll] < tmp_pi->limdw)
                        {
                            m_DTFYres[i].result = "<"+QString::number(tmp_pi->proinfo.observedmin);
                        }
                        else if(m_DTFYres[i].DTY[m_DTFYres[i].foll] > tmp_pi->limup)
                        {
                            m_DTFYres[i].result = ">"+QString::number(tmp_pi->proinfo.observedmax);
                        }
                    }
                    else
                    {
                        if(m_DTFYres[i].DTY[m_DTFYres[i].foll] < tmp_pi->limdw)
                        {
                            m_DTFYres[i].result = ">"+QString::number(tmp_pi->proinfo.observedmax);
                        }
                        else if(m_DTFYres[i].DTY[m_DTFYres[i].foll] > tmp_pi->limup)
                        {
                            m_DTFYres[i].result = "<"+QString::number(tmp_pi->proinfo.observedmin);
                        }
                    }
                }
                /*计算结果*/
                if(m_DTFYres[i].result.isEmpty())
                {
                    FOL_FUN *fun = &tmp_pi->fol[m_DTFYres[i].foll-1];
                    m_DTFYres[i].fun = fun->nfun;
                    m_DTFYres[i].result = EndResult(m_DTFYres[i].DTY[m_DTFYres[i].foll],fun);
                    if(m_DTFYres[i].result != "ERR")
                    {
                        /*
                        if(m_DTFYres[i].result.toDouble() < tmp_pi->proinfo.observedmin)
                            m_DTFYres[i].result = "<"+QString::number(tmp_pi->proinfo.observedmin);
                        else if(m_DTFYres[i].result.toDouble() > tmp_pi->proinfo.observedmax)
                            m_DTFYres[i].result = ">"+QString::number(tmp_pi->proinfo.observedmax);
                        */
                    }
                }
//                qDebug()<<"result = "<<m_DTFYres[i].result;
            }
        }
    }

}

double CFormula::Get_BaseRes()
{
    return m_baseres;
}

int CFormula::Get_BasePos()
{
    return m_baseminpos;
}


DT_ARG_t* CFormula::Get_CTRES(int tn)
{
    if(tn < 6)
    {
        return &m_DTres[tn];
    }
    return &m_DTres[0];
}

DTFY_ARG_t *CFormula::Get_FYRES(int pro)
{
    if(pro < 5)
    {
        return &m_DTFYres[pro];
    }
    return &m_DTFYres[0];
}

QString CFormula::Get_ResStr(int pro)
{
    if(pro < 5)
    {
        return m_DTFYres[pro].result;
    }

    return m_DTFYres[0].result;
}



