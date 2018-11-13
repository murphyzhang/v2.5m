#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include "basefunwidget.h"

class CMenuWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    CMenuWidget(QWidget *parent = 0);
signals:

public slots:
    void slotclickrecord();
    void slotclickset();
    void slotclickcheck();
    void slotclicktime();
    void slotclickabout();
    void slotclickback();
private:
    XButton *m_record;
    XButton *m_set;
    XButton *m_check;
    XButton *m_time;
    XButton *m_about;
    XButton *m_back;
};

#endif // MENUWIDGET_H
