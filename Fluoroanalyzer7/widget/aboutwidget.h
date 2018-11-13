#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include "basefunwidget.h"
#include "common/commoninterface.h"
#include "common/sysinfo.h"

class CAboutWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CAboutWidget(QWidget *parent = 0);
    void showEvent(QShowEvent *);
    void mouseReleaseEvent(QMouseEvent *);
signals:

public slots:

private:
    XLabel *m_pName[5];
    XLabel *m_pValu[5];
};

#endif // ABOUTWIDGET_H
