#ifndef XDRAWINFOWIDGET_H
#define XDRAWINFOWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include "base/xlabel.h"

class XDrawInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XDrawInfoWidget(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *e);

    void set_intimetext(QString text);  //插卡时间
    void set_dttimetext(QString text);  //测试时间
    void set_statetext(QString text);   //状态
    void set_storetext(QString text);   //存储

    void cleartext();

    void set_showinfo();
    void set_shownote();
    void set_notetext(QString text);
    void set_updated(QString text);
    void UpdateID(QString qStrID);
    void ClearID();

signals:

private slots:
    void onClick();

private:
    void showEvent(QShowEvent *);

private:    
    bool m_bShowBatch; //true: batch, false: ID

    QGraphicsView *m_view;
    QGraphicsScene *m_scene;
#if 0
    QGraphicsSimpleTextItem *m_intimename;
    QGraphicsSimpleTextItem *m_dttimename;
    QGraphicsSimpleTextItem *m_statename;
    QGraphicsSimpleTextItem *m_storename;

    QGraphicsSimpleTextItem *m_intime;
    QGraphicsSimpleTextItem *m_dttime;
    QGraphicsSimpleTextItem *m_state;
    QGraphicsSimpleTextItem *m_store;

    QGraphicsSimpleTextItem *m_notename;
    QGraphicsSimpleTextItem *m_note1;
    QGraphicsSimpleTextItem *m_updatedname;
    QGraphicsSimpleTextItem *m_note2;
#else
    XLabel *m_intimename;
    XLabel *m_dttimename;
    XLabel *m_statename;
    XLabel *m_storename;
    XLabel *m_intime;
    XLabel *m_dttime;
    XLabel *m_state;
    XLabel *m_store;
    XLabel *m_notename;
    XLabel *m_note1;
    XLabel *m_updatedname;
    XLabel *m_note2;
#endif
    int m_curmode;
};

#endif // XDRAWINFOWIDGET_H
