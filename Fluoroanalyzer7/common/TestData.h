#ifndef TESTDATA_H
#define TESTDATA_H
#include <QString>

class CTestData
{
public:
    CTestData();
    CTestData(CTestData &oTestData);

    //item
    void SetSerNum(int iSerNum)
    {
        m_iSerNum = iSerNum;
    }
    int GetSerNum()
    {
        return m_iSerNum;
    }

    void SetBatch(QString sBatch)
    {
        m_sBatch = sBatch;
    }
    QString GetBatch()
    {
        return m_sBatch;
    }

    void SetDate(QString sDate)
    {
        m_sDate = sDate;
    }
    QString GetDate()
    {
        return m_sDate;
    }

    void SetUnite(int iUnite)
    {
        m_iUnite = iUnite;
    }
    int GetUnite()
    {
        return m_iUnite;
    }

    void SetItem(QString sItem)
    {
        m_sItem = sItem;
    }
    QString GetItem()
    {
        return m_sItem;
    }

    void SetResult(QString sResult)
    {
        m_sResult = sResult;
    }
    QString GetResult()
    {
        return m_sResult;
    }

    void SetUnit(QString sUnit)
    {
        m_sUnit = sUnit;
    }
    QString GetUnit()
    {
        return m_sUnit;
    }

    //patient
    void SetID(QString sID)
    {
        m_sID = sID;
    }
    QString GetID()
    {
        return m_sID;
    }

    void SetName(QString sName)
    {
        m_sName = sName;
    }
    QString GetName()
    {
        return m_sName;
    }

    void SetSex(QString sSex)
    {
        m_sSex = sSex;
    }
    QString GetSex()
    {
        return m_sSex;
    }

    void SetAge(QString sAge)
    {
        m_sAge = sAge;
    }
    QString GetAge()
    {
        return m_sAge;
    }

    void SetSampleNum(QString sSampleNum)
    {
        m_sSampleNum = sSampleNum;
    }
    QString GetSampleNum()
    {
        return m_sSampleNum;
    }

    void SetSampleType(QString sSampleType)
    {
        m_sSampleType = sSampleType;
    }
    QString GetSampleType()
    {
        return m_sSampleType;
    }

private:
    //item
    int m_iSerNum;
    QString m_sBatch;
    QString m_sDate;
    int m_iUnite;
    QString m_sItem;
    QString m_sResult;
    QString m_sUnit;

    //patient
    QString m_sID;
    QString m_sName;
    QString m_sSex;
    QString m_sAge;
    QString m_sSampleNum;
    QString m_sSampleType;
};

#endif // TESTDATA_H
