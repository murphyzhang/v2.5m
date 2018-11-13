#include "sysinfo.h"


CFG_ST CSysInfo::m_cfginfo[CFG_MAX_TYPE];


CSysInfo::CSysInfo(QObject *parent): QObject(parent)
{
}

void CSysInfo::power_on()
{
//    qDebug()<<"---------init CSysInfo-------------";
    init_cfg();
    QSettings settings(CFG_STORE_PATH, QSettings::IniFormat);
    settings.beginGroup(m_cfginfo[CFG_VERSION].group);

    if(settings.value(m_cfginfo[CFG_VERSION].ename).isNull())
    {
//        qDebug() << "CFG_VERSION value is null";

        settings.endGroup();
        settings.beginGroup("VERIFY");
        int ver = settings.value("verify").toInt();
        if((ver <= 50) || (ver >= 200))
        {
            ver  = 100;
        }
        settings.endGroup();

        settings.beginGroup("SYSINFO");
        QString mcsn = settings.value("MCSN").toString();
        if(mcsn.length()<10)
            mcsn = QString("BYIIZ00000001");
        settings.endGroup();
        settings.clear();

        set_cfg(CFG_FY_CORRECT, ver / 100.0);
        set_cfg(CFG_QT_SNCODE, mcsn);
    }
    else
    {
        updt_cfg(CFG_STORE_PATH);

        if(m_cfginfo[CFG_CHECK].vaule.toInt() != check_cfg())
        {
//            reset_cfg();
            qDebug() << "config: " CFG_STORE_PATH " check err! now version no reset";
        }
    }
}

void CSysInfo::init_cfg()
{
    qDebug() << "init m_cfginfo, default value";

    m_cfginfo[CFG_VERSION].cfgtp = CFG_VERSION;
    //m_cfginfo[CFG_VERSION].cname = QString(tr("配置文件版本"));
    m_cfginfo[CFG_VERSION].cname = QString(tr("File version"));
    m_cfginfo[CFG_VERSION].ename = QString("VERSION");
    m_cfginfo[CFG_VERSION].group = QString("INI_GROUP");
    m_cfginfo[CFG_VERSION].tacit = QVariant(QString("1.0"));
    m_cfginfo[CFG_VERSION].vaule = QVariant(QString("1.0"));

    m_cfginfo[CFG_FY_CORRECT].cfgtp = CFG_FY_CORRECT;
    //m_cfginfo[CFG_FY_CORRECT].cname = QString(tr("荧光检测校正"));
    m_cfginfo[CFG_FY_CORRECT].cname = QString(tr("Fluorescent correction"));
    m_cfginfo[CFG_FY_CORRECT].ename = QString("FY_CORRECT");
    m_cfginfo[CFG_FY_CORRECT].group = QString("FY_GROUP");
    m_cfginfo[CFG_FY_CORRECT].tacit = QVariant(1.00);
    m_cfginfo[CFG_FY_CORRECT].vaule = QVariant(1.00);

    m_cfginfo[CFG_FY_DETEDIR].cfgtp = CFG_FY_DETEDIR;
    //m_cfginfo[CFG_FY_DETEDIR].cname = QString(tr("荧光检测方向"));
    m_cfginfo[CFG_FY_DETEDIR].cname = QString(tr("Fluorescent direction"));
    m_cfginfo[CFG_FY_DETEDIR].ename = QString("FY_DETEDIR");
    m_cfginfo[CFG_FY_DETEDIR].group = QString("FY_GROUP");
    m_cfginfo[CFG_FY_DETEDIR].tacit = QVariant(1);
    m_cfginfo[CFG_FY_DETEDIR].vaule = QVariant(1);

    m_cfginfo[CFG_FY_DETESPE].cfgtp = CFG_FY_DETESPE;
    //m_cfginfo[CFG_FY_DETESPE].cname = QString(tr("荧光检测速度"));
    m_cfginfo[CFG_FY_DETESPE].cname = QString(tr("Detection speed of fluorescence"));
    m_cfginfo[CFG_FY_DETESPE].ename = QString("FY_DETESPE");
    m_cfginfo[CFG_FY_DETESPE].group = QString("FY_GROUP");
    m_cfginfo[CFG_FY_DETESPE].tacit = QVariant(400);
    m_cfginfo[CFG_FY_DETESPE].vaule = QVariant(400);

    m_cfginfo[CFG_FY_RESTSPE].cfgtp = CFG_FY_RESTSPE;
    //m_cfginfo[CFG_FY_RESTSPE].cname = QString(tr("荧光复位速度"));
    m_cfginfo[CFG_FY_RESTSPE].cname = QString(tr("Resetting speed of fluorescence"));
    m_cfginfo[CFG_FY_RESTSPE].ename = QString("FY_RESTSPE");
    m_cfginfo[CFG_FY_RESTSPE].group = QString("FY_GROUP");
    m_cfginfo[CFG_FY_RESTSPE].tacit = QVariant(200);
    m_cfginfo[CFG_FY_RESTSPE].vaule = QVariant(200);

    m_cfginfo[CFG_FY_TRAVEL].cfgtp = CFG_FY_TRAVEL;
    //m_cfginfo[CFG_FY_TRAVEL].cname = QString(tr("荧光检测行程"));
    m_cfginfo[CFG_FY_TRAVEL].cname = QString(tr("Fluorescent detection route"));
    m_cfginfo[CFG_FY_TRAVEL].ename = QString("FY_TRAVEL");
    m_cfginfo[CFG_FY_TRAVEL].group = QString("FY_GROUP");
    m_cfginfo[CFG_FY_TRAVEL].tacit = QVariant(4248);
    m_cfginfo[CFG_FY_TRAVEL].vaule = QVariant(4248);

    m_cfginfo[CFG_PD_RUNDIR].cfgtp = CFG_PD_RUNDIR;
    //m_cfginfo[CFG_PD_RUNDIR].cname = QString(tr("皮带运行方向"));
    m_cfginfo[CFG_PD_RUNDIR].cname = QString(tr("Strap running direction"));
    m_cfginfo[CFG_PD_RUNDIR].ename = QString("PD_RUNDIR");
    m_cfginfo[CFG_PD_RUNDIR].group = QString("PD_GROUP");
    m_cfginfo[CFG_PD_RUNDIR].tacit = QVariant(1);
    m_cfginfo[CFG_PD_RUNDIR].vaule = QVariant(1);

    m_cfginfo[CFG_PD_RUNSPE].cfgtp = CFG_PD_RUNSPE;
    //m_cfginfo[CFG_PD_RUNSPE].cname = QString(tr("皮带运行速度"));
    m_cfginfo[CFG_PD_RUNSPE].cname = QString(tr("Strap running speed"));
    m_cfginfo[CFG_PD_RUNSPE].ename = QString("PD_RUNSPE");
    m_cfginfo[CFG_PD_RUNSPE].group = QString("PD_GROUP");
    m_cfginfo[CFG_PD_RUNSPE].tacit = QVariant(31);
    m_cfginfo[CFG_PD_RUNSPE].vaule = QVariant(31);

    m_cfginfo[CFG_PD_RUNACC].cfgtp = CFG_PD_RUNACC;
    //m_cfginfo[CFG_PD_RUNACC].cname = QString(tr("皮带运行精度"));
    m_cfginfo[CFG_PD_RUNACC].cname = QString(tr("Strap running accuracy"));
    m_cfginfo[CFG_PD_RUNACC].ename = QString("PD_RUNACC");
    m_cfginfo[CFG_PD_RUNACC].group = QString("PD_GROUP");
    m_cfginfo[CFG_PD_RUNACC].tacit = QVariant(10);
    m_cfginfo[CFG_PD_RUNACC].vaule = QVariant(10);

    m_cfginfo[CFG_PD_INTERV].cfgtp = CFG_PD_INTERV;
    //m_cfginfo[CFG_PD_INTERV].cname = QString(tr("皮带间隔长度"));
    m_cfginfo[CFG_PD_INTERV].cname = QString(tr("Interval length of strap"));
    m_cfginfo[CFG_PD_INTERV].ename = QString("PD_INTERV");
    m_cfginfo[CFG_PD_INTERV].group = QString("PD_GROUP");
    m_cfginfo[CFG_PD_INTERV].tacit = QVariant(1600);
    m_cfginfo[CFG_PD_INTERV].vaule = QVariant(1600);

    m_cfginfo[CFG_PD_RESTSET].cfgtp = CFG_PD_RESTSET;
    //m_cfginfo[CFG_PD_RESTSET].cname = QString(tr("皮带复位补偿"));
    m_cfginfo[CFG_PD_RESTSET].cname = QString(tr("Resetting compensation of strap"));
    m_cfginfo[CFG_PD_RESTSET].ename = QString("PD_RESTSET");
    m_cfginfo[CFG_PD_RESTSET].group = QString("PD_GROUP");
    m_cfginfo[CFG_PD_RESTSET].tacit = QVariant(0);
    m_cfginfo[CFG_PD_RESTSET].vaule = QVariant(0);

    m_cfginfo[CFG_SM_QRLEND].cfgtp = CFG_SM_QRLEND;
    //m_cfginfo[CFG_SM_QRLEND].cname = QString(tr("条码检测长度"));
    m_cfginfo[CFG_SM_QRLEND].cname = QString(tr("Testing length of bar code"));
    m_cfginfo[CFG_SM_QRLEND].ename = QString("SM_QRLEND");
    m_cfginfo[CFG_SM_QRLEND].group = QString("SM_GROUP");
    m_cfginfo[CFG_SM_QRLEND].tacit = QVariant(6);
    m_cfginfo[CFG_SM_QRLEND].vaule = QVariant(6);

    m_cfginfo[CFG_SM_SCANTIM].cfgtp = CFG_SM_SCANTIM;
    //m_cfginfo[CFG_SM_SCANTIM].cname = QString(tr("条码扫描时间"));
    m_cfginfo[CFG_SM_SCANTIM].cname = QString(tr("Scanning time of bar code"));
    m_cfginfo[CFG_SM_SCANTIM].ename = QString("SM_SCANTIM");
    m_cfginfo[CFG_SM_SCANTIM].group = QString("SM_GROUP");
    m_cfginfo[CFG_SM_SCANTIM].tacit = QVariant(5000);
    m_cfginfo[CFG_SM_SCANTIM].vaule = QVariant(5000);

    m_cfginfo[CFG_JC_RUNDIR].cfgtp = CFG_JC_RUNDIR;
    //m_cfginfo[CFG_JC_RUNDIR].cname = QString(tr("进卡模块方向"));
    m_cfginfo[CFG_JC_RUNDIR].cname = QString(tr("Direction of inserting cartridge"));
    m_cfginfo[CFG_JC_RUNDIR].ename = QString("JC_RUNDIR");
    m_cfginfo[CFG_JC_RUNDIR].group = QString("JC_GROUP");
    m_cfginfo[CFG_JC_RUNDIR].tacit = QVariant(1);
    m_cfginfo[CFG_JC_RUNDIR].vaule = QVariant(1);

    m_cfginfo[CFG_JC_TIMEOVR].cfgtp = CFG_JC_TIMEOVR;
    //m_cfginfo[CFG_JC_TIMEOVR].cname = QString(tr("进卡超时时间"));
    m_cfginfo[CFG_JC_TIMEOVR].cname = QString(tr("Overtime of inserting cartridge"));
    m_cfginfo[CFG_JC_TIMEOVR].ename = QString("JC_TIMEOVR");
    m_cfginfo[CFG_JC_TIMEOVR].group = QString("JC_GROUP");
    m_cfginfo[CFG_JC_TIMEOVR].tacit = QVariant(30000);
    m_cfginfo[CFG_JC_TIMEOVR].vaule = QVariant(30000);

    m_cfginfo[CFG_JC_TIMESPE].cfgtp = CFG_JC_TIMESPE;
    //m_cfginfo[CFG_JC_TIMESPE].cname = QString(tr("进卡间隔时间"));
    m_cfginfo[CFG_JC_TIMESPE].cname = QString(tr("Interval time of inserting cartridge"));
    m_cfginfo[CFG_JC_TIMESPE].ename = QString("JC_TIMESPE");
    m_cfginfo[CFG_JC_TIMESPE].group = QString("JC_GROUP");
    m_cfginfo[CFG_JC_TIMESPE].tacit = QVariant(20000);
    m_cfginfo[CFG_JC_TIMESPE].vaule = QVariant(20000);

    m_cfginfo[CFG_JC_TIMEDLY].cfgtp = CFG_JC_TIMEDLY;
    //m_cfginfo[CFG_JC_TIMEDLY].cname = QString(tr("进卡延时时间"));
    m_cfginfo[CFG_JC_TIMEDLY].cname = QString(tr("Delay time of inserting cartridge"));
    m_cfginfo[CFG_JC_TIMEDLY].ename = QString("JC_TIMEDLY");
    m_cfginfo[CFG_JC_TIMEDLY].group = QString("JC_GROUP");
    m_cfginfo[CFG_JC_TIMEDLY].tacit = QVariant(2000);
    m_cfginfo[CFG_JC_TIMEDLY].vaule = QVariant(2000);

    m_cfginfo[CFG_JC_TIMEON].cfgtp = CFG_JC_TIMEON;
    //m_cfginfo[CFG_JC_TIMEON].cname = QString(tr("进卡开机运行"));
    m_cfginfo[CFG_JC_TIMEON].cname = QString(tr("Start up running"));
    m_cfginfo[CFG_JC_TIMEON].ename = QString("JC_TIMEON");
    m_cfginfo[CFG_JC_TIMEON].group = QString("JC_GROUP");
    m_cfginfo[CFG_JC_TIMEON].tacit = QVariant(2000);
    m_cfginfo[CFG_JC_TIMEON].vaule = QVariant(2000);

    m_cfginfo[CFG_QT_AUTOPRT].cfgtp = CFG_QT_AUTOPRT;
    //m_cfginfo[CFG_QT_AUTOPRT].cname = QString(tr("结束自动打印"));
    m_cfginfo[CFG_QT_AUTOPRT].cname = QString(tr("Print result automatically after test"));
    m_cfginfo[CFG_QT_AUTOPRT].ename = QString("QT_AUTOPRT");
    m_cfginfo[CFG_QT_AUTOPRT].group = QString("QT_GROUP");
    m_cfginfo[CFG_QT_AUTOPRT].tacit = QVariant(1);
    m_cfginfo[CFG_QT_AUTOPRT].vaule = QVariant(1);

    m_cfginfo[CFG_QT_PRTREF].cfgtp = CFG_QT_PRTREF;
    //m_cfginfo[CFG_QT_PRTREF].cname = QString(tr("结束自动打印"));
    m_cfginfo[CFG_QT_PRTREF].cname = QString(tr("Print result automatically after test"));
    m_cfginfo[CFG_QT_PRTREF].ename = QString("QT_PRTREF");
    m_cfginfo[CFG_QT_PRTREF].group = QString("QT_GROUP");
    m_cfginfo[CFG_QT_PRTREF].tacit = QVariant(0);
    m_cfginfo[CFG_QT_PRTREF].vaule = QVariant(0);

    m_cfginfo[CFG_QT_SNCODE].cfgtp = CFG_QT_SNCODE;
    //m_cfginfo[CFG_QT_SNCODE].cname = QString(tr("机器SN码"));
    m_cfginfo[CFG_QT_SNCODE].cname = QString(tr("Analyzer SN"));
    m_cfginfo[CFG_QT_SNCODE].ename = QString("QT_SNCODE");
    m_cfginfo[CFG_QT_SNCODE].group = QString("QT_GROUP");
    m_cfginfo[CFG_QT_SNCODE].tacit = QVariant(QString("BYIIZ00000000"));
    m_cfginfo[CFG_QT_SNCODE].vaule = QVariant(QString("BYIIZ00000000"));

    m_cfginfo[CFG_QT_CSVERIFY].cfgtp = CFG_QT_CSVERIFY;
    //m_cfginfo[CFG_QT_CSVERIFY].cname = QString(tr("卡槽校正值"));
    m_cfginfo[CFG_QT_CSVERIFY].cname = QString(tr("Slot correction value"));
    m_cfginfo[CFG_QT_CSVERIFY].ename = QString("QT_CSVERIFY");
    m_cfginfo[CFG_QT_CSVERIFY].group = QString("QT_GROUP");
    m_cfginfo[CFG_QT_CSVERIFY].tacit = QVariant(0);
    m_cfginfo[CFG_QT_CSVERIFY].vaule = QVariant(0);

    m_cfginfo[CFG_QT_TVERIFY].cfgtp = CFG_QT_TVERIFY;
    //m_cfginfo[CFG_QT_TVERIFY].cname = QString(tr("台间差校正值"));
    m_cfginfo[CFG_QT_TVERIFY].cname = QString(tr("Technology verify"));
    m_cfginfo[CFG_QT_TVERIFY].ename = QString("QT_TVERIFY");
    m_cfginfo[CFG_QT_TVERIFY].group = QString("QT_GROUP");
    m_cfginfo[CFG_QT_TVERIFY].tacit = QVariant(QString("0,1,0,0"));
    m_cfginfo[CFG_QT_TVERIFY].vaule = QVariant(QString("0,1,0,0"));

    m_cfginfo[CFG_QT_TWC].cfgtp = CFG_QT_TWC;
    m_cfginfo[CFG_QT_TWC].cname = QString(tr("Test interface batch number display"));
    m_cfginfo[CFG_QT_TWC].ename = QString("QT_TWC");
    m_cfginfo[CFG_QT_TWC].group = QString("QT_GROUP");
    m_cfginfo[CFG_QT_TWC].tacit = QVariant(1);
    m_cfginfo[CFG_QT_TWC].vaule = QVariant(1);

    m_cfginfo[CFG_QT_RWC].cfgtp = CFG_QT_RWC;
    m_cfginfo[CFG_QT_RWC].cname = QString(tr("Sample record batch number display"));
    m_cfginfo[CFG_QT_RWC].ename = QString("QT_RWC");
    m_cfginfo[CFG_QT_RWC].group = QString("QT_GROUP");
    m_cfginfo[CFG_QT_RWC].tacit = QVariant(1);
    m_cfginfo[CFG_QT_RWC].vaule = QVariant(1);

    m_cfginfo[CFG_UPDT_TIME].cfgtp = CFG_UPDT_TIME;
    //m_cfginfo[CFG_UPDT_TIME].cname = QString(tr("文件修改时间"));
    m_cfginfo[CFG_UPDT_TIME].cname = QString(tr("Modified date of file"));
    m_cfginfo[CFG_UPDT_TIME].ename = QString("UPDT_TIME");
    m_cfginfo[CFG_UPDT_TIME].group = QString("INI_GROUP");
    m_cfginfo[CFG_UPDT_TIME].tacit = QVariant(QDateTime::currentDateTime());
    m_cfginfo[CFG_UPDT_TIME].vaule = QVariant(QDateTime::currentDateTime());

    m_cfginfo[CFG_CHECK].cfgtp = CFG_CHECK;
    //m_cfginfo[CFG_CHECK].cname = QString(tr("文件校验值"));
    m_cfginfo[CFG_CHECK].cname = QString(tr("Proof test value of file"));
    m_cfginfo[CFG_CHECK].ename = QString("CHECK");
    m_cfginfo[CFG_CHECK].group = QString("INI_GROUP");
    m_cfginfo[CFG_CHECK].tacit = QVariant(check_cfg());
    m_cfginfo[CFG_CHECK].vaule = QVariant(check_cfg());
}

void CSysInfo::updt_cfg(QString path)
{
    qDebug() << "read config file" << path;

    QSettings settings(path, QSettings::IniFormat);

    for(int i = 0; i < CFG_MAX_TYPE; i++)
    {
        settings.beginGroup(m_cfginfo[i].group);

        if(settings.value(m_cfginfo[i].ename).isNull())
        {
            settings.setValue(m_cfginfo[i].ename,m_cfginfo[i].vaule);
        }

        m_cfginfo[i].vaule = settings.value(m_cfginfo[i].ename);
        settings.endGroup();
#if 0
        qDebug() << m_cfginfo[i].ename << "=" << m_cfginfo[i].vaule.toString();
#endif
    }
}

void CSysInfo::save_cfg(QString path)
{
    QSettings settings(path, QSettings::IniFormat);

    for(int i = 0; i < CFG_MAX_TYPE; i++)
    {
        settings.beginGroup(m_cfginfo[i].group);
        settings.setValue(m_cfginfo[i].ename, m_cfginfo[i].vaule);
        settings.endGroup();
    }

    //settings.sync();
}

unsigned int CSysInfo::check_cfg()
{
    unsigned int rtn=0;
    QString tmp;

    for(int i = 0; i < CFG_MAX_TYPE - 1; i++)
    {
        tmp.append(m_cfginfo[i].vaule.toString());
    }

    QString md5 = QCryptographicHash::hash (tmp.toAscii(), QCryptographicHash::Md5 ).toHex();

    unsigned int in=0;

    for(int i = 0; i < 6; i++)
    {
        bool ok;
        in = md5.left(5*(i+1)).right(5).toInt(&ok,16);
        rtn ^= in;
    }

    rtn = rtn%900000+100000;
    //qDebug()<<"check_cfg"<<rtn;
    return rtn;
}

void CSysInfo::reset_cfg()
{
    qDebug() << "reset config file";

    updt_cfg(CFG_BACKUP_PATH);

    if(m_cfginfo[CFG_CHECK].vaule.toInt() != check_cfg())
    {
        qDebug() << CFG_BACKUP_PATH " check err";

        QSettings settings;
        settings.setPath(QSettings::IniFormat, QSettings::UserScope, CFG_BACKUP_PATH);
        settings.clear();

        settings.setPath(QSettings::IniFormat, QSettings::UserScope, CFG_STORE_PATH);
        settings.clear();
        init_cfg();
        save_cfg(CFG_BACKUP_PATH);
        save_cfg(CFG_STORE_PATH);
    }
    else
    {
        qDebug()<<"recover for " CFG_BACKUP_PATH;

//        updt_cfg(CFG_BACKUP_PATH);
        QSettings settings;
        settings.setPath(QSettings::IniFormat,QSettings::UserScope,CFG_STORE_PATH);
        settings.clear();
        save_cfg(CFG_STORE_PATH);
    }
}

QVariant CSysInfo::get_cfg(CFGTYPE type)
{
    if(type < CFG_MAX_TYPE)
    {
        return m_cfginfo[type].vaule;
    }
    else
    {
        return QVariant(0);
    }
}

QString CSysInfo::get_cfgcname(CFGTYPE type)
{
    if(type < CFG_MAX_TYPE)
    {
        return m_cfginfo[type].cname;
    }
    else
    {
        return "---";
    }
}

bool CSysInfo::set_cfg(CFGTYPE type, QVariant vaule)
{
    if(type < CFG_MAX_TYPE)
    {
        if(m_cfginfo[type].vaule.toString() != vaule.toString())
        {
            m_cfginfo[type].vaule = vaule;
            m_cfginfo[CFG_UPDT_TIME].vaule = QVariant(QDateTime::currentDateTime());
            m_cfginfo[CFG_CHECK].vaule = QVariant(check_cfg());

            QSettings settings;
            settings.setPath(QSettings::IniFormat,QSettings::UserScope,CFG_BACKUP_PATH);
            settings.clear();
            save_cfg(CFG_BACKUP_PATH);

            settings.setPath(QSettings::IniFormat,QSettings::UserScope,CFG_STORE_PATH);
            settings.clear();
            save_cfg(CFG_STORE_PATH);

            return true;
        }
    }

    return false;
}

int CSysInfo::get_cfgnum()
{
    return CFG_MAX_TYPE;
}



