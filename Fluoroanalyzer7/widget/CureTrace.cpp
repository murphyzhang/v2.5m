#include "CureTrace.h"

CCureTrace::CCureTrace(QWidget *parent) :
    CBaseFunWidget(parent)
{
    m_iPos = 0;
    InitUI();
}

CCureTrace::~CCureTrace()
{
    //
}

void CCureTrace::InitUI()
{
    QFont font;
    font.setPointSize(14);
    font.setFamily(QObject::tr("wenquanyi"));

    m_pLbTitle = new XLabel(this);
    //m_pLbTitle->setText(tr("病情跟踪"));
    m_pLbTitle->setText(tr("Trend"));
    m_pLbTitle->setGeometry(0, 40, 800, 40);
    m_pLbTitle->setAlignment(Qt::AlignCenter);

    m_pTestRecord = new QTableWidget(this);
    m_pTestRecord->setStyleSheet("color:rgb(0, 0, 0);" + FONTFAMILY + FONT_SIZE_6);
    m_pTestRecord->setGeometry(30,90,740,280);
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
    m_pTestRecord->setColumnWidth(0,0);     //sernum
    m_pTestRecord->setColumnWidth(1,130);    //date
    m_pTestRecord->setColumnWidth(2,120);    //ID
    m_pTestRecord->setColumnWidth(3,180);   //patient
    m_pTestRecord->setColumnWidth(4,140);    //item
    m_pTestRecord->setColumnWidth(5,170);    //result

    m_pTestRecord->setRowCount(8);
    for(int row = 0; row < 8; row++)
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
    //headers << tr("流水号") << tr("日期") << tr("ID") << tr("患者") << tr("项目") << tr("结果");
    headers << tr("SN") << tr("Date") << tr("ID") << tr("Patient") << tr("Item") << tr("Result");
    m_pTestRecord->setHorizontalHeaderLabels(headers);

    m_pBtUp = new XButton(this);
    m_pBtUp->setText(tr("▲"));
    m_pBtUp->setGeometry(40,400,60,30);
    connect(m_pBtUp,SIGNAL(sigbtnclick()),this,SLOT(BtUpClicked()));

    m_pBtDown = new XButton(this);
    m_pBtDown->setText(tr("▼"));
    m_pBtDown->setGeometry(120,400,60,30);
    connect(m_pBtDown,SIGNAL(sigbtnclick()),this,SLOT(BtDownClicked()));

    m_pBtCureTrace = new XButton(this);
    //m_pBtCureTrace->setText(tr("趋势图"));
    m_pBtCureTrace->setText(tr("Trend"));
    m_pBtCureTrace->setGeometry(420,400,110,40);
    m_pBtCureTrace->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pBtCureTrace,SIGNAL(sigbtnclick()),this,SLOT(BtCureTrace()));

    m_pBtReturn = new XButton(this);
    //m_pBtReturn->setText(tr("返回"));
    m_pBtReturn->setText(tr("Back"));
    m_pBtReturn->setGeometry(600,400,75,40);
    m_pBtReturn->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pBtReturn,SIGNAL(sigbtnclick()),this,SLOT(BtReturnClicked()));
}

void CCureTrace::showEvent(QShowEvent *)
{
    //CI::showBiotimeTitle(true);
    m_iPos = 0;
    ClearTbContent();
    GetDataList();
    ShowTestData(m_iPos);
}

void CCureTrace::GetDataList()
{
    QSqlQuery sqlQuery;
    QSqlQuery query = CDataBase::GetCureTrace(m_sName);

    vecTestData.clear();
    while(query.next())
    {
        int iSer = query.value(0).toInt();
        QString qStrDate = query.value(1).toString().left(10);
        QString qStrID = query.value(2).toString();
        QString qStrName = query.value(3).toString();
        QString qStrSex = query.value(4).toString();
        QString qStrAge = query.value(5).toString();
        int iUnite = query.value(6).toInt();
        QString qStrItem = query.value(7).toString();
        QString qStrRes = query.value(8).toString();
        QString qStrUnit = query.value(9).toString();

        if(qStrSex.compare(m_sSex)==0 && (qStrAge.toInt()==m_sAge.toInt() || qStrAge.toInt()==m_sAge.toInt()+1 || qStrAge.toInt()+1==m_sAge.toInt()))
        {
            CTestData *pTestData = new CTestData;
            pTestData->SetSerNum(iSer);
            pTestData->SetBatch(tr(""));
            pTestData->SetDate(qStrDate);
            pTestData->SetUnite(iUnite);
            pTestData->SetItem(qStrItem);
            pTestData->SetResult(qStrRes);
            pTestData->SetUnit(qStrUnit);

            pTestData->SetID(qStrID);
            pTestData->SetName(qStrName);
            pTestData->SetSex(qStrSex);
            pTestData->SetAge(qStrAge);
            pTestData->SetSampleNum(tr(""));
            pTestData->SetSampleType(tr(""));

            vecTestData.push_back(pTestData);
            if(iUnite > 1)
            {
                int iTemp = 6;
                sqlQuery = CDataBase::select_Record(tr("TABLE_BODY"),tr("SERNUM"),iSer);
                if(sqlQuery.next())
                {
                    for(int i=0;i<(iUnite-1);i++)
                    {
                        CTestData *pTestData = new CTestData;
                        pTestData->SetSerNum(0);
                        pTestData->SetBatch(tr(""));
                        pTestData->SetDate(tr(""));
                        pTestData->SetUnite(0);
                        pTestData->SetItem(sqlQuery.value(++iTemp).toString());
                        pTestData->SetUnit(sqlQuery.value(++iTemp).toString());
                        pTestData->SetResult(sqlQuery.value(++iTemp).toString());

                        pTestData->SetID(tr(""));
                        pTestData->SetName(tr(""));
                        pTestData->SetSex(tr(""));
                        pTestData->SetAge(tr(""));
                        pTestData->SetSampleNum(tr(""));
                        pTestData->SetSampleType(tr(""));

                        vecTestData.push_back(pTestData);
                    }
                }
            }
        }
    }
}

void CCureTrace::ShowTestData(int iPos)
{
    int iRow = 0;
    qDebug()<<"iPos:"<<iPos;
    ClearTbContent();
    for(int i=iPos;i<vecTestData.count();i++)
    {
        qDebug()<<"Row = "<<iRow;
        if(iRow > 7)
            break;
        m_pTestRecord->item(iRow,0)->setText(QString::number(vecTestData.at(i)->GetSerNum()));
        m_pTestRecord->item(iRow,1)->setText(vecTestData.at(i)->GetDate());
        m_pTestRecord->item(iRow,2)->setText(vecTestData.at(i)->GetID());
        if(vecTestData.at(i)->GetName().isEmpty())
        {
            m_pTestRecord->item(iRow,3)->setText(tr(""));
        }
        else
        {
            m_pTestRecord->item(iRow,3)->setText(tr("%1 %2/%3").arg(vecTestData.at(i)->GetName()).arg(vecTestData.at(i)->GetSex()).arg(vecTestData.at(i)->GetAge()));
        }
        m_pTestRecord->item(iRow,4)->setText(vecTestData.at(i)->GetItem());
        m_pTestRecord->item(iRow,5)->setText(tr("%1 %2").arg(vecTestData.at(i)->GetResult()).arg(vecTestData.at(i)->GetUnit()));
        iRow++;
    }
}

void CCureTrace::SetPatientInfo(int iSer, QString qStrName, QString qStrSex, QString qStrAge)
{
    m_iSer = iSer;
    m_sName = qStrName;
    m_sSex = qStrSex;
    m_sAge = qStrAge;
}

void CCureTrace::BtUpClicked()
{
    if(vecTestData.count() < 8 || m_iPos==0)
    {
        return;
    }
    else
    {
        m_iPos  = m_iPos-8;
        ShowTestData(m_iPos);
    }
}

void CCureTrace::BtDownClicked()
{
    if(vecTestData.count() < 8 || (m_iPos+8)>vecTestData.count())
    {
        return;
    }
    else
    {
        m_iPos  = m_iPos+8;
        ShowTestData(m_iPos);
    }
}

void CCureTrace::BtCureTrace()
{
    emit sigShowPlotLines(vecTestData);
    //emit sigperform(PLOTLINES_WG);
}

void CCureTrace::BtReturnClicked()
{
    emit sigperform(DETAIL_TEST_WG);
}

void CCureTrace::ClearTbContent()
{
    for(int row = 0; row < 8; row++)
    {
        m_pTestRecord->item(row,0)->setText("");
        m_pTestRecord->item(row,1)->setText("");
        m_pTestRecord->item(row,2)->setText("");
        m_pTestRecord->item(row,3)->setText("");
        m_pTestRecord->item(row,4)->setText("");
        m_pTestRecord->item(row,5)->setText("");
    }
}

