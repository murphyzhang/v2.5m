#ifndef VERIFYWIDGET_H
#define VERIFYWIDGET_H

#include "basefunwidget.h"
#include "common/commoninterface.h"
#include "common/sysinfo.h"
class CVerifyWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CVerifyWidget(QWidget *parent = 0);
    void showEvent(QShowEvent *);
    void update_table();
signals:

public slots:
    void slotclickedit();
    void slotclicksave();
    void slotclickback();
private:

    XButton *m_save;
    XButton *m_back;
    XButton *m_edit;

    int m_vfvint;

    QTableWidget *m_sysinfo;
};

#endif // VERIFYWIDGET_H
