#include "menuwidget.h"

CMenuWidget::CMenuWidget(QWidget *parent) :
    CBaseFunWidget(parent)
{
    m_record = new XButton(this);
    m_record->setGeometry(140,150,120,50);
    m_record->setText(tr("Records"));
    connect(m_record, SIGNAL(sigbtnclick()), this, SLOT(slotclickrecord()));


    m_set = new XButton(this);
    m_set->setGeometry(340,150,120,50);
    m_set->setText(tr("Setting"));
    connect(m_set, SIGNAL(sigbtnclick()), this, SLOT(slotclickset()));

    m_check = new XButton(this);
    m_check->setGeometry(540,150,120,50);
    m_check->setText(tr("Calibration"));
    connect(m_check, SIGNAL(sigbtnclick()), this, SLOT(slotclickcheck()));

    m_time = new XButton(this);
    m_time->setGeometry(140,280,120,50);
    m_time->setText(tr("Time"));
    connect(m_time, SIGNAL(sigbtnclick()), this, SLOT(slotclicktime()));

    m_about = new XButton(this);
    m_about->setGeometry(340,280,120,50);
    m_about->setText(tr("About"));
    connect(m_about, SIGNAL(sigbtnclick()), this, SLOT(slotclickabout()));

    m_back = new XButton(this);
    m_back->setGeometry(540,280,120,50);
    m_back->setText(tr("Back"));
    connect(m_back, SIGNAL(sigbtnclick()), this, SLOT(slotclickback()));
}

void CMenuWidget::slotclickrecord()
{
    emit sigperform(RECD_WG);
}

void CMenuWidget::slotclickset()
{
    emit sigperform(SET_WG);
}

void CMenuWidget::slotclickcheck()
{
    emit sigperform(VERI_WG);
}

void CMenuWidget::slotclicktime()
{
    emit sigperform(TIME_WG);
}

void CMenuWidget::slotclickabout()
{
    emit sigperform(ABOUT_WG);
}

void CMenuWidget::slotclickback()
{
    emit sigperform(WAIT_WG);
}
