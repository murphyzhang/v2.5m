#ifndef TIMEEDITWIDGET_H
#define TIMEEDITWIDGET_H

#include "basefunwidget.h"

class CTimeEditWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CTimeEditWidget(QWidget *parent = 0);
    void showEvent(QShowEvent *);
    void timetoshow();
    int SetCurDateTime(QString strTime);
signals:

public slots:
    void slotclickadd();
    void slotclicksub();

    void slotclicksave();
    void slotclickback();

    void slotclicktime();
private:
    XButton *m_add;
    XButton *m_sub;

    XButton *m_save;
    XButton *m_back;

    QLabel *m_label[6];

    QPushButton *m_dt[6];

    int m_setn;
    QDateTime m_datetime;
};

#endif // TIMEEDITWIDGET_H
