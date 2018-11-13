#include "aboutwidget.h"idget.h"
#include "common/configurate.h"


CAboutWidget::CAboutWidget(QWidget *parent) :
    CBaseFunWidget(parent)
{
    for(int nTmp = 0; nTmp < 5; nTmp++)
    {
        m_pName[nTmp] = new XLabel(this);
        m_pName[nTmp]->setGeometry(180, 100+60*nTmp,200, 50);
        m_pValu[nTmp] = new XLabel(this);
        m_pValu[nTmp]->setGeometry(380, 100+60*nTmp,300, 50);
    }
    m_pName[0]->setText(tr("Name:"));
    m_pName[1]->setText(tr("Type:"));
    m_pName[2]->setText(tr("SN:"));
    m_pName[3]->setText(tr("Ver No.:"));
    m_pName[4]->setText(tr("Tel:"));

    m_pValu[0]->setText(tr("FIA Analyzer"));
    m_pValu[1]->setText(tr("BIOT-YG-II"));
    m_pValu[2]->setText("");

#ifdef PET_HOSPITAL
    m_pValu[3]->setText(CCommonInterface::get_sysversion()+QString("-PET"));
#else
    m_pValu[3]->setText(CCommonInterface::get_sysversion());
#endif

    m_pValu[4]->setText("400-0592-711");
}

void CAboutWidget::showEvent(QShowEvent *)
{
    QString sn = CSysInfo::get_cfg(CFG_QT_SNCODE).toString();
    m_pValu[2]->setText(sn);
}

void CAboutWidget::mouseReleaseEvent(QMouseEvent *)
{
    emit sigperform(MENU_WG);
}
