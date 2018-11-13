#ifndef XDRAWCASEWIDGET_H
#define XDRAWCASEWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QAbstractScrollArea>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QTimer>

#include "common/commoninterface.h"
#include "common/configfile.h"
#include "common/testcard.h"

#define THIS_WIDTH 300
#define THIS_HEIGTH 80

#define WAITTEST_MAX    14              // 最大等待测试
#define INSERT_CARD_POS 0               // 插入新卡位置
#define SCAN_CARD_POS   INSERT_CARD_POS // 条码扫锚位置
#define TEST_CARD_POS   13              // 测试卡位置

class xDrawCaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit xDrawCaseWidget(QWidget *parent = 0);
    void setnposcolor(int npos,QColor color);
    void setleftnstep(int nstep);
    void setdetetstate(bool start);
    void setincardstate(bool start);
    void setscanqrstate(bool start);
    void updatecase(XTestCard *card[WAITTEST_MAX]);
    void setcardselet(int ncard);
signals:
    void sigcanscanqr();
public slots:
    void drawdetet();
    void drawincard();
    void drawscanqr();
private:

    QGraphicsView *m_view;
    QGraphicsScene *m_scene;
    QGraphicsRectItem *m_case[WAITTEST_MAX];
    QGraphicsRectItem *m_selet;

    QPixmap m_facula;

    QTimer *m_detettimer;
    QGraphicsPixmapItem *m_detet;

    QTimer *m_incardtimer;
    QGraphicsRectItem *m_incard;

    QTimer *m_sacnqrtimer;
    QGraphicsRectItem *m_scanqr;
};

#endif // XDRAWCASEWIDGET_H
