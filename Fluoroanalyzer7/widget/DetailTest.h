#ifndef DETAILTEST_H
#define DETAILTEST_H

#include <QtGui>
#include <QtCore>
#include <QWSServer>
#include "common/commoninterface.h"
#include "basefunwidget.h"
#include "base/xbutton.h"
#include "base/xlabel.h"
#include "common/configurate.h"

class CDetailTest : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CDetailTest(QWidget *parent = 0);
    ~CDetailTest();

    void InitUI();
    void showEvent(QShowEvent *);
    void Clear();
    void SetValue(int iSerNum);
    void ShowHistory(QString qStrSerNum);

private slots:
    void BtCureTrace();
    void BtReturnClicked();

signals:
    void SigCureTrace(int iSer,QString qStrName,QString qStrSex,QString qStrAge);

private:
    XLabel *m_pLbTitle;
    XLabel *m_pLbID;
    XLabel *m_pLbID_2;
    XLabel *m_pLbName;
    XLabel *m_pLbName_2;
    XLabel *m_pLbSex;
    XLabel *m_pLbSex_2;
    XLabel *m_pLbAge;
    XLabel *m_pLbAge_2;
    XLabel *m_pLbSampleNo;
    XLabel *m_pLbSampleNo_2;
    XLabel *m_pLbSampleType;
    XLabel *m_pLbSampleType_2;
    XButton *m_pBtCureTrace;
    XButton *m_pBtReturn;

    int m_iSerNum;
    QTableWidget *m_pTestRecord;
};

#endif // DETAILTEST_H
