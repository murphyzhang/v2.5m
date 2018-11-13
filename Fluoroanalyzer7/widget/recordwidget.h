#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include "basefunwidget.h"

class CRecordWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CRecordWidget(QWidget *parent = 0);

signals:

public slots:
    void slotclicksample();
    void slotclickbatch();
    void slotclickback();
private:
    XButton *m_sample;
    XButton *m_batch;
    XButton *m_back;
};

#endif // RECORDWIDGET_H
