#ifndef DETECTWIDGET_H
#define DETECTWIDGET_H

#include <QWidget>
#include "basefunwidget.h"
#include "xtablewidget.h"
#include "xdrawcasewidget.h"
#include "xdrawinfowidget.h"
#include "common/commoninterface.h"
#include "common/testcard.h"
#include "common/configurate.h"
#include "promptbox.h"
#include "common/sysinfo.h"

#define CONVEYOR_SLOTNUM 16

#define SLOTCOUNT_MAX 20

#define STOP_INCARD_TIME 8  //反应时间剩余小于此值时 停止进卡 准备移动卡槽


class CDetectWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CDetectWidget(QWidget *parent = 0);

    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

//    void insertnewcard();

    void leftshiftnstep(int nstep);
    bool needtoscanqr();
    bool needtodetect();
    void updatainfo(XTestCard *card);
    int hascandetcard(); //获取是否还有待测卡
    void clearcard();
    void setcardbatch(XTestCard *card,QString batch);
signals:
    void SendCurveData(QByteArray data, ENUM_WIDGET returnWg);

public slots:
    void slotChangeID(bool isBatch);
    void SetCurveData(QByteArray data, ENUM_WIDGET returnWg);
    void SetUpdateID(QString qStrID);

private slots:
    void slotclickback();
    void slottabclick(XTestCard *card);
    void slotclickclear();
    void slotDetect();
    void slotincard();
    void slotincardsucc();
    void slotqrscan();
    void slotdetectstart();
    void slotdetectsucc();
    void SlotWaitCode(QString batch);
    void Slotrun1step();
    void Slotrunend();
    void Slotcheckover();
    void Slotpromptok(int ser);
    void Slotpromptcs(int ser);
    void Slotsetbatch(XTestCard *card,QString batch);
#if defined(DEVICE_TEST)
    void slotDeviceTest();
#endif

private:
    int m_currcardnum;
    bool m_runcheck;
    XTestCard *m_conveeg[WAITTEST_MAX];     //皮带上对应的卡
    XTestCard *m_scancard;                  //保存正在扫描卡 防止移动时 位置对不上
    XTestCard *m_detectcard;                //保存正在测试卡

    bool m_bShowBatch; //true: batch, false: ID

    XTableWidget *m_xtabwg;
    xDrawCaseWidget *m_drawcase;
    XDrawInfoWidget *m_drawinfo;

#if defined(DEVICE_TEST)
    bool m_test;
    QTimer * m_pTimerDeviceTest;
#endif

    QTimer *m_detecttime;

    //QList<SLOT_DATA> m_listslot;

    int m_currdtime; //当前正在测试反应时间
    XPromptBox *m_promptbox;
    QString m_lastbatch;
    XTestCard *m_needbatch;

    bool m_currunstop; //确保收到皮带停止信号

    unsigned int m_jiffyes;
};

#endif // DETECTWIDGET_H
