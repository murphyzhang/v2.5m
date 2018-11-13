#include "syssetwidget.h"
#include "common/configurate.h"

CSyssetWidget::CSyssetWidget(QWidget *parent): CBaseFunWidget(parent)
{
    /*
    m_basewg = new QWidget();
    m_basewg->setGeometry(0,0,410,170);

    m_serport = new QWidget();
    m_serport->setGeometry(0,0,410,170);

    m_serbautname = new XLabel(m_serport);
    m_serbautname->setStyleSheet(FONTFAMILY + FONT_SIZE_4);
    m_serbautname->setGeometry(30,20,100,20);
    m_serbautname->setText(tr("波特率："));

    m_serbautvaule = new QComboBox(m_serport);
    m_serbautvaule->setGeometry(130,20,100,20);
    m_serbautvaule->addItem("9600");

    m_tabwg = new QTabWidget(this);
    m_tabwg->setGeometry(20,30,440,172);
    m_tabwg->addTab(m_basewg,tr("基本"));
    m_tabwg->addTab(m_serport,tr("串口"));
    */
    m_recover = new XButton(this);
    m_recover->setGeometry(140,370,120,40);
    m_recover->setText(tr("Initialize"));
    connect(m_recover, SIGNAL(sigbtnclick()), this, SLOT(slotclickrecover()));

    m_save = new XButton(this);
    m_save->setGeometry(320,370,120,40);
    m_save->setText(tr("Save"));
    connect(m_save, SIGNAL(sigbtnclick()), this, SLOT(slotclicksave()));

    m_back = new XButton(this);
    m_back->setGeometry(500,370,120,40);
    m_back->setText(tr("Back"));
    connect(m_back, SIGNAL(sigbtnclick()), this, SLOT(slotclickback()));

    m_promptbox = new XPromptBox();
    m_promptbox->hide();
    connect(m_promptbox, SIGNAL(sigclickok(int)), this, SLOT(slotpromptok(int)));
    connect(m_promptbox, SIGNAL(sigclickcancel(int)), this, SLOT(slotpromptcs(int)));

    m_printf = new QCheckBox(this);
    m_printf->setStyleSheet("QCheckBox::indicator {width: 30px; height: 30px;}QCheckBox::indicator:checked{background:darkBlue};" + FONTFAMILY + FONT_SIZE_4);
    m_printf->setText(tr("End of test, print.(Please slide the back switch to “Print”)"));
    m_printf->setGeometry(60, 90, 800, 50);
    //connect(m_printf, SIGNAL(clicked()), this, SLOT(slotclickprintf()));

    m_pfrefer = new QCheckBox(this);
    m_pfrefer->setStyleSheet("QCheckBox::indicator {width: 30px; height: 30px;}QCheckBox::indicator:checked{background:darkBlue};" + FONTFAMILY + FONT_SIZE_4);
    m_pfrefer->setText(tr("Print test result included reference value"));
    m_pfrefer->setGeometry(60, 140, 800, 50);
    //connect(m_pfrefer, SIGNAL(clicked()), this, SLOT(slotclickprintf()));
    //m_printf->setStyleSheet("QCheckBox::indicator:checked {image: url(:/image/checkbox.png);}");

    m_testWinIdBatchChange = new QCheckBox(this);
    m_testWinIdBatchChange->setStyleSheet("QCheckBox::indicator {width: 30px; height: 30px;}QCheckBox::indicator:checked{background:darkBlue};" + FONTFAMILY + FONT_SIZE_4);
//    m_testWinIdBatchChange->setText("测试界面切换批号和ID显示(当前显示批号)");
    m_testWinIdBatchChange->setGeometry(60, 190, 800, 50);
    m_testWinIdBatchChange->hide();
    connect(m_testWinIdBatchChange, SIGNAL(clicked()), this, SLOT(slotTestWinIdBatchChange()));

    m_recordWinIdBatchChange = new QCheckBox(this);
    m_recordWinIdBatchChange->setStyleSheet("QCheckBox::indicator {width: 30px; height: 30px;}QCheckBox::indicator:checked{background:darkBlue};" + FONTFAMILY + FONT_SIZE_4);
//    m_recordWinIdBatchChange->setText("样本记录界面切换批号和ID显示(当前显示批号)");
    m_recordWinIdBatchChange->setGeometry(60, 240, 800, 50);
    m_recordWinIdBatchChange->hide();
    connect(m_recordWinIdBatchChange, SIGNAL(clicked()), this, SLOT(slotRecordWinIdBatchChange()));
}

void CSyssetWidget::showEvent(QShowEvent *)
{
    m_printf->setChecked(CSysInfo::get_cfg(CFG_QT_AUTOPRT).toBool());
    m_pfrefer->setChecked(CSysInfo::get_cfg(CFG_QT_PRTREF).toBool());

    m_testWinIdBatchChange->setChecked(CSysInfo::get_cfg(CFG_QT_TWC).toBool());
    slotTestWinIdBatchChange();

    m_recordWinIdBatchChange->setChecked(CSysInfo::get_cfg(CFG_QT_RWC).toBool());
    slotRecordWinIdBatchChange();
}

void CSyssetWidget::slotclicksave()
{
    CSysInfo::set_cfg(CFG_QT_AUTOPRT, m_printf->isChecked());
    CSysInfo::set_cfg(CFG_QT_PRTREF, m_pfrefer->isChecked());

    if (CSysInfo::set_cfg(CFG_QT_TWC, m_testWinIdBatchChange->isChecked()))
        emit sigTestWidgetChangeID(m_testWinIdBatchChange->isChecked());

    if (CSysInfo::set_cfg(CFG_QT_RWC, m_recordWinIdBatchChange->isChecked()))
        emit sigSampleWidgetChangeID(m_recordWinIdBatchChange->isChecked());

    emit sigperform(MENU_WG);
}

void CSyssetWidget::slotclickback()
{
    emit sigperform(MENU_WG);
}

void CSyssetWidget::slotclickprintf()
{

}

void CSyssetWidget::slotclickrecover()
{
    this->setEnabled(false);
    m_promptbox->settitle(tr("此操作将删除所有保存数据，恢复到出厂状态！\n\n确认：执行恢复    取消：返回"));
    m_promptbox->setnsec(6);
    m_promptbox->setwhichbutton(1);
    m_promptbox->setpromptser(0);
    m_promptbox->show();
}

void CSyssetWidget::slotpromptok(int ser)
{
    system("rm "+QByteArray(X_BTDB_PATH));
    system("reboot");
    this->setEnabled(true);
}

void CSyssetWidget::slotpromptcs(int ser)
{
    this->setEnabled(true);
}

void CSyssetWidget::slotTestWinIdBatchChange()
{
    if (m_testWinIdBatchChange->isChecked())
        m_testWinIdBatchChange->setText(tr("测试界面切换批号和ID显示(当前显示批号)"));
    else
        m_testWinIdBatchChange->setText(tr("测试界面切换批号和ID显示(当前显示ID)"));
}

void CSyssetWidget::slotRecordWinIdBatchChange()
{
    if (m_recordWinIdBatchChange->isChecked())
        m_recordWinIdBatchChange->setText(tr("样本记录界面切换批号和ID显示(当前显示批号)"));
    else
        m_recordWinIdBatchChange->setText(tr("样本记录界面切换批号和ID显示(当前显示ID)"));
}




