#include "recordwidget.h"

CRecordWidget::CRecordWidget(QWidget *parent) :
    CBaseFunWidget(parent)
{
    m_sample = new XButton(this);
    m_sample->setGeometry(150, 180, 200, 50);
    m_sample->setText(tr("Sample record"));
    connect(m_sample, SIGNAL(sigbtnclick()), this, SLOT(slotclicksample()));

    m_batch = new XButton(this);
    m_batch->setGeometry(450, 180, 200, 50);
    m_batch->setText(tr("Lot record"));
    connect(m_batch, SIGNAL(sigbtnclick()), this, SLOT(slotclickbatch()));

    m_back = new XButton(this);
    m_back->setGeometry(340, 300, 120, 50);
    m_back->setText(tr("Back"));
    connect(m_back, SIGNAL(sigbtnclick()), this, SLOT(slotclickback()));
}

void CRecordWidget::slotclicksample()
{
    emit sigperform(SAMP_WG);
}

void CRecordWidget::slotclickbatch()
{
    emit sigperform(BATCH_WG);
}

void CRecordWidget::slotclickback()
{
    emit sigperform(MENU_WG);
}
