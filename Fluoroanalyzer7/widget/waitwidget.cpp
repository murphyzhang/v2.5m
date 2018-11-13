#include "waitwidget.h"


CWaitWidget::CWaitWidget(QWidget *parent): CBaseFunWidget(parent)
{
    m_detec = new XButton(this);
    m_detec->setGeometry(180,210,120,50);
    m_detec->setText(tr("Test"));
    connect(m_detec, SIGNAL(sigbtnclick()), this, SLOT(slotclickdetec()));

    m_menu = new XButton(this);
    m_menu->setGeometry(500,210,120,50);
    m_menu->setText(tr("Menu"));
    connect(m_menu, SIGNAL(sigbtnclick()), this, SLOT(slotclickmenu()));

    m_btnShutdown = new CShutdown(this);
    m_btnShutdown->setGeometry(20, 375, 50, 50);
}

void CWaitWidget::slotclickdetec()
{
    emit sigperform(DETET_WG);
}

void CWaitWidget::slotclickmenu()
{
    emit sigperform(MENU_WG);
}
