#ifndef GLOBAL_DEFINE_H
#define GLOBAL_DEFINE_H



#include <QString>
#include <QTime>
typedef enum _enum_widget_
{
    MAIN_WG=0,    //主页面（背景）
    INIT_WG,    //初始化页面
    CHECK_WG,    //自检页面
    WAIT_WG,    //一级菜单页面
    DETET_WG,    //检测页面
    CALUT_WG,   //等待计算和存储页面
    RES1_WG,    //
    RES2_WG,
    RES4_WG,
    RES5_WG,
    YG_WG,      //荧光曲线           25
    MENU_WG,    //二级菜单页面
    SET_WG,     //系统设置页面
    RECD_WG,    //历史记录选择页面
    SAMP_WG,    //历史样本页面
    BATCH_WG,    //历史批号页面
    MODE_WG,    //测试模式选择页面
    INCAD_WG,    //等待插卡页面
    INCHP_WG,    //等待插芯片页面
    INBT_WG,    //输入批号页面
    SCAN_WG,    //扫描条码页面
    DELEY_WG,    //标准测试延时页面
    TIME_WG,    //时间修改页面
    VERI_WG,    //校验参数修改页面
    ABOUT_WG,   //关于页面
    PATIENT_INFO_WG, //用户信息编辑界面
    DETAIL_TEST_WG,  //详细测试信息
    CURETRACE_WG,       //治疗跟踪
    PLOTLINES_WG,
    ALL_WG
}ENUM_WIDGET;

typedef struct _PF_DATA_
{
    int pronum;         //项目标号
    QString proname;      //项目名称
    QString prounit;      //项目单位
    int reactortime;      //反应时间
    float refermin;         //参考值最小值
    float refermax;         //参考值最大值
    float observedmin;      //最小测量值
    float observedmax;      //最大测量值
    QString prochina;     //中文名
    struct _PF_DATA_ *nextsection;//第二段信息
}PF_DATA;

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




typedef struct _pro_res_
{
    QString name;
    QString result;         //浓度值
    QString unit;
    QString refer;
}PRO_RES;

typedef struct _patient_
{
    bool m_bIsSave;
    QString m_sID;
    QString m_sName;
    QString m_sSex;
    QString m_sAge;
    QString m_sSmpNum;
    QString m_sSmpType;
}PATIENT_T;

typedef struct _res_data_
{
    QString swinum;
    QString batch;
    int unite;
    PRO_RES pro[6];
    QDateTime endtime;     //测试结束时间
    QByteArray code;
    QString notes;
    QDateTime updatedtime; //更新时间
    PATIENT_T patient;
}RES_DATA;

typedef enum
{
    ERR_NONE,       // MSP430 answer
    ERR_AD_TIMEOUT, // MSP430 answer ad timeout 001
    ERR_OLS,        // MSP430 answer outside limit switch error 002
    ERR_ILS,        // MSP430 answer inside limit switch error  003
    ERR_WAIT_TIMEOUT,// 发送0x02指令测试,等待答复超时    004
    ERR_INVALID_CARD,// 无效卡            005
    ERR_CARD_BIAS,   // 试纸偏移          006

    /*************************在此处增加*************************/

    ERR_POWER_ON_SELF_CHECK_TIMEOUT, //发送自检命令，采集板答复超时  007
    ERR_ARGS_INVALID,           // 参数错误          008
    ERR_CHECK_COM_TIMEOUT,      //0x08指令通讯测试，采集板答复超时    009
    ERR_UPDATE_CARD_SLOT_VERIFY,//MSP430更新卡槽校正值失败   010

    /**********************************************************/

    ERR_UNKNOW,
    ERR_TYPE_MAX
}ERR_NO;

#endif // GLOBAL_DEFINE_H
