#ifndef SYSSETWIDGET_H
#define SYSSETWIDGET_H

#include "basefunwidget.h"

#include "common/commoninterface.h"
#include "promptbox.h"

class CSyssetWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CSyssetWidget(QWidget *parent = 0);
    void showEvent(QShowEvent *);

signals:
    void sigTestWidgetChangeID(bool isBatch);
    void sigSampleWidgetChangeID(bool isBatch);

private slots:
    void slotclicksave();
    void slotclickback();
    void slotclickrecover();
    void slotclickprintf();

    void slotpromptok(int ser);
    void slotpromptcs(int ser);

    void slotTestWinIdBatchChange();
    void slotRecordWinIdBatchChange();

private:
    XButton *m_save;
    XButton *m_back;
    XButton *m_recover;

    QTabWidget *m_tabwg;
    QWidget *m_basewg;
    QWidget *m_serport;
    QWidget *m_usbport;
    QWidget *m_netport;

    XLabel *m_serbautname;
    QComboBox *m_serbautvaule;

    QCheckBox *m_printf;
    QCheckBox *m_pfrefer;
    QCheckBox *m_testWinIdBatchChange;
    QCheckBox *m_recordWinIdBatchChange;

    XPromptBox *m_promptbox;

};

#endif // SYSSETWIDGET_H
