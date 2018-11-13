#ifndef PATIENTINFO_H
#define PATIENTINFO_H

#include <QtGui>
#include <QtCore>
#include <QWSServer>
#include <QIntValidator>
#include <QListWidget>
#include <QtSql>
#include <QSqlQuery>
#include "basefunwidget.h"
#include "base/MyLineEdit.h"
#include "base/xbutton.h"
#include "base/xlabel.h"
#include "widget/syszuxim.h"
#include "widget/syszuxpinyin.h"
#include "common/DataBase.h"
#include "common/commoninterface.h"
#include "common/configurate.h"
#include "common/testcard.h"

#define ID 1
#define NAME 2
#define AGE 3
#define SAMPLENUM 4
#define SAMPLETYPE 5

class CPatientInfo : public CBaseFunWidget
{
    Q_OBJECT

public:
    explicit CPatientInfo(QWidget *parent = 0);
    void closeEvent(QCloseEvent *);
    ~CPatientInfo();

    void InitUI();
    void ClearUI();
    void SetAutoID(QString qStrID);
    void SetSerNum(int iSerNum);
    void ShowData(QString qStrID,QString qStrName,QString qStrSex,QString qStrAge,QString qStrSmpNum,QString qStrSmpType);

    void setCurrEditCtrl(XTestCard * card, QTableWidgetItem * nameCell);

signals:
    void EditFinish(QString qStrID, XTestCard * card);

public slots:
    void CallKeyBoard(QString qStrText);
    void BtConfirmClicked();
    void BtReturnClicked();
    void ConfirmString(QString qStrText);   //接收键盘发过来的数据
    void IndexChange(int iIndex);
    void ComboboxEdit(QString qStrText);
    void BtIdListClicked();
    void IdItemListClicked(QListWidgetItem *item);
    void IdEditFinished();

private slots:
    void timeoutBtnConfirm();

private:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

private:
    int m_iSerNum;
    int m_iFlag;
    bool m_bIdListHide;
    bool m_bSampleTypeListHide;
    QList<QVariant> patientInfoList;
    QIntValidator *m_pValidator;
    QListWidget *m_pIdListWidget;
    QListWidget *m_pSampleTypeListWidget;
    XLabel *m_pLbTitle;
    XLabel *m_pLbID;
    CMyLineEdit *m_pLeID;
    QPushButton *m_pBtID;
    XLabel *m_pLbName;
    CMyLineEdit *m_pLeName;
    XLabel *m_pLbSex;
    QComboBox *m_pCbbSex;
    XLabel *m_pLbAge;
    CMyLineEdit *m_pLeAge;
    XLabel *m_pLbSampleNo;
    CMyLineEdit *m_pLeSampleNo;
    XLabel *m_pLbSampleType;
    QComboBox *m_pCbbSampleType;
    XButton *m_pXbtConfirm;
    XButton *m_pXbtReturn;
    SyszuxPinyin *m_pSyszux;

    XTestCard * m_card;
    QTableWidgetItem * m_nameCell;

    QTimer * timerBtnConfirm;
};

#endif // PATIENTINFO_H
