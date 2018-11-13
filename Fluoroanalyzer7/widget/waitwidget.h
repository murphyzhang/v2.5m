#ifndef WAITWIDGET_H
#define WAITWIDGET_H


#include "basefunwidget.h"
#include "common/commoninterface.h"
#include "base/shutdown.h"


class CWaitWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CWaitWidget(QWidget *parent = 0);


signals:

public slots:
    void slotclickdetec();
    void slotclickmenu();
private:
    XButton *m_detec;
    XButton *m_menu;

    CShutdown * m_btnShutdown;
};

#endif // WAITWIDGET_H
