#ifndef FORMULA_H
#define FORMULA_H

#include <stdio.h>
#include <QtCore>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <QDebug>

#include "argdecode.h"



#define DATA_MAX_SIZE   (600) // 数据的组数
#define MIN_POS   (1)
#define MAX_POS   (DATA_MAX_SIZE - 1)

#define RES_CAL_ERR -3 //无计算结果
#define RES_ARG_ERR -2 //公式参数错误
#define RES_LIM_ERR -1 //结果超过测试极限值

typedef struct _FORMULA_ARG_T_
{
    double d_a;
    double d_b;
    double d_c;
    double d_d;
}FORMULA_ARG_t;

typedef struct _DTFY_ARG_T_
{
    double DTY[4];  //预处理1,2,3,4公式结果
    int foll;
    int fun;
    QString result;
}DTFY_ARG_t;

typedef struct _DT_ARG_T_
{
    int maxPOS;
    int value;
    double dvT;
}DT_ARG_t;


class CFormula
{

public:


    CFormula();
    CFormula(unsigned int *data,QByteArray &qrcode);
    void Set_FLdata(unsigned int *data);
    void Set_QRcode(QByteArray qrcode);
    XArgDecode* Get_QRcodeinfo();

    int FindMinbd(int nstart,int nstop,unsigned int *data);
    int FindMaxArea(int nPos, int area, unsigned int *data);
    int FindMinArea(int nPos, int area, unsigned int *data);
    double DataHORAB(int nMinPos, int area, unsigned int *data);
    double DataHORAP(int nPosA, int area, unsigned int *data);
    double DataDealTN(int aol, int nMaxPos, int area, double dBase, unsigned int *data);

    double TNtoDTY(DT_ARG_t *tres,quint8 numerator,quint8 denominator,bool isLOGten);

    int DTWitchOne(double fyres, double thup,double thdw);
    QString EndResult(double fyres, FOL_FUN *arg);


    void CalculateRes();
    double Get_BaseRes();
    int Get_BasePos();
    QString Get_ResStr(int pro);
    DT_ARG_t* Get_CTRES(int tn);
    DTFY_ARG_t* Get_FYRES(int pro);

private:
    unsigned int m_data[DATA_MAX_SIZE];
    int m_baseminpos;
    double m_baseres;
    DT_ARG_t m_DTres[6];
    DTFY_ARG_t m_DTFYres[5];
    int m_nfoll;

    XArgDecode m_code;

};

#endif // FORMULA_H
