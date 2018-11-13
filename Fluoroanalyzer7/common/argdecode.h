#ifndef ARGDECODE_H
#define ARGDECODE_H
#include<QString>
#include "global_define.h"

typedef struct _Tn_INFO_
{
    bool isusd; //是否使用
    int coord;  //坐标
    int mf;     //峰高或面积
}Tn_INFO;

typedef struct _PRE_FUN_
{
    quint8 numerator;   //分子
    quint8 denominator; //分母
    bool islog;         //是否取对数
}PRE_FUN;

typedef struct _FOL_FUN_
{
    quint8 nfun;
    float avalue;   //
    float bvalue;   //
    float cvalue;   //
    float dvalue;   //
    bool isrelog;       //是否取反对数
}FOL_FUN;



typedef struct _Pn_INFO_
{
    bool isusd;         //是否使用
    PF_DATA proinfo;    //项目信息
    PF_DATA nextproinfo; //存储项目信息
    bool islimit;       //是否有有效值
    float thup;         //阈值上限
    float thdw;         //阈值下限
    float limup;        //有效值上限
    float limdw;        //有效值下限
    int dtway;          //比例方式
    PRE_FUN pre[4];
    FOL_FUN fol[3];
}Pn_INFO;



class XArgDecode
{
public:
    XArgDecode();
    XArgDecode(QByteArray &code);
    bool set_newcode(QByteArray &code);
    QByteArray get_codestr();
    bool arg_decode(QByteArray code);
    bool isvalid();
    bool isIESser();
    quint8 getcardtype();
    quint8 getPDyear();
    quint8 getPDmon();
    quint8 getPDser();
    QString getPDbatch();
    quint8 gethowpeak();
    quint8 getbdtype();
    quint16 getbdAPos();
    quint16 getbdBPos();
    quint8 getbcsera();
    quint8 getbcpeak();
    quint8 getbcbagr();
    quint16 getreatime();
    quint16 getemptymin();

    Tn_INFO* getTnPeak(quint8 peak);
    quint8 getunite();
    Pn_INFO* getPnInfo(quint8 pn);
    QString getAllPnName(QString separate);
private:
    quint16 char_to_short(QByteArray str);
    float char_to_float(QByteArray str);
    quint16 get_code_crc(QByteArray str);
    bool is_code_valid(QByteArray str);
    int pf_decode(QByteArray str,PF_DATA *pftmp);
private:
    QByteArray m_currcode;  //保存当前条码
    bool is_valid;          //条码是否有效
    quint8 m_cardtype;      //卡类型
    quint8 m_PDyear;        //生产年
    quint8 m_PDmon;         //生产月
    quint8 m_PDser;         //生产批次
    bool m_IESser;          //是否是修订版本
    quint8 m_bdtype;        //本底选取方式
    quint16 m_bdAPos;       //A点坐标
    quint16 m_bdBPos;       //B点坐标
    quint8 m_bcsera;        //查找范围
    quint8 m_bcpeak;        //峰平均数
    quint8 m_bcbagr;        //底平均数
    quint16 m_minarea;      //面积最小值
    quint16 m_minpeak;      //峰高最小值
    quint16 m_reatime;      //反应时间
    Tn_INFO m_peak[6];      //峰信息
    quint8 m_unite;         //联卡数
    Pn_INFO m_pro[5];      //最多5个项目信息


};

#endif // ARGDECODE_H
