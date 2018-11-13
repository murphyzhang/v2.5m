#include "samplewidget.h"
#include "common/xprintf.h"
#include "common/configurate.h"

#define SHOW_LINE 10

CSampleWidget::CSampleWidget(QWidget *parent): CBaseFunWidget(parent)
{
    m_bSearch = false;
    m_iPos = 0;

    m_pSearchWidget = new CSearchWidget;
    m_pSearchWidget->hide();
    connect(m_pSearchWidget,SIGNAL(EndInput(QString,int)),this,SLOT(RecordQuery(QString,int)));
    connect(m_pSearchWidget,SIGNAL(sigCancel()),this,SLOT(RecordCancel()));

    m_pDeleteChoose = new CDeleteChoose;
    m_pDeleteChoose->hide();
    connect(m_pDeleteChoose,SIGNAL(DelSelect(int)),this,SLOT(DeleteRecord(int)));

    m_spacename = new XLabel(this);
    m_spacename->setText(tr("Data："));
    m_spacename->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    m_spacename->setGeometry(300, 0, 80, 40);

    m_spacevaule = new XLabel(this);
    m_spacevaule->setText(QString("0/")+QString::number(CDataBase::get_RecordMax(TABLE_BODY_NAME)));
    m_spacevaule->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    m_spacevaule->setGeometry(380, 0, 220, 40);

#if 1
    m_samplerecord = new QTableWidget(this);
    m_samplerecord->setStyleSheet("color:rgb(0, 0, 0);" + FONTFAMILY + FONT_SIZE_6);
    m_samplerecord->setGeometry(30,40,740,340);
    m_samplerecord->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_samplerecord->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_samplerecord->setSelectionBehavior(QTableWidget::SelectRows);
    m_samplerecord->setEditTriggers(QTableWidget::NoEditTriggers);
    m_samplerecord->setSelectionMode(QAbstractItemView::SingleSelection);
    m_samplerecord->verticalHeader()->setVisible(false);   //隐藏列表头
    //m_samplerecord->horizontalHeader()->setVisible(false);
    m_samplerecord->horizontalHeader()->setMinimumHeight(40);
    m_samplerecord->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    m_samplerecord->setColumnCount(6);
    m_samplerecord->setColumnWidth(0,80);
    m_samplerecord->setColumnWidth(1,130);
    m_samplerecord->setColumnWidth(2,120);
    m_samplerecord->setColumnWidth(3,200);
    m_samplerecord->setColumnWidth(4,150);
    m_samplerecord->setColumnWidth(5,60);

    m_samplerecord->setRowCount(SHOW_LINE);
    ClearTbContent();
    QStringList headers;
    headers << tr("SN") << tr("Date") << tr("Item") << tr("Result") << tr("ID/Name") << tr("Detail");
    m_samplerecord->setHorizontalHeaderLabels(headers);
    m_samplerecord->setCurrentCell(0,0);
    connect(m_samplerecord, SIGNAL(currentCellChanged (int,int,int,int)), this, SLOT(slotserchange()));
#else
    m_samplelist = new QListWidget(this);
    m_samplelist->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    m_samplelist->setGeometry(340,80,440,240);
    m_samplelist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#endif

    m_up = new XButton(this);
    m_up->setGeometry(60,390,50,40);
    m_up->setText(tr("▲"));
    m_up->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_up, SIGNAL(sigbtnclick()), this, SLOT(slotclickup()));


    m_dw = new XButton(this);
    m_dw->setGeometry(140,390,50,40);
    m_dw->setText(tr("▼"));
    m_dw->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_dw, SIGNAL(sigbtnclick()), this, SLOT(slotclickdw()));

    m_curve = new XButton(this);
    m_curve->setGeometry(220,390,80,40);
    m_curve->setText(tr("Curve"));
    m_curve->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_curve, SIGNAL(sigbtnclick()), this, SLOT(slotClickCurve()));

    m_search = new XButton(this);
    m_search->setGeometry(350,390,80,40);
    m_search->setText(tr("Track"));
    m_search->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_search, SIGNAL(sigbtnclick()), this, SLOT(slotclicksearch()));

    m_printf = new XButton(this);
    m_printf->setGeometry(460,390,80,40);
    m_printf->setText(tr("Print"));
    m_printf->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_printf, SIGNAL(sigbtnclick()), this, SLOT(slotclickprintf()));

    m_dele = new XButton(this);
    m_dele->setGeometry(570,390,80,40);
    m_dele->setText(tr("Del"));
    m_dele->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_dele, SIGNAL(sigbtnclick()), this, SLOT(slotclickdele()));

    m_back = new XButton(this);
    m_back->setGeometry(680,390,80,40);
    m_back->setText(tr("←"));
    m_back->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_back, SIGNAL(sigbtnclick()), this, SLOT(slotclickback()));

    m_promptbox = new XPromptBox();
    m_promptbox->hide();
    connect(m_promptbox, SIGNAL(sigclickok(int)), this, SLOT(slotpromptok(int)));
    connect(m_promptbox, SIGNAL(sigclickcancel(int)), this, SLOT(slotpromptcs(int)));

    bRefreshData = true;
}

void CSampleWidget::showEvent(QShowEvent *)
{
    this->setEnabled(true);
    if (bRefreshData == false)
        return;

    m_curtable = TABLE_BODY_NAME;
    m_spacevaule->setText(QString::number(CDataBase::get_RecordNum(TABLE_BODY_NAME))+"/"+QString::number(CDataBase::get_RecordMax(TABLE_BODY_NAME)));
    int curr = CDataBase::get_MaxAdss(TABLE_BODY_NAME);
    updatarecord(TABLE_BODY_NAME,curr,true);
}

void CSampleWidget::hideEvent(QShowEvent *)
{
    //m_tempquery.exec(QObject::tr(" drop table %0").arg(TMPTB_BODY_NAME));

    QSqlQuery query ;
    query.exec(QObject::tr("VACUUM"));
}

void CSampleWidget::updatarecord(QString table,int first,bool up_down)
{
    if(first <= 0)
        return;

    ClearTbContent();
    if(up_down)//往下更新
    {
        QSqlQuery query = CDataBase::select_Record(table,"SERNUM",QString::number(first),"<=",true);

        for(int i = 0; i < SHOW_LINE; i++)
        {
            if(query.next())
            {
                QString ser = query.value(0).toString();
                QString date = query.value(2).toString().left(10);
                int unite = query.value(3).toString().toInt();
                QString name1 = query.value(4).toString();
                QString unit1 = query.value(5).toString();
                QString rest1 = query.value(6).toString();
                QString name2 = query.value(7).toString();
                QString name3 = query.value(10).toString();
                QString name4 = query.value(13).toString();
                QString name5 = query.value(16).toString();

                m_samplerecord->item(i,0)->setText(ser);
                m_samplerecord->item(i,1)->setText(date);
                if(unite > 1)
                {
                    if(name1.compare(tr("hsCRP")) == 0 && name2.mid(2,3).compare(tr("CRP")) == 0)
                    {
                        m_samplerecord->item(i,2)->setText(tr("CRP"));
                        m_samplerecord->item(i,3)->setText(tr("View details"));
                    }
                    else if(name1.compare(tr("LH")) == 0 && name2.compare(tr("FSH")) == 0 && name3.compare(tr("PRL")) == 0)
                    {
                        m_samplerecord->item(i,2)->setText(tr("3in1(hormone)"));
                        m_samplerecord->item(i,3)->setText(tr("View details"));
                    }
                    else if(name1.compare(tr("cTnI")) == 0 && name2.compare(tr("CK-MB")) == 0 && name3.compare(tr("MYO")) == 0
                            && name4.compare(tr("D-D")) != 0 && name5.compare(tr("NT-ProBNP")) != 0)
                    {
                        m_samplerecord->item(i,2)->setText(tr("3in1(cardiac)"));
                        m_samplerecord->item(i,3)->setText(tr("View details"));
                    }
                    else if(name1.compare(tr("cTnI")) == 0 && name2.compare(tr("CK-MB")) == 0 && name3.compare(tr("MYO")) == 0
                            && name4.compare(tr("D-D")) == 0 && name5.compare(tr("NT-ProBNP")) == 0)
                    {
                        m_samplerecord->item(i,2)->setText(tr("5in1(cardiac)"));
                        m_samplerecord->item(i,3)->setText(tr("View details"));
                    }
                    else
                    {
                        m_samplerecord->item(i,2)->setText(name1);
                        m_samplerecord->item(i,3)->setText(tr("%1 %2").arg(rest1).arg(unit1));
                    }
                }
                else
                {
                    m_samplerecord->item(i,2)->setText(name1);
                    m_samplerecord->item(i,3)->setText(tr("%1 %2").arg(rest1).arg(unit1));
                }
                QSqlQuery sqlQuery = CDataBase::select_Record(tr("PATIENT_BODY"),tr("SERNUM"),ser);

                QString qStrID;
                QString qStrName;
                qDebug()<<"----------------";
                if(sqlQuery.first())
                {
                    qStrID = sqlQuery.value(1).toString();
                    qStrName = sqlQuery.value(2).toString();

                    qDebug()<<"----------updatarecord up_down"<<qStrID;

                    if(!(qStrName.isEmpty()))
                    {
                        m_samplerecord->item(i,4)->setText(qStrName);
                    }
                    else
                    {
                        m_samplerecord->item(i,4)->setText(qStrID);
                    }
                }
                else
                {
                    QSqlQuery query = CDataBase::select_Record(tr("TABLE_BODY"),tr("SERNUM"),ser);
                    if(query.next())
                    {
                        qStrID = query.value(22).toString();
                    }

                    m_samplerecord->item(i,4)->setText(qStrID);
                }

                CMyButton *pButton = new CMyButton(m_samplerecord);
                pButton->setText(tr("Show"));
                pButton->setStyleSheet(FONTFAMILY + FONT_SIZE_7);
                pButton->setMaximumWidth(60);
                pButton->setMinimumWidth(60);
                pButton->setMaximumHeight(30);
                pButton->setMinimumHeight(30);
                pButton->SetNum(i);

                /*QHBoxLayout *hLayout = new QHBoxLayout(m_samplerecord);
                QFrame *frame = new QFrame(m_samplerecord);
                hLayout->addWidget(pButton);
                hLayout->setMargin(0);
                hLayout->setAlignment(pButton,Qt::AlignCenter);
                frame->setLayout(hLayout);*/

                connect(pButton,SIGNAL(sigbtnclick(int)),this,SLOT(SLOT_TbButtonClick(int)));
                m_samplerecord->setCellWidget(i,5,pButton);
            }
            else
                break;
        }

    }
    else//往上更新
    {
        QSqlQuery query = CDataBase::select_Record(table,"SERNUM",QString::number(first),">=",false);

        for(int i = 0; i < SHOW_LINE; i++)
        {
            if(query.next())
            {
                QString ser = query.value(0).toString();
                QString date = query.value(2).toString().left(10);
                int unite = query.value(3).toString().toInt();
                QString name1 = query.value(4).toString();
                QString unit1 = query.value(5).toString();
                QString rest1 = query.value(6).toString();
                QString name2 = query.value(7).toString();
                QString name3 = query.value(10).toString();
                QString name4 = query.value(13).toString();
                QString name5 = query.value(16).toString();

                m_samplerecord->item(SHOW_LINE-i-1,0)->setText(ser);
                m_samplerecord->item(SHOW_LINE-i-1,1)->setText(date);
                if(unite > 1)
                {
                    if(name1.compare(tr("hsCRP")) == 0 && name2.mid(2,3).compare(tr("CRP")) == 0)
                    {
                        m_samplerecord->item(SHOW_LINE-i-1,2)->setText(tr("CRP"));
                        m_samplerecord->item(SHOW_LINE-i-1,3)->setText(tr("View details"));
                    }
                    else if(name1.compare(tr("LH")) == 0 && name2.compare(tr("FSH")) == 0 && name3.compare(tr("PRL")) == 0)
                    {
                        m_samplerecord->item(SHOW_LINE-i-1,2)->setText(tr("3in1(hormone)"));
                        m_samplerecord->item(SHOW_LINE-i-1,3)->setText(tr("View details"));
                    }
                    else if(name1.compare(tr("cTnI")) == 0 && name2.compare(tr("CK-MB")) == 0 && name3.compare(tr("MYO")) == 0
                            && name4.compare(tr("D-D")) != 0 && name5.compare(tr("NT-ProBNP")) != 0)
                    {
                        m_samplerecord->item(SHOW_LINE-i-1,2)->setText(tr("3in1(cardiac)"));
                        m_samplerecord->item(SHOW_LINE-i-1,3)->setText(tr("View details"));
                    }
                    else if(name1.compare(tr("cTnI")) == 0 && name2.compare(tr("CK-MB")) == 0 && name3.compare(tr("MYO")) == 0
                            && name4.compare(tr("D-D")) == 0 && name5.compare(tr("NT-ProBNP")) == 0)
                    {
                        m_samplerecord->item(SHOW_LINE-i-1,2)->setText(tr("5in1(cardiac)"));
                        m_samplerecord->item(SHOW_LINE-i-1,3)->setText(tr("View details"));
                    }
                    else
                    {
                        m_samplerecord->item(SHOW_LINE-i-1,2)->setText(tr("%1 3 in 1(cTnI/CK-MB/MYO)").arg(unite));
                        m_samplerecord->item(SHOW_LINE-i-1,3)->setText(tr("View details"));
                    }

                }
                else
                {
                    m_samplerecord->item(SHOW_LINE-i-1,2)->setText(name1);
                    m_samplerecord->item(SHOW_LINE-i-1,3)->setText(tr("%1 %2").arg(rest1).arg(unit1));
                }

                QSqlQuery sqlQuery = CDataBase::select_Record(tr("PATIENT_BODY"),tr("SERNUM"),ser);
                QString qStrID;
                QString qStrName;
                if(sqlQuery.first())
                {
                    qStrID = sqlQuery.value(1).toString();
                    qStrName = sqlQuery.value(2).toString();

                    if(!(qStrName.isEmpty()))
                    {
                        m_samplerecord->item(SHOW_LINE-i-1,4)->setText(qStrName);
                    }
                    else
                    {
                        m_samplerecord->item(SHOW_LINE-i-1,4)->setText(qStrID);
                    }
                }
                else
                {
                    QSqlQuery query = CDataBase::select_Record(tr("TABLE_BODY"),tr("SERNUM"),ser);
                    if(query.next())
                    {
                        qStrID = query.value(22).toString();
                    }

                    m_samplerecord->item(SHOW_LINE-i-1,4)->setText(qStrID);
                }

                CMyButton *pButton = new CMyButton(m_samplerecord);
                pButton->setText(tr("Show"));
				pButton->setStyleSheet(FONTFAMILY + FONT_SIZE_7);
                pButton->setMaximumWidth(60);
                pButton->setMinimumWidth(60);
                pButton->setMaximumHeight(30);
                pButton->setMinimumHeight(30);
                pButton->SetNum(SHOW_LINE-i-1);

                /*QHBoxLayout *hLayout = new QHBoxLayout(m_samplerecord);
                QFrame *frame = new QFrame(m_samplerecord);
                hLayout->addWidget(pButton);
                hLayout->setMargin(0);
                hLayout->setAlignment(pButton,Qt::AlignCenter);
                frame->setLayout(hLayout);*/

                connect(pButton,SIGNAL(sigbtnclick(int)),this,SLOT(SLOT_TbButtonClick(int)));
                m_samplerecord->setCellWidget(SHOW_LINE-i-1,5,pButton);
            }
            else
                break;
        }

        if(m_samplerecord->item(0,0)->text().isEmpty())
        {
            int ser=0;
            for(int i = 1; i < SHOW_LINE; i++)
            {
                if(!m_samplerecord->item(i,0)->text().isEmpty())
                {
                    ser  = m_samplerecord->item(i,0)->text().toInt();
                    updatarecord(table,ser,true);
                    break;
                }
            }
        }
    }

    m_samplerecord->selectRow(0);
    slotserchange();
}

void CSampleWidget::slotclicksearch()
{
    this->setEnabled(false);
    m_pSearchWidget->ClearContent();
    m_pSearchWidget->show();
}

void CSampleWidget::slotclickprintf()
{
    if(m_samplerecord->currentRow() < 0)
        return;

    if(!CCommonInterface::get_datacom()->isBuildCommt())
    {
        RES_DATA tmpres;

        int ser = m_samplerecord->item(m_samplerecord->currentRow(),0)->text().toInt();
        if(ser == 0)
            return;

        QSqlQuery query = CDataBase::select_Record(TABLE_BODY_NAME,"SERNUM",ser);
        if(query.next())
        {
            tmpres.swinum = QString::number(ser);
            tmpres.batch = query.value(1).toString();
            tmpres.endtime = query.value(2).toDateTime();
            tmpres.unite = query.value(3).toInt();
            for(int i=0;i<tmpres.unite;i++)
            {
                tmpres.pro[i].name = query.value(4+3*i).toString();
                tmpres.pro[i].result = query.value(6+3*i).toString();
                tmpres.pro[i].unit = query.value(5+3*i).toString();
            }
            tmpres.notes = query.value(22).toString();
        }

        if(CSysInfo::get_cfg(CFG_QT_PRTREF).toBool())
        {
            query = CDataBase::select_Record(BATCH_BODY_NAME,"BATCH",tmpres.batch);
            if(query.next())
            {
                tmpres.code = query.value(2).toByteArray();
                XArgDecode tmpad(tmpres.code);
                int howres = 0;
                for(int i=0;i<tmpad.getunite();i++)
                {
                    tmpres.pro[howres].refer = QString::number(tmpad.getPnInfo(i)->proinfo.refermin)+"-"+\
                            QString::number(tmpad.getPnInfo(i)->proinfo.refermax);
                    howres++;
                    if(tmpad.getPnInfo(i)->proinfo.nextsection != NULL)
                    {
                        tmpres.pro[howres].refer = QString::number(tmpad.getPnInfo(i)->nextproinfo.refermin)+"-"+\
                                QString::number(tmpad.getPnInfo(i)->nextproinfo.refermax);
                        howres++;
                    }
                }
            }
            CCommonInterface::get_printf()->set_bpfrefer(true);
        }
        else
        {
            CCommonInterface::get_printf()->set_bpfrefer(false);
        }

        QSqlQuery sqlQuery = CDataBase::GetPatientInfoByID(m_samplerecord->item(m_samplerecord->currentRow(),4)->text());
        if(sqlQuery.first())
        {
            tmpres.patient.m_bIsSave = true;
            tmpres.patient.m_sID = sqlQuery.value(1).toString();
            tmpres.patient.m_sName = sqlQuery.value(2).toString();
            tmpres.patient.m_sSex = sqlQuery.value(3).toString();
            tmpres.patient.m_sAge = sqlQuery.value(4).toString();
            tmpres.patient.m_sSmpNum = sqlQuery.value(5).toString();
            tmpres.patient.m_sSmpType = sqlQuery.value(6).toString();
        }

        CCommonInterface::get_printf()->Printf_topage(&tmpres);
    }
}

void CSampleWidget::slotclickdele()
{
    if(m_samplerecord->currentRow() < 0)
    {
        return;
    }

    int iRow = m_samplerecord->currentRow();
    int iSer = m_samplerecord->item(iRow,0)->text().trimmed().toInt();

    if(iSer != 0)
    {
        this->setEnabled(false);
        m_pDeleteChoose->show();
    }
    else
    {
        return;
    }
}

void CSampleWidget::DeleteRecord(int iIndex)
{
    if((m_samplerecord->currentRow() < 0) || (m_samplerecord->currentRow() >= SHOW_LINE))
        return;

    this->setEnabled(true);
    if(iIndex == BTDELCURRENT)
    {
        int ser = m_samplerecord->item(m_samplerecord->currentRow(),0)->text().toInt();
        this->setEnabled(false);
        //m_promptbox->settitle(tr("此操作将删除流水号%0数据，且不可恢复！\n\n确认：执行删除    取消：返回").arg(ser));
        m_promptbox->settitle(tr("This operation will delete the lot number \nof %0 and cannot be restored!\n\nConfirm: Delete        Cancel: Back").arg(ser));
        m_promptbox->setnsec(6);//倒计时6秒
        m_promptbox->setwhichbutton(1);
        m_promptbox->setpromptser(0);
        m_promptbox->show();
    }
    else if(iIndex == BTDELALL)
    {
        this->setEnabled(false);
        //m_promptbox->settitle(tr("此操作将清空所有测试数据，且不可恢复！\n\n确认：执行删除    取消：返回"));
        m_promptbox->settitle(tr("This operation will delete all the lot number \nand cannot be restored\n\nConfirm: Delete        Cancel: Back"));
        m_promptbox->setnsec(6);//倒计时6秒
        m_promptbox->setwhichbutton(1);
        m_promptbox->setpromptser(1);
        m_promptbox->show();
    }
    else
    {
        return;
    }
}

void CSampleWidget::slotclickback()
{
    bRefreshData = true;
    emit sigperform(RECD_WG);
}


void CSampleWidget::slotserchange()
{
    if(m_samplerecord->currentRow() < 0)
        return;
    m_nCurrSerNO = m_samplerecord->item(m_samplerecord->currentRow(),0)->text().toInt();
}

void CSampleWidget::slotclickup()
{
    if(m_bSearch)
    {
        if(vecTestData.count() < 10 || m_iPos==0)
        {
            return;
        }
        else
        {
            m_iPos  = m_iPos-10;
            ShowTestData(m_iPos);
        }
    }
    else
    {
        int first = m_samplerecord->item(0,0)->text().toInt()+1;
        int max = CDataBase::get_MaxAdss(m_curtable);

        if(first > max)
            return;

        updatarecord(m_curtable,first,false);
    }
}

void CSampleWidget::slotclickdw()
{
    if(m_bSearch)
    {
        if(vecTestData.count() < 10 || (m_iPos+10)>vecTestData.count())
        {
            return;
        }
        else
        {
            m_iPos  = m_iPos+10;
            ShowTestData(m_iPos);
        }
    }
    else
    {
        int first = m_samplerecord->item(SHOW_LINE-1,0)->text().toInt()-1;
        int min = CDataBase::get_MinAdss(m_curtable);

        if(first < min)
            return;

        updatarecord(m_curtable,first,true);
    }
}

void CSampleWidget::slotclickdel1()
{
    if((m_samplerecord->currentRow() < 0) || (m_samplerecord->currentRow() >= SHOW_LINE))
        return;
    int ser = m_samplerecord->item(m_samplerecord->currentRow(),0)->text().toInt();
    this->setEnabled(false);
    //m_promptbox->settitle(tr("此操作将删除流水号%0数据，且不可恢复！\n\n确认：执行删除    取消：返回").arg(ser));
    m_promptbox->settitle(tr("This operation will delete the lot number \nof %0 and cannot be restored\n\n确认：执行删除    取消：返回").arg(ser));
    m_promptbox->setnsec(6);
    m_promptbox->setwhichbutton(1);
    m_promptbox->setpromptser(0);
    m_promptbox->show();
}

void CSampleWidget::slotclickdeln()
{
    this->setEnabled(false);
    //m_promptbox->settitle(tr("此操作将清空所有测试数据，且不可恢复！\n\n确认：执行删除    取消：返回"));
    m_promptbox->settitle(tr("This operation will delete the test \ndata and cannot be restored!\n\nConfirm：Delete    Cancel：Back"));
    m_promptbox->setnsec(6);
    m_promptbox->setwhichbutton(1);
    m_promptbox->setpromptser(1);
    m_promptbox->show();
}

void CSampleWidget::slotinputclick(QString text)
{
    if((text != "英") && (text != "数"))
    {
        if(m_curlineedit != NULL)
        {
            QString cur = m_curlineedit->text();
            //if(cur.length()>=32) return;
            cur.append(text);
            m_curlineedit->clear();
            m_curlineedit->setText(cur);
        }
    }
    else
    {
        int ne = 0;
        m_input[26]->setText(tr("英"));
        if(m_input[0]->text() == "1")
        {
            ne = 1;
            m_input[26]->setText(tr("数"));
        }
        for(int i=0;i<26;i++)
        {
            m_input[i]->setText(QString(m_btUGC[ne][i]));
        }
    }
}

void CSampleWidget::slotcurlineedit(XLineEdit *id)
{
    QPalette lette;
    lette.setColor(QPalette::Base, QColor(Qt::white));
    if(m_curlineedit != NULL)
    {
        m_curlineedit->setPalette(lette);
    }
    lette.setColor(QPalette::Base, QColor(Qt::gray));
    m_curlineedit = id;
    m_curlineedit->setPalette(lette);
}

void CSampleWidget::slotsearchdel()
{
    QString cur = m_curlineedit->text();
    if(cur.length()>0)
    {
        m_curlineedit->clear();
        m_curlineedit->setText(cur.left(cur.length()-1));
    }
}

void CSampleWidget::slotsearchclr()
{
    m_searchser->clear();
    m_searchbatch->clear();
    m_searchID->clear();
    m_searchpro->clear();
    m_searchyear->clear();
    m_searchmon->clear();
    m_searchday->clear();
}

void CSampleWidget::slotsearchall()
{
#if 0
    qDebug()<<"slotsearchall";
    QSqlQuery query;
    QString search;
    if(!m_searchser->text().isEmpty())
    {
        search.append(QString("SELECT * FROM %0 WHERE %1 LIKE '\%%2\%'")
                                .arg(TABLE_BODY_NAME).arg("SERNUM").arg(m_searchser->text()));
    }
    if(!m_searchbatch->text().isEmpty())
    {
        if(!search.isEmpty()) search.append(" INTERSECT  ");
        search.append(QString("SELECT * FROM %0 WHERE %1 LIKE '\%%2\%'")
                                .arg(TABLE_BODY_NAME).arg("BATCH").arg(m_searchbatch->text()));
    }
    if(!m_searchID->text().isEmpty())
    {
        if(!search.isEmpty()) search.append(" INTERSECT  ");
        search.append(QString("SELECT * FROM %0 WHERE %1 LIKE '\%%2\%'")
                                .arg(TABLE_BODY_NAME).arg("NOTES").arg(m_searchID->text()));

    }
    if(!m_searchpro->text().isEmpty())
    {
        if(!search.isEmpty()) search.append(" INTERSECT  ");
        search.append(QString("SELECT * FROM %0 WHERE %1 LIKE '\%%2\%'")
                                .arg(TABLE_BODY_NAME).arg("NAME1").arg(m_searchpro->text()));
    }

    if(!m_searchyear->text().isEmpty())
    {
        if(m_searchyear->text().toInt() >= 2000)
        {
            if(!search.isEmpty()) search.append(" INTERSECT  ");
            search.append(QString("SELECT * FROM %0 WHERE strftime('%Y',%1)='%2'")
                          .arg(TABLE_BODY_NAME).arg("TDATE").arg(m_searchyear->text()));
        }
    }
    if(!m_searchmon->text().isEmpty())
    {
        if(m_searchmon->text().toInt() != 0)
        {
            QString tmp;
            if(m_searchmon->text().length() < 2) tmp.append("0"+m_searchmon->text());
            else tmp.append(m_searchmon->text());
            if(!search.isEmpty()) search.append(" INTERSECT  ");
            search.append(QString("SELECT * FROM %0 WHERE strftime('%m',%1)='%2'")
                                    .arg(TABLE_BODY_NAME).arg("TDATE").arg(tmp));
        }

    }
    if(!m_searchday->text().isEmpty())
    {
        if(m_searchday->text().toInt() != 0)
        {
            QString tmp;
            if(m_searchday->text().length() < 2) tmp.append("0"+m_searchday->text());
            else tmp.append(m_searchday->text());
            if(!search.isEmpty()) search.append(" INTERSECT  ");
            search.append(QString("SELECT * FROM %0 WHERE strftime('%d',%1)='%2'")
                                    .arg(TABLE_BODY_NAME).arg("TDATE").arg(tmp));
        }
    }
    CDataBase::del_Record(TMPTB_BODY_NAME);
    if(!search.isEmpty())
    {
        qDebug()<<search;
        QString tmp;
        tmp.append(QString("CREATE TABLE %0 AS (").arg(TMPTB_BODY_NAME));
        tmp.append(search+")");
        query.exec(tmp);
        query.exec(QString("SELECT * FROM %0").arg(TMPTB_BODY_NAME));
        while(query.next())
        {
            qDebug()<<"search "<<query.value(0).toInt();
        }
        int curr = CDataBase::get_InsertAdss(TMPTB_BODY_NAME)-1;
        updatarecord(TMPTB_BODY_NAME,curr,true);

    }
#else
//    qDebug()<<"slotsearchall";
    QSqlQuery query;
    QString search;
    search.append(QString("SELECT * FROM %0 WHERE ").arg(TABLE_BODY_NAME));
    if(!m_searchser->text().isEmpty())
    {
        if(search.length() > 40) search.append(" AND ");
        search.append(QString("%0 = '%1'").arg("SERNUM").arg(m_searchser->text()));
    }
    if(!m_searchbatch->text().isEmpty())
    {

        if(search.length() > 40) search.append(" AND ");
        search.append(QString("%0 LIKE '\%%1\%'").arg("BATCH").arg(m_searchbatch->text()));
    }
    if(!m_searchID->text().isEmpty())
    {

        if(search.length() > 40) search.append(" AND ");
        search.append(QString("%0 LIKE '\%%1\%'").arg("NOTES").arg(m_searchID->text()));
    }
    if(!m_searchpro->text().isEmpty())
    {
        if(search.length() > 40) search.append(" AND ");
        search.append(QString("%0 LIKE '\%%1\%'").arg("NAME1").arg(m_searchpro->text()));
    }

    if(!m_searchyear->text().isEmpty())
    {
        if(m_searchyear->text().toInt() >= 2000)
        {
            if(search.length() > 40) search.append(" AND ");
            search.append(QString("strftime('%Y',%0)='%1'").arg("TDATE").arg(m_searchyear->text()));
        }
    }
    if(!m_searchmon->text().isEmpty())
    {
        if(m_searchmon->text().toInt() != 0)
        {
            QString tmp;
            if(m_searchmon->text().length() < 2) tmp.append("0"+m_searchmon->text());
            else tmp.append(m_searchmon->text());

            if(search.length() > 40) search.append(" AND ");
            search.append(QString("strftime('%m',%0)='%1'").arg("TDATE").arg(tmp));
        }
    }
    if(!m_searchday->text().isEmpty())
    {
        if(m_searchday->text().toInt() != 0)
        {
            QString tmp;
            if(m_searchday->text().length() < 2) tmp.append("0"+m_searchday->text());
            else tmp.append(m_searchday->text());

            if(search.length() > 40) search.append(" AND ");
            search.append(QString("strftime('%d',%0)='%1'").arg("TDATE").arg(tmp));
        }
    }

    CDataBase::del_Table(TMPTB_BODY_NAME);

    if(search.length() > 40)
    {
        QString tmp;
        tmp.append(QString("CREATE TABLE %0 AS ").arg(TMPTB_BODY_NAME)+search);
//        qDebug()<<tmp;
//        qDebug()<<"CREATE"<<query.exec(tmp);
        if ( ! query.exec(tmp) )
            qDebug() << query.lastError();

        m_curtable = TMPTB_BODY_NAME;

        m_searchnum->setText(QString::number(CDataBase::get_RecordNum(TMPTB_BODY_NAME)));
    }
    else
    {
        m_curtable = TABLE_BODY_NAME;
        m_searchnum->setText(tr("请正确输入搜索条件"));
    }

    int curr = CDataBase::get_MaxAdss(m_curtable);
    updatarecord(m_curtable,curr,true);
#endif

}

void CSampleWidget::slotpromptok(int ser)
{
    if(ser == 0)
    {
        if((m_samplerecord->currentRow() < 0) || (m_samplerecord->currentRow() >= SHOW_LINE))
            return;
        int num = m_samplerecord->item(m_samplerecord->currentRow(),0)->text().toInt();
        CDataBase::del_Record(TABLE_BODY_NAME,"SERNUM",num);
        CDataBase::del_Record(FLUDT_BODY_NAME,"SERNUM",num);
        CDataBase::del_Record(TMPTB_BODY_NAME,"SERNUM",num);
        CDataBase::del_Record(PATIENT_BODY_NAME,"SERNUM",num);
        int first = m_samplerecord->item(0,0)->text().toInt();

        if(m_samplerecord->currentRow() == 0)
        {
            first = m_samplerecord->item(1,0)->text().toInt();
        }

        updatarecord(TABLE_BODY_NAME,first,true);
        m_samplerecord->setCurrentCell(0,0);
    }
    else if(ser == 1)
    {
        CDataBase::del_Record(TABLE_BODY_NAME);
        CDataBase::del_Record(FLUDT_BODY_NAME);
        CDataBase::del_Record(TMPTB_BODY_NAME);
        CDataBase::del_Record(PATIENT_BODY_NAME);

        ClearTbContent();
    }

    this->setEnabled(true);
}

void CSampleWidget::slotpromptcs(int ser)
{
    this->setEnabled(true);
}

void CSampleWidget::slotChangeID(bool isBatch)
{
    updatarecord(TABLE_BODY_NAME,0,true);
}

void CSampleWidget::ClearTbContent()
{
    m_samplerecord->clearContents();
    for(int i = 0; i < SHOW_LINE; i++)
    {
        m_samplerecord->setItem(i,0,new QTableWidgetItem());
        m_samplerecord->item(i,0)->setText("");
        m_samplerecord->item(i,0)->setTextAlignment(Qt::AlignCenter);
        m_samplerecord->setItem(i,1,new QTableWidgetItem());
        m_samplerecord->item(i,1)->setText("");
        m_samplerecord->item(i,1)->setTextAlignment(Qt::AlignCenter);
        m_samplerecord->setItem(i,2,new QTableWidgetItem());
        m_samplerecord->item(i,2)->setText("");
        m_samplerecord->item(i,2)->setTextAlignment(Qt::AlignCenter);
        m_samplerecord->setItem(i,3,new QTableWidgetItem());
        m_samplerecord->item(i,3)->setText("");
        m_samplerecord->item(i,3)->setTextAlignment(Qt::AlignCenter);
        m_samplerecord->setItem(i,4,new QTableWidgetItem());
        m_samplerecord->item(i,4)->setText("");
        m_samplerecord->item(i,4)->setTextAlignment(Qt::AlignCenter);
        m_samplerecord->setItem(i,5,new QTableWidgetItem());
        m_samplerecord->item(i,5)->setText("");
        m_samplerecord->item(i,5)->setTextAlignment(Qt::AlignCenter);
        m_samplerecord->setRowHeight(i,30);
    }
}

void CSampleWidget::SLOT_TbButtonClick(int iRow)
{
    QString qStrSer = m_samplerecord->item(iRow,0)->text();
    if(qStrSer.isEmpty())
    {
        return;
    }
    emit BtDetailClicked(qStrSer.toInt());
}


void CSampleWidget::RecordQuery(QString qStrKey,int iIndex)
{
    this->setEnabled(true);
    vecTestData.clear();
    if(iIndex == DATE_SEARCH)
    {
        if (qStrKey.length() < 8)
            return;

        QString str = qStrKey.left(4) + '-' + qStrKey.mid(4, 2) + '-' + qStrKey.right(2);
        str = "select max(SERNUM) from TABLE_BODY where TDATE like '" + str + "%'";

        QSqlQuery query;
        qDebug()<<"-----str:"<<str;
        if (query.exec(str))
        {
            if(query.first())
            {
                if(query.value(0).toInt()<=0)
                {
                    this->setEnabled(false);
                    CMessageBox *pMessageBox = new CMessageBox;
                    pMessageBox->SetType(BT_CONFIRM,tr("未搜索到相关样本记录！"));
                    connect(pMessageBox,SIGNAL(SigConfirm()),this,SLOT(EnableWidget()));
                    pMessageBox->show();

                    int iFirst = CDataBase::get_MaxAdss(TABLE_BODY_NAME);
                    updatarecord(TABLE_BODY_NAME,iFirst,true);
                }
                else
                {
                    updatarecord(m_curtable, query.value(0).toInt(), true);
                    m_samplerecord->setCurrentCell(0,0);
                }
            }
            else
            {
                this->setEnabled(false);
                CMessageBox *pMessageBox = new CMessageBox;
                pMessageBox->SetType(BT_CONFIRM,tr("未搜索到相关样本记录！"));
                connect(pMessageBox,SIGNAL(SigConfirm()),this,SLOT(EnableWidget()));
                pMessageBox->show();

                int iFirst = CDataBase::get_MaxAdss(TABLE_BODY_NAME);
                updatarecord(TABLE_BODY_NAME,iFirst,true);
            }
        }
        else
        {
            this->setEnabled(false);
            CMessageBox *pMessageBox = new CMessageBox;
            pMessageBox->SetType(BT_CONFIRM,tr("未搜索到相关样本记录！"));
            connect(pMessageBox,SIGNAL(SigConfirm()),this,SLOT(EnableWidget()));
            pMessageBox->show();

            int iFirst = CDataBase::get_MaxAdss(TABLE_BODY_NAME);
            updatarecord(TABLE_BODY_NAME,iFirst,true);
        }
        m_bSearch = false;
        bRefreshData = false;
    }
    else if(iIndex == ID_SEARCH)
    {
        QSqlQuery query = CDataBase::GetSmpByID(qStrKey);
        while(query.next())
        {
            int iSerNum = query.value(0).toInt();
            QString qStrDate = query.value(1).toString().left(10);
            int iUnite = query.value(2).toInt();
            QString qStrItem = query.value(3).toString();
            QString qStrUnit = query.value(4).toString();
            QString qStrRes = query.value(5).toString();
            QString qStrID = query.value(6).toString();

            CTestData *pTestDate = new CTestData;
            pTestDate->SetSerNum(iSerNum);
            pTestDate->SetDate(qStrDate);
            pTestDate->SetUnite(iUnite);
            pTestDate->SetItem(qStrItem);
            pTestDate->SetUnit(qStrUnit);
            pTestDate->SetResult(qStrRes);
            pTestDate->SetID(qStrID);

            vecTestData.push_back(pTestDate);
        }
        m_bSearch = true;
        bRefreshData = false;
        if(vecTestData.count() == 0)
        {
            this->setEnabled(false);
            CMessageBox *pMessageBox = new CMessageBox;
            pMessageBox->SetType(BT_CONFIRM,tr("未搜索到相关样本记录！"));
            connect(pMessageBox,SIGNAL(SigConfirm()),this,SLOT(EnableWidget()));
            pMessageBox->show();
            updatarecord(TABLE_BODY_NAME,CDataBase::get_MaxAdss(TABLE_BODY_NAME),true);
        }
        else
        {
            ShowTestData(0);
        }
    }
    else if(iIndex == NAME_SEARCH)
    {
        QSqlQuery query = CDataBase::GetSmpByName(qStrKey);
        while(query.next())
        {
            int iSerNum = query.value(0).toInt();
            QString qStrDate = query.value(1).toString().left(10);
            int iUnite = query.value(2).toInt();
            QString qStrItem = query.value(3).toString();
            QString qStrUnit = query.value(4).toString();
            QString qStrRes = query.value(5).toString();
            QString qStrID = query.value(6).toString();
            QString qStrName = query.value(7).toString();

            CTestData *pTestDate = new CTestData;
            pTestDate->SetSerNum(iSerNum);
            pTestDate->SetDate(qStrDate);
            pTestDate->SetUnite(iUnite);
            pTestDate->SetItem(qStrItem);
            pTestDate->SetUnit(qStrUnit);
            pTestDate->SetResult(qStrRes);
            pTestDate->SetID(qStrID);
            pTestDate->SetName(qStrName);

            vecTestData.push_back(pTestDate);
        }
        m_bSearch = true;
        bRefreshData = false;
        if(vecTestData.count() == 0)
        {
            this->setEnabled(false);
            CMessageBox *pMessageBox = new CMessageBox;
            pMessageBox->SetType(BT_CONFIRM,tr("未搜索到相关样本记录！"));
            connect(pMessageBox,SIGNAL(SigConfirm()),this,SLOT(EnableWidget()));
            pMessageBox->show();
            updatarecord(TABLE_BODY_NAME,CDataBase::get_MaxAdss(TABLE_BODY_NAME),true);
        }
        else
        {
            ShowTestData(0);
        }
    }
    else
    {
        updatarecord(TABLE_BODY_NAME,CDataBase::get_MaxAdss(TABLE_BODY_NAME),true);
    }
}

void CSampleWidget::RecordCancel()
{
    this->setEnabled(true);
}

void CSampleWidget::ShowTestData(int iPos)
{
    int iRow = 0;
    ClearTbContent();
    for(int i=iPos;i<vecTestData.count();i++)
    {
        if(iRow > 9)
            break;
        m_samplerecord->item(iRow,0)->setText(QString::number(vecTestData.at(i)->GetSerNum()));
        m_samplerecord->item(iRow,1)->setText(vecTestData.at(i)->GetDate());
        if(vecTestData.at(i)->GetUnite() > 1)
        {
            m_samplerecord->item(iRow,2)->setText(tr("%1联卡").arg(vecTestData.at(i)->GetUnite()));
            m_samplerecord->item(iRow,3)->setText(tr("查看详情"));
        }
        else
        {
            m_samplerecord->item(iRow,2)->setText(vecTestData.at(i)->GetItem());
            m_samplerecord->item(iRow,3)->setText(tr("%1 %2").arg(vecTestData.at(i)->GetResult()).arg(vecTestData.at(i)->GetUnit()));
        }

        if(vecTestData.at(i)->GetName().isEmpty())
        {
            m_samplerecord->item(iRow,4)->setText(vecTestData.at(i)->GetID());
        }
        else
        {
            m_samplerecord->item(iRow,4)->setText(vecTestData.at(i)->GetName());
        }

        CMyButton *pButton = new CMyButton(m_samplerecord);
        pButton->setText(tr("详情"));
        pButton->setMaximumWidth(60);
        pButton->setMinimumWidth(60);
        pButton->setMaximumHeight(30);
        pButton->setMinimumHeight(30);
        pButton->SetNum(i);

        QHBoxLayout *hLayout = new QHBoxLayout(m_samplerecord);
        QFrame *frame = new QFrame(m_samplerecord);
        hLayout->addWidget(pButton);
        hLayout->setMargin(0);
        hLayout->setAlignment(pButton,Qt::AlignCenter);
        frame->setLayout(hLayout);

        connect(pButton,SIGNAL(sigbtnclick(int)),this,SLOT(SLOT_TbButtonClick(int)));
        m_samplerecord->setCellWidget(i,5,pButton);

        iRow++;
    }
    slotserchange();
}

void CSampleWidget::EnableWidget()
{
    this->setEnabled(true);
}


void CSampleWidget::slotClickCurve()
{
    qDebug()<<"Click curve sernum:"<<m_nCurrSerNO;
    QSqlQuery query = CDataBase::select_Record(FLUDT_BODY_NAME, "SERNUM", m_nCurrSerNO);

    if(query.next())
    {
        QByteArray arr(query.value(1).toByteArray().left(DATA_MAX_SIZE * 4));
        emit this->sinSetYgCurveData(arr, SAMP_WG);
        bRefreshData = false;
        emit sigperform(YG_WG);
    }
}



