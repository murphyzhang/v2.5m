#ifndef SYSINFO_H
#define SYSINFO_H

#include <QObject>
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QCryptographicHash>

#define CFG_STORE_PATH "/BioTime/Option.ini"
#define CFG_BACKUP_PATH "/BioTime/back.ini"


typedef enum
{
    CFG_VERSION = 0,//配置文件版本
    CFG_FY_CORRECT, //荧光检测校正
    CFG_FY_DETEDIR, //荧光检测方向
    CFG_FY_DETESPE, //荧光检测速度
    CFG_FY_RESTSPE, //荧光复位速度
    CFG_FY_TRAVEL,  //荧光检测行程
    CFG_PD_RUNDIR,  //皮带运行方向
    CFG_PD_RUNSPE,  //皮带运行速度
    CFG_PD_RUNACC,  //皮带运行精度
    CFG_PD_INTERV,  //皮带间隔长度
    CFG_PD_RESTSET, //皮带复位补偿
    CFG_SM_QRLEND,  //条码检测长度
    CFG_SM_SCANTIM, //条码扫描时间
    CFG_JC_RUNDIR,  //进卡模块方向
    CFG_JC_TIMEOVR, //进卡超时时间
    CFG_JC_TIMESPE, //进卡间隔时间
    CFG_JC_TIMEDLY, //进卡延时时间
    CFG_JC_TIMEON,  //进卡开机运行
    CFG_QT_AUTOPRT, //结束自动打印
    CFG_QT_PRTREF,  //打印参考信息
    CFG_QT_SNCODE,  //机器SN码
    CFG_QT_CSVERIFY,//卡槽校正值    card slot verify
    CFG_QT_TVERIFY,  //台间差校正值
    CFG_QT_TWC,     //测试界面切换批号和ID显示      1: 批号  0: ID
    CFG_QT_RWC,     //样本记录界面切换批号和ID显示   1: 批号  0: ID
    CFG_UPDT_TIME,  //文件修改时间
    CFG_CHECK,      //文件校验值
    CFG_MAX_TYPE
}CFGTYPE;

typedef struct CFG_Struct
{
    CFGTYPE cfgtp;  //枚举绑定
    QString cname;  //中文名称
    QString ename;  //英文名称
    QString group;  //保存组
    QVariant tacit; //默认值
    QVariant vaule; //配置值
}CFG_ST;

class CSysInfo : public QObject
{
    Q_OBJECT
public:
    explicit CSysInfo(QObject *parent = 0);
    static void power_on();    //开机执行
    static void init_cfg();    //初始化配置文件 全部重置默认值
    static void updt_cfg(QString path); //更新指定配置文件到内存
    static void save_cfg(QString path); //将内存配置保存到指定文件
    static unsigned int check_cfg();   //获取配置文件校验值
    static void reset_cfg();   //重置配置文件
    static QVariant get_cfg(CFGTYPE type); //获取配置信息
    static QString get_cfgcname(CFGTYPE type); //获取中文信息
    static bool set_cfg(CFGTYPE type,QVariant vaule); //修改配置信息
    static int get_cfgnum(); //获取配置项数
signals:

public slots:

private:
    static CFG_ST m_cfginfo[CFG_MAX_TYPE];
};

#endif // SYSINFO_H
