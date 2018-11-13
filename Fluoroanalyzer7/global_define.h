#ifndef GLOBAL_DEFINE_H
#define GLOBAL_DEFINE_H

#include <QString>
#include <QTime>
typedef enum _enum_widget_
{
    MAIN_WG,
    INIT_WG,
    CHECK_WG,
    WAIT_WG,
    DETET_WG,
    MENU_WG,
    TIME_WG,
    VERI_WG,
    ALL_WG
}ENUM_WIDGET;

typedef struct _proinfo_data_
{
    int pronum;         //项目标号
    QString proname;      //项目名称
    QString prounit;      //项目单位
    int observedmin;      //最小测量值
    int observedmax;      //最大测量值
    int reactortime;      //反应时间
    QString prorefer;     //项目参考值
    QString prochina;     //中文名
    struct _proinfo_data_ *nextsection;//第二段信息
}PROINFO_DATA;

typedef enum _enum_detet_
{
    NULLCARD = 0,   //无卡
    INCARDING,      //进卡中
    WAITQRSCAN,     //等待扫描条码
    QRSCANING,      //正在扫描
    QRSCANERR,      //条码扫描不到
    PROCLASH,       //项目冲突
    REACTION,       //等待反应
    WAITDETET,      //等待测试
    DETETING,       //正在测试
    DETETEND,       //测试完成
    STATEALL
}ENUM_DETET;


typedef struct _st_tab_data_
{
    int slotid;         //槽编号
    int sernum;         //序号
    QString proname;    //项目
    QString batnum;     //批号
    QString state;      //状态
    QString time;       //剩余时间
    QString result;     //结果
    QString swinum;     //流水号
}ST_TAB_DATA;

typedef struct _result_data_
{
    int result;         //浓度值
    int C1_res;         //C1值
    int C2_res;         //
    int T1_res;         //
    int T2_res;         //
    unsigned int data[600];      //原始数据
}RESULT_DATA;

typedef struct _slot_data_
{
    int slotid;           //当前槽编号     用于记录进卡的顺序和个数
    int nowpos;           //当前相对位置   用于表示相对于进卡位置的偏移
    bool loadingcard;     //是否有装卡
    int swinum;           //数据存储位置
    QTime inserttime;     //进卡时间
    int surplustime;      //剩余时间
    bool scancode;        //是否扫描条码
    QString qrcode;       //条码信息
    bool effectivecard;   //是否有效卡
    PROINFO_DATA *proinfo; //项目信息
    bool testend;         //是否已经测试
    QDateTime endtime;     //测试结束时间
    RESULT_DATA resdata;
    ENUM_DETET state;     //状态
}SLOT_DATA;

typedef struct _printf_data_
{
    int swinum;
    int result;         //浓度值
    QDateTime endtime;     //测试结束时间
    PROINFO_DATA *proinfo;
}PRINTF_DATA;


#endif // GLOBAL_DEFINE_H
