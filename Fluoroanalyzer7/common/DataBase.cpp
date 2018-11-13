/******************************************************************************
*     数据模块的实现文件
*******************************************************************************/

#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include "DataBase.h"

static QSqlDatabase m_currdb;

///////////////////////////////////////////////////////////////
//函 数 名 : add_Record
//函数功能 : 添加记录
//处理过程 : 无
//参数说明 :
//          const QString & tableName         要添加的表名
//          const QList<QVariant> & valuesList 添加的值
//返 回 值 :
//成功  返回 true
//失败  返回 false
///////////////////////////////////////////////////////////////
bool CDataBase::add_Record(const QString & tableName, const QList<QVariant> & valuesList)
{
    bool ret = false;
    int nCount = valuesList.count();
    QString str = "?";

    for(int nIndex = 1; nIndex < nCount; nIndex++)
    {
        str += QObject::tr(",?");
    }

    QSqlQuery query;
    query.prepare(QObject::tr("INSERT INTO %0 VALUES(%1)").arg(tableName).arg(str));

    for(int nIndex = 0; nIndex < nCount; nIndex++)
    {
        query.addBindValue(valuesList.at(nIndex));
    }

    if(query.exec())
    {
        ret = true;
    }
    else
    {
        qDebug() << "CDataBase::add_Record():" << query.lastError();
        ret = false;
    }

    tidy_Record();

    return ret;
}

bool CDataBase::del_Record(const QString  & tableName) //表格名
{
    QSqlQuery query;
    query.prepare(QObject::tr("DELETE FROM %0").arg(tableName));

    if(query.exec())
    {
        return true;
    }
    else
    {
        qDebug() <<"CDataBase::del_Record(1 arg):" << query.lastError();
        return false;
    }
}

bool CDataBase::del_Table(const QString  & tableName) //表格名
{
    QSqlQuery query;
    query.prepare(QObject::tr("DROP TABLE %0").arg(tableName));

    if(query.exec())
    {
        return true;
    }
    else
    {
        qDebug() <<"CDataBase::del_Table(1 arg):" << query.lastError();
        return false;
    }
}

///////////////////////////////////////////////////////////////
//	函 数 名 : del_Record
//	函数功能 : 删除记录
//	处理过程 : 无
//	参数说明 :
//                const QString  & tableName //表格名
//                const QString  & factor_FieldName //条件的字段名
//                const QVariant & factor_Value)   //条件的值
//	返 回 值 :
//      成功  返回 true
//      失败  返回 false
///////////////////////////////////////////////////////////////
bool CDataBase::del_Record(const QString  & tableName //表格名
                           , const QString  & factor_FieldName //条件的字段名
                           , const QVariant & factor_Value)   //条件的值
{
    if(!select_Record(tableName, factor_FieldName, factor_Value).next())
    {
        return false;
    }

    QSqlQuery query;
    query.prepare(QObject::tr("DELETE FROM %0 WHERE %1 = ?")
                  .arg(tableName).arg(factor_FieldName));
    query.addBindValue(factor_Value);

    if(query.exec())
    {
        return true;
    }
    else
    {
        qDebug() <<"CDataBase::del_Record(3 args):" << query.lastError();
        return false;
    }
}



///////////////////////////////////////////////////////////////
//	函 数 名 : del_Record
//	函数功能 : 删除记录
//	处理过程 : 无
//	参数说明 :
//            const QString  & tableName //表格名
//            const QString  & factor_FieldName1 //条件的字段名1
//            const QString  & factor_FieldName2 //条件的字段名2
//            const QVariant & factor_Value1     //条件的值1
//            const QVariant & factor_Value2 )   //条件的值2
//	返 回 值 :
//          成功  返回 true
//          失败  返回 false
///////////////////////////////////////////////////////////////
bool CDataBase::del_Record(const QString  & tableName //表格名
                           , const QString  & factor_FieldName1 //条件的字段名1
                           , const QString  & factor_FieldName2 //条件的字段名2
                           , const QVariant & factor_Value1     //条件的值1
                           , const QVariant & factor_Value2 )   //条件的值2
{

    if(!select_Record(tableName, factor_FieldName1, factor_FieldName2, factor_Value1, factor_Value2).next())
    {
        return false;
    }

    QSqlQuery query;
    query.prepare(QObject::tr("DELETE FROM %0 WHERE %1 = ? AND %2 = ?")
                  .arg(tableName).arg(factor_FieldName1).arg(factor_FieldName2));

    query.addBindValue(factor_Value1);
    query.addBindValue(factor_Value2);

    if(query.exec())
    {
        return true;
    }
    else
    {
        qDebug() <<"CDataBase::del_Record(5 args):" << query.lastError();
        return false;
    }
}

///////////////////////////////////////////////////////////////
//函 数 名 : updata_Record
//函数功能 : 更新数据
//处理过程 : 无
//参数说明 :
//     tableName           表格名
//     update_FieldName    更新的字段名
//     factor_FieldName    条件的字段名
//     update_Value        更新的值
//     factor_Value        条件的值
//返 回 值 :
//    成功  返回 true
//    失败  返回 false
///////////////////////////////////////////////////////////////
bool CDataBase::updata_Record(  const QString  & tableName
                                , const QString  & update_FieldName
                                , const QString  & factor_FieldName
                                , const QVariant & update_Value
                                , const QVariant & factor_Value )
{
    QSqlQuery query;
    query.prepare(QObject::tr("UPDATE %0 SET %1 = ? WHERE %2 = ?")
                  .arg(tableName).arg(update_FieldName).arg(factor_FieldName));
    query.addBindValue(update_Value);
    query.addBindValue(factor_Value);

    if(query.exec())
    {
        return true;
    }
    else
    {
        qDebug() << "CDataBase::updata_Record(5 args):" << query.lastError();
        return false;
    }
}

QSqlQuery CDataBase::select_Record(const QString &tableName, const QList<QString> &FieldName, const QList<QVariant> &Value)
{
    QString str;
    str.append(QObject::tr("SELECT * FROM %0 WHERE").arg(tableName));
    int listnum = FieldName.count();
    str.append(QObject::tr(" %0 = ?").arg(FieldName.at(0)));
    for(int i=1;i<listnum;i++)
    {
        str.append(QObject::tr(" AND %0 = ?").arg(FieldName.at(i)));
    }
    QSqlQuery query;
    query.prepare(str);
    listnum = Value.count();
    for(int i=0;i<listnum;i++)
        query.addBindValue(Value.at(i));

    if ( ! query.exec())
    {
        qDebug() << "CDataBase::select_Record(3 args):" << query.lastError();
    }

    return query;
}


///////////////////////////////////////////////////////////////
//	函 数 名 : select_Record
//	函数功能 : 查找记录
//	处理过程 : 无
//	参数说明 :
//           tableName            表格名
//           factor_FieldName1    条件的字段名1
//           factor_FieldName2    条件的字段名2
//           factor_Value1        条件的值1
//           factor_Value2        条件的值2
//	返 回 值 :
//          返回 结果集
///////////////////////////////////////////////////////////////
QSqlQuery CDataBase::select_Record(const QString  & tableName         //表格名
                                   , const QString  & factor_FieldName1 //条件的字段名1
                                   , const QString  & factor_FieldName2 //条件的字段名2
                                   , const QVariant & factor_Value1     //条件的值1
                                   , const QVariant & factor_Value2 )   //条件的值2
{
    QSqlQuery query;
    query.prepare(QObject::tr("SELECT * FROM %0 WHERE %1 = ? AND %2 = ?")
                  .arg(tableName).arg(factor_FieldName1).arg(factor_FieldName2));
    query.addBindValue(factor_Value1);
    query.addBindValue(factor_Value2);

    if ( ! query.exec())
    {
        qDebug() << "CDataBase::select_Record(5 args):" << query.lastError();
    }

    return query;
}

///////////////////////////////////////////////////////////////
//	函 数 名 : select_Record
//	函数功能 : 查找记录
//	处理过程 : 无
//	参数说明 :
//           tableName             表格名
//           selection_FieldName   查找的字段名
//           factor_FieldName      条件的字段名
//           factor_Value          条件的值
//	返 回 值 :
//          返回 结果集
///////////////////////////////////////////////////////////////
QSqlQuery CDataBase::select_Record(const QString  & tableName
                                   , const QString  & selection_FieldName
                                   , const QString  & factor_FieldName
                                   , const QVariant & factor_Value)
{
    QSqlQuery query;
    query.prepare(QObject::tr("SELECT %0 FROM %1 WHERE %2 = ?")
                  .arg(selection_FieldName)
                  .arg(tableName)
                  .arg(factor_FieldName));

    query.addBindValue(factor_Value);

    if ( ! query.exec())
    {
        qDebug() << "CDataBase::select_Record(4 args):" << query.lastError();
    }

    return query;
}



///////////////////////////////////////////////////////////////
//	函 数 名 : select_Record
//	函数功能 : 查找记录
//	处理过程 : 无
//	参数说明 :
//           tableName             表格名
//           factor_FieldName      条件的字段名
//           factor_Value          条件的值
//	返 回 值 :
//          返回 结果集
///////////////////////////////////////////////////////////////
QSqlQuery CDataBase::select_Record(const QString  & tableName
                                   , const QString  & factor_FieldName
                                   , const QVariant & factor_Value )
{
    QSqlQuery query;
    query.prepare(QObject::tr("SELECT * FROM %0 WHERE %1 = ?").arg(tableName).arg(factor_FieldName));
    query.addBindValue(factor_Value);

    if ( ! query.exec())
    {
        qDebug() << "CDataBase::select_Record(3 args):" << query.lastError();
    }

    return query;
}


///////////////////////////////////////////////////////////////
//	函 数 名 : select_Record
//	函数功能 : 查找记录
//	处理过程 : 无
//	参数说明 :
//           tableName             表格名
//           factor_FieldName      条件的字段名
//           factor_Value          条件的值
//	返 回 值 :
//          返回 结果集
///////////////////////////////////////////////////////////////
QSqlQuery CDataBase::select_Record(const QString  & tableName)
{
    QSqlQuery query;

    if ( ! query.exec(QObject::tr("SELECT * FROM %0").arg(tableName)))
    {
        qDebug() << "CDataBase::select_Record(1 arg):" << query.lastError();
    }

    return query;
}

///////////////////////////////////////////////////////////////
QSqlQuery CDataBase::select_Record(const QString  & tableName //表格名
                                   , const QString  & strFieldName //条件的字段名
                                   , int nMinVar    //条件1的值
                                   , int nMaxVar ) //条件2的值
{
    QString strSql = QObject::tr("SELECT * FROM %0 WHERE %1 BETWEEN %2 AND %3;")
            .arg(tableName).arg(strFieldName).arg(nMinVar).arg(nMaxVar);
    QSqlQuery query;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::select_Record(4 arg):" << query.lastError();
    }

    return query;
}

QSqlQuery CDataBase::select_Record(  const QString  & tableName //表格名
                                     , const QString  & strFieldName //条件的字段名1
                                     , const QString factor_Value    // 条件的值
                                     , const QString &output_criteria //输出条件 > < = !=...
                                     , bool up_down ) //输出排序 true降序
{
    QString strSql;
    QSqlQuery query;

    if(up_down)
    {
        strSql = QObject::tr("SELECT * FROM %0 WHERE %1 %2 %3 ORDER BY %4 DESC")
                   .arg(tableName).arg(strFieldName).arg(output_criteria)
                   .arg(factor_Value).arg(strFieldName);

        if ( ! query.exec(strSql))
        {
            qDebug() << "CDataBase::select_Record(up_down = true):" << query.lastError();
        }
    }
    else
    {
        strSql = QObject::tr("SELECT * FROM %0 WHERE %1 %2 %3 ORDER BY %4 ASC")
                   .arg(tableName).arg(strFieldName).arg(output_criteria)
                   .arg(factor_Value).arg(strFieldName);

        if ( ! query.exec(strSql))
        {
            qDebug() << "CDataBase::select_Record(up_down = false):" << query.lastError();
        }
    }

    return query;
}

QSqlQuery CDataBase::GetCureTrace(QString qStrName)
{
    QString strSql = QObject::tr("select TABLE_BODY.[SERNUM],TABLE_BODY.[TDATE],PATIENT_BODY.[ID],PATIENT_BODY.[NAME],PATIENT_BODY.[SEX],PATIENT_BODY.[AGE],TABLE_BODY.[UNITE],TABLE_BODY.[NAME1],TABLE_BODY.[REST1],TABLE_BODY.[UNIT1] "
                                 " from TABLE_BODY,PATIENT_BODY where TABLE_BODY.[SERNUM]=PATIENT_BODY.[SERNUM] and PATIENT_BODY.[NAME]='%1' limit 0,20;").arg(qStrName);
    QSqlQuery query;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::GetCureTrace():" << query.lastError();
    }

    return query;
}

QSqlQuery CDataBase::GetPatientList()
{
    QString strSql;
    QSqlQuery query;

    strSql = QObject::tr("SELECT distinct ID FROM PATIENT_BODY WHERE ID <>'' order by rowid desc limit 0,10;");
    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::select_Record(up_down = false):" << query.lastError();
    }
    query.exec(strSql);

    return query;
}

QSqlQuery CDataBase::GetPatientInfo(QString qStrSerNum)
{
    QString strSql;
    QSqlQuery query;

    strSql = QObject::tr("SELECT * FROM PATIENT_BODY WHERE SERNUM = %1;").arg(qStrSerNum);
    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::select_Record(up_down = false):" << query.lastError();
    }
    query.exec(strSql);

    return query;
}

QSqlQuery CDataBase::GetPatientInfoByID(QString qStrID)
{
    QString strSql;
    QSqlQuery query;

    strSql = QObject::tr("SELECT * FROM PATIENT_BODY WHERE ID = \"%1\";").arg(qStrID);
    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::select_Record(up_down = false):" << query.lastError();
    }
    query.exec(strSql);

    return query;
}

QSqlQuery CDataBase::GetSmpByID(QString qStrID)
{
    QString strSql = QObject::tr("select TABLE_BODY.[SERNUM],TABLE_BODY.[TDATE],TABLE_BODY.[UNITE],TABLE_BODY.[NAME1],TABLE_BODY.[UNIT1],TABLE_BODY.[REST1],PATIENT_BODY.[ID] "
                                 " from TABLE_BODY,PATIENT_BODY where TABLE_BODY.[SERNUM]=PATIENT_BODY.[SERNUM] and PATIENT_BODY.[ID]='%1' order by TABLE_BODY.[SERNUM] desc;").arg(qStrID);
    QSqlQuery query;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::GetSmpByID():" << query.lastError();
    }

    return query;
}

QSqlQuery CDataBase::GetSmpByName(QString qStrName)
{
    QString strSql = QObject::tr("select TABLE_BODY.[SERNUM],TABLE_BODY.[TDATE],TABLE_BODY.[UNITE],TABLE_BODY.[NAME1],TABLE_BODY.[UNIT1],TABLE_BODY.[REST1],PATIENT_BODY.[ID],PATIENT_BODY.[NAME] "
                                 " from TABLE_BODY,PATIENT_BODY where TABLE_BODY.[SERNUM]=PATIENT_BODY.[SERNUM] and PATIENT_BODY.[NAME]='%1' order by TABLE_BODY.[SERNUM] desc;").arg(qStrName);
    QSqlQuery query;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::GetSmpByName():" << query.lastError();
    }

    return query;
}

QSqlQuery CDataBase::GetPatientID(QString qStrKey)
{
    QString strSql = QObject::tr("select ID from PATIENT_BODY where ID like '%1%';").arg(qStrKey);
    QSqlQuery query;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::GetPatientID:" << query.lastError();
    }

    return query;
}
int CDataBase::GetIdFromPatient(QString qStrKey)
{
    QString strSql = QObject::tr("select max(ID) from PATIENT_BODY where ID like '%1%';").arg(qStrKey);
    QSqlQuery query;

    if (query.exec(strSql) && query.next())
        return query.value(0).toInt();
    else
        return 0;
}

QSqlQuery CDataBase::GetPatientFromTb(QString qStrKey)
{
    QString strSql = QObject::tr("select NOTES from TABLE_BODY where NOTES like '%1%';").arg(qStrKey);
    QSqlQuery query;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::GetPatientFromTb:" << query.lastError();
    }

    return query;
}
int CDataBase::GetIdFromTb(QString qStrKey)
{
    QString strSql = QObject::tr("select max(NOTES) from TABLE_BODY where NOTES like '%1%';").arg(qStrKey);
    QSqlQuery query;

    if (query.exec(strSql) && query.next())
        return query.value(0).toInt();
    else
        return 0;
}

bool CDataBase::IsPatientExist(QString qStrSerNum)
{
    QString strSql = QObject::tr("SELECT * from PATIENT_BODY WHERE SERNUM = %1;").arg(qStrSerNum);
    QSqlQuery query;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::IsPatientExist():" << query.lastError();
    }

    while(query.next())
    {
        return true;
    }

    return false;
}

bool CDataBase::UpdataPatientInfo(QList<QVariant> patientInfoList)
{
    bool bRet = false;

    QSqlQuery oSqlQuery;
    oSqlQuery.prepare("UPDATE PATIENT_BODY SET ID=:ID,NAME=:NAME,SEX=:SEX,AGE=:AGE,SAMPLENUM=:SAMPLENUM,"
                      "SAMPLETYPE=:SAMPLETYPE WHERE SERNUM=:SERNUM");
    oSqlQuery.bindValue(":SERNUM",patientInfoList.at(0));
    oSqlQuery.bindValue(":ID",patientInfoList.at(1));
    oSqlQuery.bindValue(":NAME",patientInfoList.at(2));
    oSqlQuery.bindValue(":SEX",patientInfoList.at(3));
    oSqlQuery.bindValue(":AGE",patientInfoList.at(4));
    oSqlQuery.bindValue(":SAMPLENUM",patientInfoList.at(5));
    oSqlQuery.bindValue(":SAMPLETYPE",patientInfoList.at(6));
    oSqlQuery.exec();
    bRet = oSqlQuery.isActive();
    return bRet;
}



///////////////////////////////////////////////////////////////
//函 数 名 : record_IsExist
//函数功能 : 判断某个数据是否存在
//处理过程 : 无
//参数说明 :
//     factor_FieldName    条件的字段名
//     factor_Value        条件的值
//返 回 值 :
//    存在  返回 true
//    否则  返回 false
///////////////////////////////////////////////////////////////
bool CDataBase::record_IsExist(const QString &tableName,const QString  &factor_FieldName, const QVariant &factor_Value)
{
    QSqlQuery query = select_Record(tableName, factor_FieldName, factor_Value);

    if(query.next())
    {
        return true;
    }
    return false;
}

bool CDataBase::record_IsExist(const QString &tableName,const QList<QString> &FieldName, const QList<QVariant> &Value)
{
    QSqlQuery query = select_Record(tableName, FieldName, Value);

    if(query.next())
    {
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////
//函 数 名 : record_IsExist
//函数功能 : 判断某个数据是否存在
//处理过程 : 无
//参数说明 :
//     factor_FieldName1    条件的字段名1
//     factor_FieldName2    条件的字段名2
//     factor_Value1        条件的值1
//     factor_Value2        条件的值2
//返 回 值 :
//    存在  返回 true
//    否则  返回 false
///////////////////////////////////////////////////////////////
bool CDataBase::record_IsExist(const QString &tableName,const QString  & factor_FieldName1, const QString  & factor_FieldName2
                               , const QVariant & factor_Value1, const QVariant & factor_Value2 )
{
    QSqlQuery query = select_Record(tableName, factor_FieldName1, factor_FieldName2, factor_Value1, factor_Value2);

    if(query.next())
    {
        return true;
    }
    return false;
}

int CDataBase::get_MinAdss(const QString &tableName)
{
    int num = 0;
    QString strSql = QObject::tr("SELECT MIN(SERNUM) FROM %0 ").arg(tableName);
    QSqlQuery query;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::get_MinAdss():" << query.lastError();
    }

#if 1
    if(query.next())
    {
        num = query.value(0).toInt();
        if(num < 0) num = 0;
    }
    else num = 0;
#endif
    return num;
}

int CDataBase::get_MaxAdss(const QString &tableName)
{
    int num = 0;
    QString strSql = QObject::tr("SELECT MAX(SERNUM) FROM %0 ").arg(tableName);
    QSqlQuery query;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::get_MaxAdss():" << query.lastError();
    }

#if 1
    if(query.next())
    {
        num = query.value(0).toInt();

        if(num < 0)
            num = 0;
    }
    else
        num = 0;
#endif
    return num;
}


int CDataBase::get_InsertAdss(const QString &tableName)
{
    return get_MaxAdss(tableName)+1;
}

int CDataBase::get_RecordNum(const QString &tableName)
{
    int num = 0;
    QString strSql = QObject::tr("SELECT COUNT(*) FROM %0").arg(tableName);
    QSqlQuery query ;

    if ( ! query.exec(strSql))
    {
        qDebug() << "CDataBase::get_RecordNum():" << query.lastError();
    }

    if(query.next())
    {
        num = query.value(0).toInt();
    }

    return num;
}

int CDataBase::get_RecordMax(const QString &tableName)
{
    if(tableName == TABLE_BODY_NAME)
    {
        return 20000;
    }
    else if(tableName == BATCH_BODY_NAME)
    {
        return 10000;
    }
    else if(tableName == FLUDT_BODY_NAME)
    {
        return 2000;
    }
}

void CDataBase::tidy_Record()
{
#if 0
    int num = CDataBase::get_RecordNum(TABLE_BODY_NAME) - CDataBase::get_RecordMax(TABLE_BODY_NAME);
    if(num > 0)
    {
        int max = get_MaxAdss(TABLE_BODY_NAME);
        QSqlQuery query = CDataBase::select_Record(TABLE_BODY_NAME,"SERNUM",QString::number(max),"<=",false);
        for(int i=0;i<num;i++)
        {
            if(query.next())
            {
                int ser = query.value(0).toInt();
                CDataBase::del_Record(TABLE_BODY_NAME, "SERNUM", ser);
                CDataBase::del_Record(FLUDT_BODY_NAME, "SERNUM", ser);
            }
        }
    }
    num = CDataBase::get_RecordNum(FLUDT_BODY_NAME) > CDataBase::get_RecordMax(FLUDT_BODY_NAME);
    if(num > 0)
    {
        int max = get_MaxAdss(FLUDT_BODY_NAME);
        QSqlQuery query = CDataBase::select_Record(FLUDT_BODY_NAME,"SERNUM",QString::number(max),"<=",false);
        for(int i=0;i<num;i++)
        {
            if(query.next())
            {
                int ser = query.value(0).toInt();
                CDataBase::del_Record(FLUDT_BODY_NAME, "SERNUM", ser);
            }
        }
    }
    num = CDataBase::get_RecordNum(BATCH_BODY_NAME) > CDataBase::get_RecordMax(BATCH_BODY_NAME);
    if(num > 0)
    {
        int max = get_MaxAdss(BATCH_BODY_NAME);
        QSqlQuery query = CDataBase::select_Record(BATCH_BODY_NAME,"SERNUM",QString::number(max),"<=",false);
        for(int i=0;i<num;i++)
        {
            if(query.next())
            {
                int ser = query.value(0).toInt();
                CDataBase::del_Record(BATCH_BODY_NAME, "SERNUM", ser);
            }

        }
    }
#else
    int num = CDataBase::get_RecordNum(TABLE_BODY_NAME) - CDataBase::get_RecordMax(TABLE_BODY_NAME);
    while(num-- > 0)
    {
        int min = get_MinAdss(TABLE_BODY_NAME);
        CDataBase::del_Record(TABLE_BODY_NAME, "SERNUM", min);
        CDataBase::del_Record(FLUDT_BODY_NAME, "SERNUM", min);
    }
    num = CDataBase::get_RecordNum(FLUDT_BODY_NAME) > CDataBase::get_RecordMax(FLUDT_BODY_NAME);
    while(num-- > 0)
    {
        int min = get_MinAdss(FLUDT_BODY_NAME);
        CDataBase::del_Record(FLUDT_BODY_NAME, "SERNUM", min);
    }
    num = CDataBase::get_RecordNum(BATCH_BODY_NAME) > CDataBase::get_RecordMax(BATCH_BODY_NAME);
    while(num-- > 0)
    {
        int min = get_MinAdss(BATCH_BODY_NAME);
        CDataBase::del_Record(BATCH_BODY_NAME, "SERNUM", min);
    }
#endif

}
///////////////////////////////////////////////////////////////
//	函 数 名 : init
//	函数功能 : 初始化数据库
//	处理过程 : 先判断数据库文件是否存在。 打开数据库，若失败则返回false
//                根据之前判断数据库文件是否存在，若不存在则初始化数据库里的表格等
//	参数说明 : 无
//	返 回 值 :
//          成功  返回 true
//          失败  返回 false
///////////////////////////////////////////////////////////////
bool CDataBase::Init(QString filedir)
{
    QDir dir;
//    QFileInfo fi = QFileInfo(filedir);
//    QString filepath = fi.absolutePath();

//    dir.setPath(filepath);
//    if(!dir.exists())
//    {
//        dir.mkpath(filepath);
//    }

//    QFile dataFile;
//    dataFile.setFileName(filedir);
//    if( ! dataFile.open(QIODevice::ReadWrite))
//        qDebug() << "file Exists fail";

//    dataFile.close();

    QFile file(filedir);
    if (file.exists())
    {
        qDebug() << filedir << "already exists";
        Open(filedir);
        return true;
    }
    else
    {
        QFileInfo fi = QFileInfo(filedir);
        QString filepath = fi.absolutePath();
        qDebug() << "make dir" << filepath;
        dir.mkpath(filepath);
    }

    Open(filedir);
    QString strSql;
    QSqlQuery query;

    //--------------------------创建表----------------------------
    /*保存结果记录*/
    strSql = QObject::tr("CREATE TABLE [%0] ("
                         "[SERNUM] INTEGER INTEGER NOT NULL PRIMARY KEY,"
                         "[BATCH] VARCHAR(12),"
                         "[TDATE] TIMESTAMP(14),"
                         "[UNITE] INTEGER,"
                         "[NAME1] VARCHAR(16),[UNIT1] VARCHAR(16),[REST1] VARCHAR(16),"
                         "[NAME2] VARCHAR(16),[UNIT2] VARCHAR(16),[REST2] VARCHAR(16),"
                         "[NAME3] VARCHAR(16),[UNIT3] VARCHAR(16),[REST3] VARCHAR(16),"
                         "[NAME4] VARCHAR(16),[UNIT4] VARCHAR(16),[REST4] VARCHAR(16),"
                         "[NAME5] VARCHAR(16),[UNIT5] VARCHAR(16),[REST5] VARCHAR(16),"
                         "[NAME6] VARCHAR(16),[UNIT6] VARCHAR(16),[REST6] VARCHAR(16),"
                         "[NOTES] VARCHAR(64),[UPDATED] TIMESTAMP(14)"
                         ");").arg(TABLE_BODY_NAME);
    if ( ! query.exec(strSql))
        qDebug() << "CDataBase::Init(), " << TABLE_BODY_NAME << ":" << query.lastError();

    /*保存条码记录*/
    strSql = QObject::tr("CREATE TABLE [%0] ("
                         "[SERNUM] INTEGER NOT NULL PRIMARY KEY,"
                         "[BATCH] VARCHAR(12),"
                         "[CODE] VARCHAR(1024)"
                         ");").arg(BATCH_BODY_NAME);
    if ( ! query.exec(strSql))
        qDebug() << "CDataBase::Init(), " << BATCH_BODY_NAME << ":" << query.lastError();

    /*保存荧光数据记录*/
    strSql = QObject::tr("CREATE TABLE [%0] ("
                         "[SERNUM] INTEGER NOT NULL PRIMARY KEY,"
                         "[FLUDT] VARCHAR(2410)"
                         ");").arg(FLUDT_BODY_NAME);
    if ( ! query.exec(strSql))
        qDebug() << "CDataBase::Init(), " << FLUDT_BODY_NAME << ":" << query.lastError();

    /*保存患者信息*/
    strSql = QObject::tr("CREATE TABLE [%0] ("
                         "[SERNUM] INTEGER,"
                         "[ID] VARCHAR(20),"
                         "[NAME] VARCHAR(20),"
                         "[SEX] VARCHAR(20),"
                         "[AGE] VARCHAR(20),"
                         "[SAMPLENUM] VARCHAR(20),"
                         "[SAMPLETYPE] VARCHAR(20)"
                         ");").arg(PATIENT_BODY_NAME);
    if ( ! query.exec(strSql))
        qDebug() << "CDataBase::Init(), " << FLUDT_BODY_NAME << ":" << query.lastError();

    return true;
}

void CDataBase::Open(QString filedir)
{
    m_currdb.close();

    //m_currdb = QSqlDatabase::database(filedir, false);

    /* 存在连接直接退出*/
    if(m_currdb.isValid())
    {
        //qDebug() << "currdb isValid true";
        //   return ;
    }

    /* 使用的是QSQLITE*/
#if 1
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
#endif
    m_currdb = QSqlDatabase::addDatabase("QSQLITE");

    //QSqlQuery query = QSqlQuery(m_currdb);
    QFileInfo fi = QFileInfo(filedir);
    QString filename = fi.fileName();
    /* 数据库名与路径, 此时是放在同目录下*/
    m_currdb.setDatabaseName(filedir);

    if(!m_currdb.open())
    {
        qDebug() << "CDataBase::Open(): open " << filedir << " fail !";
        return;
    }
}

void CDataBase::Close()
{
    m_currdb.close();
}










