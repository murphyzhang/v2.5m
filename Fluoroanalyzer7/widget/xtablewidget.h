#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>

#include <QTableWidget>
#include <QHeaderView>
#include <QScrollArea>
#include <QLayout>
#include "common/global_define.h"
#include "common/testcard.h"
#include "base/xbutton.h"
#include "base/xlabel.h"
#include "base/xlineedit.h"
#include "base/MyButton.h"
#include "basefunwidget.h"
#include "common/DataBase.h"
#include "common/commoninterface.h"
#include "widget/PatientInfo.h"

#define ROW_COUNT_MAX 200
class XTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XTableWidget(QWidget *parent = 0);
    ~XTableWidget();
    void initcardwidget();
    void initprowidget();
    void initeditwidget();
    void initnoteswidget();

    void xinsert_row(XTestCard *card);
    void xsetcur_row(XTestCard *card);
    XTestCard* xgetcur_row();
    void xupdate_tab();
    void xclear();
    void setproline(int line);
    void showeditbatch();
    void showproinfo();
    void shownotesedit();

//    void delayEanbleBtnEdit();

    void refresh();

signals:
    void sigremoverow(XTestCard *card);
    void sigclickrow(XTestCard *card);
    void sigclickback();
    void sigclickclear();
    void sigsetbatch(XTestCard *card,QString batch);
    //void sigsetnote();
    void sinSetYgCurveData(QByteArray data, ENUM_WIDGET returnWg);
    void sigUpdateID(QString qStrID);

public slots:
    void slotcardclick();
    void slotbatchclick(int row, int column);
    void sloteditclick();
    void slotClickCurve();
    void slotprintclick();
    void slotclearclick();
    void slotbackclick();

    void slotokclick();

    void slotinputclick(QString text);
    void slotnotesdel();
    void slotnotesok();

    void slotScanInput(QString strId);
    void slotAddPatientInfo(QString qStrID, XTestCard * card);

private:
    void showEvent(QShowEvent *);
    bool setIdToCardList(QString qStrID,
                         QString qStrName,
                         QString qStrSex,
                         QString qStrAge,
                         QString qStrSmpNum,
                         QString qStrSmpType);
    void updateID();

    /* 将吴文龙交接时的代码修改至该位置 */
    void initID();

private slots:
//    void timeoutBtnEdit();

private:
    //QMap<int, XTestCard *> testcardmap;
    QList<XTestCard *> m_cardlist;

    bool m_bShowBatch; //true: batch, false: ID
    QString m_strId;

    QTableWidget *m_cardinfo;   //已进卡信息表
    QTableWidget *m_proinfo;    //测试结果信息表
    QTableWidget *m_batch;      //批号编辑面板
    XButton *m_ok;
    XLabel *m_prompt;
    XLabel *m_btlb;
    XLineEdit *m_inbatch;

    QWidget *m_editgroup;

    XButton *m_curve;
    XButton *m_print;
    XButton *m_edit;
    XButton *m_clear;
    XButton *m_back;

    char m_btUGC[2][27];
    QWidget *m_notesgroup;
    XLabel *m_notesprompt;
    QTextEdit *m_notesUGC;
    XButton *m_input[27];
    XButton *m_notesdel;
    XButton *m_notesok;

    CPatientInfo *m_pPatientInfo;
    QWidget *m_plusfun;
    QScrollArea *m_scrollarea;

//    QTimer * timerBtnEdit;

    int m_ID;

    QFont font;
};

#endif // TABLEWIDGET_H
