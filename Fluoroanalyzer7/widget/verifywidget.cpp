#include "verifywidget.h"
#include "common/configurate.h"

CVerifyWidget::CVerifyWidget(QWidget *parent) :
    CBaseFunWidget(parent)
{
    //m_vfvint = CCommonInterface::get_verify();

    m_sysinfo = new QTableWidget(this);
    m_sysinfo->setStyleSheet("color:rgb(0, 0, 0);" + FONTFAMILY + FONT_SIZE_6);
    m_sysinfo->setGeometry(10,80, 650, 340);
    m_sysinfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //m_sysinfo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_sysinfo->setSelectionBehavior(QTableWidget::SelectRows);
    m_sysinfo->setEditTriggers(QTableWidget::NoEditTriggers);
    m_sysinfo->setSelectionMode(QAbstractItemView::SingleSelection);
    //m_sysinfo->verticalHeader()->setVisible(false);   //隐藏列表头
    //m_sysinfo->horizontalHeader()->setVisible(false);
    m_sysinfo->verticalHeader()->setMinimumWidth(35);
    m_sysinfo->horizontalHeader()->setMinimumHeight(45);
    m_sysinfo->verticalScrollBar()->setMinimumWidth(10);
//    m_sysinfo->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    m_sysinfo->setColumnCount(2);
    m_sysinfo->setColumnWidth(0,270);
    m_sysinfo->setColumnWidth(1,325);


    int show_num = CSysInfo::get_cfgnum();
    QStringList headers;
    headers << QObject::tr("Configuration") <<QObject::tr("Value");
    m_sysinfo->setHorizontalHeaderLabels(headers);

    m_sysinfo->setRowCount(show_num);
    for(int row=0;row<show_num;row++)
    {
        m_sysinfo->setItem(row,0,new QTableWidgetItem());
        m_sysinfo->item(row,0)->setText(CSysInfo::get_cfgcname((CFGTYPE)row));
        m_sysinfo->setItem(row,1,new QTableWidgetItem());
        m_sysinfo->item(row,1)->setText(CSysInfo::get_cfg((CFGTYPE)row).toString());
    }


    m_edit = new XButton(this);
    m_edit->setGeometry(680,120,100,50);
    m_edit->setText(tr("Edit"));
    connect(m_edit, SIGNAL(sigbtnclick()), this, SLOT(slotclickedit()));
    m_edit->setEnabled(false);

    m_save = new XButton(this);
    m_save->setGeometry(680,220,100,50);
    m_save->setText(tr("Save"));
    connect(m_save, SIGNAL(sigbtnclick()), this, SLOT(slotclicksave()));

    m_back = new XButton(this);
    m_back->setGeometry(680,320,100,50);
    m_back->setText(tr("Back"));
    connect(m_back, SIGNAL(sigbtnclick()), this, SLOT(slotclickback()));
}

void CVerifyWidget::showEvent(QShowEvent *)
{
    update_table();
}
void CVerifyWidget::update_table()
{
    int show_num = CSysInfo::get_cfgnum();
    for(int row=0;row<show_num;row++)
    {
        m_sysinfo->item(row,0)->setText(CSysInfo::get_cfgcname((CFGTYPE)row));
        m_sysinfo->item(row,1)->setText(CSysInfo::get_cfg((CFGTYPE)row).toString());
    }
}


void CVerifyWidget::slotclickedit()
{

}

void CVerifyWidget::slotclicksave()
{

    //emit sigperform(MENU_WG);
}
void CVerifyWidget::slotclickback()
{
    emit sigperform(MENU_WG);
}




