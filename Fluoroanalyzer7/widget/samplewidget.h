#ifndef SAMPLEWIDGET_H
#define SAMPLEWIDGET_H

#include "basefunwidget.h"
#include "common/commoninterface.h"
#include "base/MyButton.h"
#include "promptbox.h"
#include "common/TestData.h"
#include "widget/SearchWidget.h"
#include "widget/DeleteChoose.h"
#include "widget/MessageBox.h"

class CSampleWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CSampleWidget(QWidget *parent = 0);
    void showEvent(QShowEvent *);
    void hideEvent(QShowEvent *);
    void updatarecord(QString table,int first,bool up_down);
    void ClearTbContent();
    void ShowTestData(int iPos);
signals:
    void BtDetailClicked(int iSer);
    void sinSetYgCurveData(QByteArray data, ENUM_WIDGET returnWg);

public slots:
    void slotclicksearch();
    void slotclickprintf();
    void slotclickdele();
    void slotclickback();
    void slotClickCurve();

    void slotserchange();
    void slotclickup();
    void slotclickdw();
    void slotclickdel1();
    void slotclickdeln();

    void slotinputclick(QString text);

    void slotcurlineedit(XLineEdit *id);
    void slotsearchdel();
    void slotsearchclr();
    void slotsearchall();

    void slotpromptok(int ser);
    void slotpromptcs(int ser);

    void slotChangeID(bool isBatch);
    void SLOT_TbButtonClick(int iRow);
    void RecordQuery(QString qStrKey,int iIndex);
    void RecordCancel();
    void DeleteRecord(int iIndex);
    void EnableWidget();

private:
    QString m_curtable;
    QSqlQuery m_tempquery;

    XLabel *m_spacename;
    XLabel *m_spacevaule;

    QTableWidget *m_samplerecord;
    QListWidget *m_samplelist;

    XButton *m_pageup;
    XButton *m_pagedown;
    XButton *m_curve;
    XButton *m_search;
    XButton *m_printf;
    XButton *m_dele;
    XButton *m_back;

    char m_btUGC[2][27];
    QWidget *m_searchgroup;
    XLabel *m_searchlabel[10];
    XLineEdit *m_searchser;
    XLineEdit *m_searchbatch;
    XLineEdit *m_searchID;
    XLineEdit *m_searchpro;
    XLineEdit *m_searchyear;
    XLineEdit *m_searchmon;
    XLineEdit *m_searchday;
    XLabel *m_searchnum;

    XButton *m_input[27];
    XButton *m_curdel;

    XButton *m_searchclr;
    XButton *m_searchall;

    XLineEdit *m_curlineedit;


    XButton *m_up;
    XButton *m_dw;

    XPromptBox *m_promptbox;

    int m_iPos;
    int m_nCurrSerNO;
    bool m_bSearch;
    bool bRefreshData;
    CSearchWidget *m_pSearchWidget;
    CDeleteChoose *m_pDeleteChoose;
    QVector<CTestData *> vecTestData;
};

#endif // SAMPLEWIDGET_H
