#include "DetailTest.h"

CDetailTest::CDetailTest(QWidget *parent) :
    CBaseFunWidget(parent)
{
    InitUI();
}

CDetailTest::~CDetailTest()
{
    //
}

void CDetailTest::InitUI()
{
    QFont font;
    font.setPointSize(14);
    font.setFamily(QObject::tr("wenquanyi"));

    m_pLbTitle = new XLabel(this);
    //m_pLbTitle->setText(tr("测试详情"));
    m_pLbTitle->setText(tr("Test details"));
    m_pLbTitle->setGeometry(0, 40, 800, 40);
    m_pLbTitle->setAlignment(Qt::AlignCenter);

    m_pLbID = new XLabel(this);
    m_pLbID->setText(tr("ID："));
    m_pLbID->setGeometry(40, 80, 40, 40);
    m_pLbID->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbID_2 = new XLabel(this);
    m_pLbID_2->setText(tr(""));
    m_pLbID_2->setGeometry(80,80,120,40);
    m_pLbID_2->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbName = new XLabel(this);
    //m_pLbName->setText(tr("姓名："));
    m_pLbName->setText(tr("Name："));
    m_pLbName->setGeometry(200, 80, 80, 40);
    m_pLbName->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbName_2 = new XLabel(this);
    m_pLbName_2->setText(tr(""));
    m_pLbName_2->setGeometry(280,80,120,40);
    m_pLbName_2->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbSex = new XLabel(this);
    //m_pLbSex->setText(tr("性别："));
    m_pLbSex->setText(tr("Sex："));
    m_pLbSex->setGeometry(400, 80, 60, 40);
    m_pLbSex->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbSex_2 = new XLabel(this);
    m_pLbSex_2->setText(tr(""));
    m_pLbSex_2->setGeometry(480, 80, 90, 40);
    m_pLbSex_2->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbAge = new XLabel(this);
    //m_pLbAge->setText(tr("年龄："));
    m_pLbAge->setText(tr("Age："));
    m_pLbAge->setGeometry(570, 80, 80, 40);
    m_pLbAge->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbAge_2 = new XLabel(this);
    m_pLbAge_2->setText(tr(""));
    m_pLbAge_2->setGeometry(650,80,60,40);
    m_pLbAge_2->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbSampleNo= new XLabel(this);
    //m_pLbSampleNo->setText(tr("样本编号："));
    m_pLbSampleNo->setText(tr("Sample SN："));
    m_pLbSampleNo->setGeometry(40, 120, 130, 40);
    m_pLbSampleNo->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbSampleNo_2 = new XLabel(this);
    m_pLbSampleNo_2->setText(tr(""));
    m_pLbSampleNo_2->setGeometry(150,120,200,40);
    m_pLbSampleNo_2->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbSampleType = new XLabel(this);
    //m_pLbSampleType->setText(tr("样本类型："));
    m_pLbSampleType->setText(tr("Sample："));
    m_pLbSampleType->setGeometry(400, 120, 110, 40);
    m_pLbSampleType->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLbSampleType_2 = new XLabel(this);
    m_pLbSampleType_2->setText(tr(""));
    m_pLbSampleType_2->setGeometry(510,120,200,40);
    m_pLbSampleType_2->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pTestRecord = new QTableWidget(this);
    m_pTestRecord->setStyleSheet("color:rgb(0, 0, 0);" + FONTFAMILY + FONT_SIZE_6);
    m_pTestRecord->setGeometry(30,170,740,194);
    m_pTestRecord->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTestRecord->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTestRecord->setSelectionBehavior(QTableWidget::SelectRows);
    m_pTestRecord->setEditTriggers(QTableWidget::NoEditTriggers);
    m_pTestRecord->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTestRecord->verticalHeader()->setVisible(false);   //隐藏列表头
    //m_pTestRecord->horizontalHeader()->setVisible(false);
    m_pTestRecord->horizontalHeader()->setMaximumHeight(40);
    m_pTestRecord->horizontalHeader()->setMinimumHeight(40);
    m_pTestRecord->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    m_pTestRecord->setColumnCount(6);
    m_pTestRecord->setColumnWidth(0,80);
    m_pTestRecord->setColumnWidth(1,130);
    m_pTestRecord->setColumnWidth(2,120);
    m_pTestRecord->setColumnWidth(3,160);
    m_pTestRecord->setColumnWidth(4,140);
    m_pTestRecord->setColumnWidth(5,110);

    m_pTestRecord->setRowCount(5);
    for(int row = 0; row < 5; row++)
    {
        m_pTestRecord->setItem(row,0,new QTableWidgetItem());
        m_pTestRecord->item(row,0)->setText("");
        m_pTestRecord->item(row,0)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,1,new QTableWidgetItem());
        m_pTestRecord->item(row,1)->setText("");
        m_pTestRecord->item(row,1)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,2,new QTableWidgetItem());
        m_pTestRecord->item(row,2)->setText("");
        m_pTestRecord->item(row,2)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,3,new QTableWidgetItem());
        m_pTestRecord->item(row,3)->setText("");
        m_pTestRecord->item(row,3)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,4,new QTableWidgetItem());
        m_pTestRecord->item(row,4)->setText("");
        m_pTestRecord->item(row,4)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,5,new QTableWidgetItem());
        m_pTestRecord->item(row,5)->setText("");
        m_pTestRecord->item(row,5)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setRowHeight(row,30);
    }
    QStringList headers;
    //headers << tr("流水号") << tr("日期") << tr("批次") << tr("项目") << tr("结果") << tr("单位");
    headers << tr("Serial") << tr("Date") << tr("Lot") << tr("Item") << tr("Result") << tr("Unit");
    m_pTestRecord->setHorizontalHeaderLabels(headers);

    m_pBtCureTrace = new XButton(this);
    //m_pBtCureTrace->setText(tr("病情跟踪"));
    m_pBtCureTrace->setText(tr("Trend"));
    m_pBtCureTrace->setGeometry(420,390,120,40);
    m_pBtCureTrace->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pBtCureTrace,SIGNAL(sigbtnclick()),this,SLOT(BtCureTrace()));

    m_pBtReturn = new XButton(this);
    //m_pBtReturn->setText(tr("返回"));
    m_pBtReturn->setText(tr("Back"));
    m_pBtReturn->setGeometry(600,390,75,40);
    m_pBtReturn->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pBtReturn,SIGNAL(sigbtnclick()),this,SLOT(BtReturnClicked()));
}

void CDetailTest::showEvent(QShowEvent *)
{
    //CI::showBiotimeTitle(false);
    ShowHistory(QString::number(m_iSerNum));
}

void CDetailTest::Clear()
{
    m_pLbID_2->clear();
    m_pLbName_2->clear();
    m_pLbSex_2->clear();
    m_pLbAge_2->clear();
    m_pLbSampleNo_2->clear();
    m_pLbSampleType_2->clear();

    m_pTestRecord->clearContents();

    for(int row = 0; row < 5; row++)
    {
        m_pTestRecord->setItem(row,0,new QTableWidgetItem());
        m_pTestRecord->item(row,0)->setText("");
        m_pTestRecord->item(row,0)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,1,new QTableWidgetItem());
        m_pTestRecord->item(row,1)->setText("");
        m_pTestRecord->item(row,1)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,2,new QTableWidgetItem());
        m_pTestRecord->item(row,2)->setText("");
        m_pTestRecord->item(row,2)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,3,new QTableWidgetItem());
        m_pTestRecord->item(row,3)->setText("");
        m_pTestRecord->item(row,3)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,4,new QTableWidgetItem());
        m_pTestRecord->item(row,4)->setText("");
        m_pTestRecord->item(row,4)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setItem(row,5,new QTableWidgetItem());
        m_pTestRecord->item(row,5)->setText("");
        m_pTestRecord->item(row,5)->setTextAlignment(Qt::AlignCenter);
        m_pTestRecord->setRowHeight(row,30);
    }
}

void CDetailTest::SetValue(int iSerNum)
{
    m_iSerNum = iSerNum;
}

void CDetailTest::BtCureTrace()
{
    if(m_pLbName_2->text().isEmpty())
    {
        return;
    }
    else
    {
        SigCureTrace(m_pLbID_2->text().toInt(),m_pLbName_2->text().trimmed(),m_pLbSex_2->text().trimmed(),m_pLbAge_2->text().trimmed());
    }
//    Clear();
//    this->hide();
//    emit sigperform(CURETRACE_WG);
}

void CDetailTest::BtReturnClicked()
{
    Clear();
    this->hide();
    emit sigperform(SAMP_WG);
}

void CDetailTest::ShowHistory(QString qStrSerNum)
{
    QSqlQuery sqlQuery = CDataBase::select_Record(tr("PATIENT_BODY"),tr("SERNUM"),qStrSerNum);
    if(sqlQuery.next())
    {
        QString id = sqlQuery.value(1).toString();
        QString name = sqlQuery.value(2).toString();
        QString sex = sqlQuery.value(3).toString();
        QString age = sqlQuery.value(4).toString();
        QString sampleNum = sqlQuery.value(5).toString();
        QString sampleType = sqlQuery.value(6).toString();

        if(id.isEmpty())
        {
            QSqlQuery query = CDataBase::select_Record(tr("TABLE_BODY"),tr("SERNUM"),qStrSerNum);
            if(query.next())
            {
                id = query.value(22).toString();
            }
        }

        m_pLbID_2->setText(id);
        m_pLbName_2->setText(name);
        m_pLbSex_2->setText(sex);
        m_pLbAge_2->setText(age);
        m_pLbSampleNo_2->setText(sampleNum);
        m_pLbSampleType_2->setText(sampleType);
    }
    else
    {
        QSqlQuery query = CDataBase::select_Record(tr("TABLE_BODY"),tr("SERNUM"),qStrSerNum);
        QString id;
        if(query.next())
        {
            id = query.value(22).toString();
        }
        m_pLbID_2->setText(id);
    }

    QSqlQuery query = CDataBase::select_Record(tr("TABLE_BODY"),tr("SERNUM"),qStrSerNum);
    if(query.next())
    {
        int iUnite = query.value(3).toInt();
        int iFlag = 4;
        m_pTestRecord->item(0,0)->setText(query.value(0).toString());
        m_pTestRecord->item(0,1)->setText(query.value(2).toString().left(10));
        m_pTestRecord->item(0,2)->setText(query.value(1).toString());
        for(int i=0;i<iUnite;i++)
        {
            m_pTestRecord->item(i,3)->setText(query.value(iFlag).toString());
            iFlag++;
            m_pTestRecord->item(i,5)->setText(query.value(iFlag).toString());
            iFlag++;
            m_pTestRecord->item(i,4)->setText(query.value(iFlag).toString());
            iFlag++;
        }
    }
}



