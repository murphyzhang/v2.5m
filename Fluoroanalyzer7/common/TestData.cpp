#include "TestData.h"

CTestData::CTestData()
{
}

CTestData::CTestData(CTestData &oTestData)
{
    SetSerNum(oTestData.GetSerNum());
    SetBatch(oTestData.GetBatch());
    SetDate(oTestData.GetDate());
    SetUnite(oTestData.GetUnite());
    SetItem(oTestData.GetItem());
    SetResult(oTestData.GetResult());
    SetUnit(oTestData.GetUnit());

    SetID(oTestData.GetID());
    SetName(oTestData.GetName());
    SetSex(oTestData.GetSex());
    SetAge(oTestData.GetAge());
    SetSampleNum(oTestData.GetSampleNum());
    SetSampleType(oTestData.GetSampleType());
}
