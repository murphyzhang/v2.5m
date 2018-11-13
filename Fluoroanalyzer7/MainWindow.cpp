#include "MainWindow.h"
#include "ui_MainWindow.h"

#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    // this->setGeometry(0, 0, this->width(), this->height());
   /* move((QApplication::desktop()->width() - this->width())/2,
         (QApplication::desktop()->height() - this->height())/2);*/
    //this->setGeometry(0, 0, 800, 480);
//    qDebug()<<this->width()<<this->height();

    m_sysinit = new CSysInitWidget(this);
    m_sysinit->hide();
    connect(m_sysinit, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(INIT_WG,m_sysinit);

    m_check = new CSelfCheckWidget(this);
    m_check->hide();
    connect(m_check, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(CHECK_WG,m_check);

    m_waitmennu = new CWaitWidget(this);
    m_waitmennu->hide();
    connect(m_waitmennu, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(WAIT_WG,m_waitmennu);

    m_ygCurveWidget = new CygCurveWidget(this);
    m_ygCurveWidget->hide();
    connect(m_ygCurveWidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(YG_WG, m_ygCurveWidget);

    m_detectwidget = new CDetectWidget(this);
    m_detectwidget->hide();
    connect(m_detectwidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));

    widgetmap.insert(DETET_WG,m_detectwidget);
    connect(m_detectwidget, SIGNAL(SendCurveData(QByteArray,ENUM_WIDGET)), m_ygCurveWidget, SLOT(slotSetData(QByteArray, ENUM_WIDGET)));

    m_menuwidget = new CMenuWidget(this);
    m_menuwidget->hide();
    connect(m_menuwidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(MENU_WG,m_menuwidget);

    m_timewidget = new CTimeEditWidget(this);
    m_timewidget->hide();
    connect(m_timewidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(TIME_WG,m_timewidget);

    m_verifywidget= new CVerifyWidget(this);
    m_verifywidget->hide();
    connect(m_verifywidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(VERI_WG,m_verifywidget);

    //CDataBase::Init(X_BTDB_PATH);

    m_recordwidget = new CRecordWidget(this);
    m_recordwidget->hide();
    connect(m_recordwidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(RECD_WG,m_recordwidget);

    m_batchwidget = new CBatchWidget(this);
    m_batchwidget->hide();
    connect(m_batchwidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(BATCH_WG,m_batchwidget);

    m_samplewidget = new CSampleWidget(this);
    m_samplewidget->hide();
    connect(m_samplewidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    connect(m_samplewidget,SIGNAL(BtDetailClicked(int)),this,SLOT(ShowTestInfo(int)));
    widgetmap.insert(SAMP_WG,m_samplewidget);

    m_aboutwidget = new CAboutWidget(this);
    m_aboutwidget->hide();
    connect(m_aboutwidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(ABOUT_WG,m_aboutwidget);

    m_syssetwidget = new CSyssetWidget(this);
    m_syssetwidget->hide();
    connect(m_syssetwidget, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    connect(m_syssetwidget, SIGNAL(sigTestWidgetChangeID(bool)),    m_detectwidget, SLOT(slotChangeID(bool)));
    connect(m_syssetwidget, SIGNAL(sigSampleWidgetChangeID(bool)),  m_samplewidget, SLOT(slotChangeID(bool)));
    widgetmap.insert(SET_WG,m_syssetwidget);

    m_pDetailTest = new CDetailTest(this);
    m_pDetailTest->hide();
    connect(m_pDetailTest, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(DETAIL_TEST_WG, m_pDetailTest);
    connect(m_pDetailTest,SIGNAL(SigCureTrace(int,QString,QString,QString)),this,SLOT(ShowCureTrace(int,QString,QString,QString)));

    m_pCureTrace = new CCureTrace(this);
    m_pCureTrace->hide();
    connect(m_pCureTrace, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(CURETRACE_WG, m_pCureTrace);
    connect(m_pCureTrace,SIGNAL(sigShowPlotLines(QVector<CTestData*>)),this,SLOT(ShowPlotLines(QVector<CTestData*>)));

    m_pPlotLines = new CPlotLines(this);
    m_pPlotLines->hide();
    connect(m_pPlotLines, SIGNAL(sigperform(ENUM_WIDGET)), this, SLOT(slotswitchpage(ENUM_WIDGET)));
    widgetmap.insert(PLOTLINES_WG, m_pPlotLines);

    connect(m_samplewidget,SIGNAL(sinSetYgCurveData(QByteArray, ENUM_WIDGET)), m_ygCurveWidget, SLOT(slotSetData(QByteArray, ENUM_WIDGET)));

    m_currwidget = NULL;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotswitchpage(ENUM_WIDGET turn_wg)
{
    if(m_currwidget != NULL)
    {
        m_currwidget->hide();
    }

    m_currwidget = widgetmap.value(turn_wg,m_currwidget);

    if(m_currwidget)
    {
        qDebug() << "show" << turn_wg << "window";
        m_currwidget->show();
        m_currWinIndex = turn_wg;
    }
    else
    {
        qDebug() << "MainWindow::slotswitchpage()" << turn_wg << " window is NULL";
    }
}

void MainWindow::showEvent(QShowEvent *)
{
    slotswitchpage(INIT_WG);
}

ENUM_WIDGET MainWindow::getCurrWinIndex()
{
    return m_currWinIndex;
}

void MainWindow::ShowTestInfo(int iSer)
{
    m_pDetailTest->SetValue(iSer);
    slotswitchpage(DETAIL_TEST_WG);
}

void MainWindow::ShowCureTrace(int iSer, QString qStrName, QString qStrSex, QString qStrAge)
{
    m_pCureTrace->SetPatientInfo(iSer,qStrName,qStrSex,qStrAge);
    slotswitchpage(CURETRACE_WG);
}

void MainWindow::ShowPlotLines(QVector<CTestData *> vecData)
{
    qDebug()<<"ShowPlotLines";
    m_pPlotLines->SetValue(vecData);
    slotswitchpage(PLOTLINES_WG);
}







