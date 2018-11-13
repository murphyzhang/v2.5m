#ifndef CURETRACE_H
#define CURETRACE_H

#include <QtGui>
#include <QtCore>
#include <QWSServer>
#include "common/commoninterface.h"
#include "basefunwidget.h"
#include "base/xbutton.h"
#include "base/xlabel.h"
#include "common/TestData.h"
#include "common/configurate.h"

class CCureTrace : public CBaseFunWidget
{
    Q_OBJECT

public:
    explicit CCureTrace(QWidget *parent = 0);
    ~CCureTrace();
    void InitUI();
    void SetPatientInfo(int iSer,QString qStrName,QString qStrSex,QString qStrAge);
    void showEvent(QShowEvent *);
    void GetDataList();
    void ShowTestData(int iPos);
    void ClearTbContent();

private slots:
    void BtUpClicked();
    void BtDownClicked();
    void BtCureTrace();
    void BtReturnClicked();

signals:
    void sigShowPlotLines(QVector<CTestData *> vecData);

private:
    int m_iSer;
    int m_iPos;
    QString m_sName;
    QString m_sSex;
    QString m_sAge;
    XLabel *m_pLbTitle;
    XButton *m_pBtUp;
    XButton *m_pBtDown;
    XButton *m_pBtReturn;
    XButton *m_pBtCureTrace;
    QTableWidget *m_pTestRecord;
    QVector<CTestData *> vecTestData;
};

#endif // CURETRACE_H
