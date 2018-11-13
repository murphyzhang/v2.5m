/******************************************************************************
*     数据模块的定义文件
*******************************************************************************/

#ifndef DATABASE_H
#define DATABASE_H

#include <QtGui>
#include <QtCore>
#include <QtSql>
#include <QSqlQuery>


#define DATABASE_NAME   ("/BioTime/biotime.db")     // 数据库


#define TABLE_BODY_NAME "TABLE_BODY"        //存储结果
#define BATCH_BODY_NAME "BATCH_BODY"        //存储批号
#define FLUDT_BODY_NAME "FLUDT_BODY"        //存储荧光值
#define TMPTB_BODY_NAME "TMPTB_BODY"        //临时表,用于样本记录中查找数据
#define PATIENT_BODY_NAME "PATIENT_BODY"    //患者信息表

class CDataBase
{
public:

    //---------------------------------- 通用接口 ------------------------------------


    // 不填字段给 NULL
    static bool add_Record(  const QString & tableName
                             , const QList<QVariant> & valuesList);

    static bool del_Record(const QString  & tableName); //表格名

    static bool del_Table(const QString  & tableName); //表格名

    static bool del_Record(const QString  & tableName //表格名
                           , const QString  & factor_FieldName //条件的字段名
                           , const QVariant & factor_Value);   //条件的值

    static bool del_Record(  const QString  & tableName //表格名
                             , const QString  & factor_FieldName1 //条件的字段名1
                             , const QString  & factor_FieldName2 //条件的字段名2
                             , const QVariant & factor_Value1    //条件1的值
                             , const QVariant & factor_Value2 ); //条件2的值


    static bool updata_Record(  const QString  & tableName //表格名
                                , const QString  & update_FieldName //更新的字段名
                                , const QString  & factor_FieldName //条件的字段名
                                , const QVariant & update_value    //更新的值
                                , const QVariant & factor_Value);   //条件的值

    static QSqlQuery select_Record(  const QString  & tableName //表格名
                                     ,const QList<QString> &FieldName //条件字段名
                                     ,const QList<QVariant> &Value);//条件值


    static QSqlQuery select_Record(  const QString  & tableName //表格名
                                     , const QString  & factor_FieldName1 //条件的字段名1
                                     , const QString  & factor_FieldName2 //条件的字段名2
                                     , const QVariant & factor_Value1    //条件1的值
                                     , const QVariant & factor_Value2 ); //条件2的值


    static QSqlQuery select_Record(  const QString  & tableName //表格名
                                     , const QString  & strFieldName //条件的字段名1
                                     , int nMinVar    //条件1的值
                                     , int nMaxVar ); //条件2的值

    static QSqlQuery select_Record(  const QString  & tableName //表格名
                                     , const QString  & strFieldName //条件的字段名1
                                     , const QString factor_Value    // 条件的值
                                     , const QString &output_criteria //输出条件 > < = !=...
                                     , bool up_down ); //输出排序 true降序


    static QSqlQuery select_Record(  const QString  & tableName //表格名
                                     , const QString  & factor_FieldName //条件的字段名
                                     , const QVariant & factor_Value ); //条件的值

    static QSqlQuery select_Record(const QString  & tableName);

    static QSqlQuery select_Record(  const QString  & tableName             //表格名
                                     , const QString  & selection_FieldName //查找的字段名
                                     , const QString  & factor_FieldName    //条件的字段名
                                     , const QVariant & factor_Value);      //条件的值

    static QSqlQuery GetCureTrace(QString qStrName);
    static QSqlQuery GetPatientList();
    static QSqlQuery GetPatientInfo(QString qStrSerNum);
    static QSqlQuery GetPatientInfoByID(QString qStrID);
    static QSqlQuery GetSmpByID(QString qStrID);
    static QSqlQuery GetSmpByName(QString qStrName);

    static QSqlQuery GetPatientID(QString qStrKey);
    static QSqlQuery GetPatientFromTb(QString qStrKey);

    static int GetIdFromPatient(QString qStrKey);
    static int GetIdFromTb(QString qStrKey);

    static bool IsPatientExist(QString qStrSerNum);
    static bool UpdataPatientInfo(QList<QVariant> patientInfoList);

    static bool record_IsExist(const QString &tableName,const QString  &factor_FieldName,const QVariant &factor_Value);

    static bool record_IsExist(const QString &tableName,const QList<QString> &FieldName ,const QList<QVariant> &Value);

    static bool record_IsExist(const QString &tableName,const QString  &factor_FieldName1 , const QString  &factor_FieldName2 //条件的字段名
                                 ,const QVariant &factor_Value1 , const QVariant &factor_Value2 );

    static int get_MinAdss(const QString &tableName);

    static int get_MaxAdss(const QString &tableName);

    static int get_InsertAdss(const QString &tableName);

    static int get_RecordNum(const QString &tableName);

    static int get_RecordMax(const QString &tableName);

    static void tidy_Record();
    //---------------------------------- 初始化 ------------------------------------
    static bool Init(QString filedir);
    static void Open(QString filedir);
    static void Close();
private:


};



#endif // DATABASE_H

