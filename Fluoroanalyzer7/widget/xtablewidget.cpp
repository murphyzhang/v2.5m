#include "xtablewidget.h"
#include "MainWindow.h"
#include <QStyle>
#include <stdio.h>


XTableWidget::XTableWidget(QWidget *parent): QWidget(parent)
{
    m_bShowBatch = true;    //默认显示批号

    initID();

    this->setGeometry(0,0,740,300);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet(FONTFAMILY + FONT_SIZE_6);

    initcardwidget();//左边已进卡信息
    initprowidget();//测试结果
    initeditwidget();//输入批号
    initnoteswidget();//输入ID

    m_edit = new XButton(this);
    m_edit->setGeometry(340,250,80,40);
    m_edit->setText(tr("Edit"));
    connect(m_edit, SIGNAL(sigbtnclick()), this, SLOT(sloteditclick()));

    m_curve = new XButton(this);
    m_curve->setGeometry(430,250,80,40);
    m_curve->setText(tr("Curve"));
    connect(m_curve, SIGNAL(sigbtnclick()), this, SLOT(slotClickCurve()));

    m_print = new XButton(this);
    m_print->setGeometry(520,250,80,40);
    m_print->setText(tr("Print"));
    connect(m_print, SIGNAL(sigbtnclick()), this, SLOT(slotprintclick()));

    m_clear = new XButton(this);
    m_clear->setGeometry(610,250,80,40);
    m_clear->setText(tr("Clear"));
    connect(m_clear, SIGNAL(sigbtnclick()), this, SLOT(slotclearclick()));

    m_back = new XButton(this);
    m_back->setGeometry(700,250,80,40);
    m_back->setText(tr("Back"));
    connect(m_back, SIGNAL(sigbtnclick()), this, SLOT(slotbackclick()));

    font.setFamily(FONT);    
#if  defined(USE_NEW_FONT)
    font.setPixelSize(18);
#else
    font.setPixelSize(24);
#endif

    m_pPatientInfo = new CPatientInfo();
    m_pPatientInfo->hide();
    connect(m_pPatientInfo,SIGNAL(EditFinish(QString, XTestCard*)),
            this,SLOT(slotAddPatientInfo(QString, XTestCard*)),
            Qt::DirectConnection);

//    timerBtnEdit = new QTimer();
//    connect(timerBtnEdit,SIGNAL(timeout()), this,SLOT(timeoutBtnEdit()));
    /*
    m_scrollarea = new QScrollArea(this);
    m_scrollarea->resize(this->width(),this->rowHeight(0));
    m_scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_plusfun = new QWidget();
    m_plusfun->resize(this->width(),this->rowHeight(0));
   */

    //connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(slotchanged(int,int)));
}

XTableWidget::~XTableWidget()
{
    delete m_pPatientInfo;
}

void XTableWidget::showEvent(QShowEvent *)
{
    m_bShowBatch = CSysInfo::get_cfg(CFG_QT_TWC).toBool();

    if (m_bShowBatch)
       m_cardinfo->setHorizontalHeaderLabels(QStringList()<<tr("No.")<<tr("Serial No.")<<tr("Item")<<tr("Time")<<tr("Name"));
   else
        m_cardinfo->setHorizontalHeaderLabels(QStringList()<<tr("No.")<<tr("Serial No.")<<tr("Item")<<tr("Time")<<tr("Name"));

}

void XTableWidget::initcardwidget()
{
    m_cardinfo = new QTableWidget(this);
    m_cardinfo->setGeometry(0,0,320,290);//setStyleSheet("QDialog#filewnd{border:1px solid rgb(44,60,69);border-radius:5px}");
    m_cardinfo->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    m_cardinfo->setSelectionBehavior(QTableWidget::SelectRows);
    m_cardinfo->setEditTriggers(QTableWidget::NoEditTriggers);
    m_cardinfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_cardinfo->setSelectionMode(QAbstractItemView::SingleSelection);
    //m_resulttab->verticalHeader()->setVisible(false);   //隐藏列表头
    //m_resulttab->verticalHeader()->setMinimumWidth(40);
    m_cardinfo->horizontalHeader()->setFixedHeight(40); //设置表头的高度
    m_cardinfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;};" + FONTFAMILY + FONT_SIZE_6); //设置表头背景色
    m_cardinfo->setStyleSheet("selection-background-color:lightblue;" + FONTFAMILY + FONT_SIZE_6); //设置选中背景色

    m_cardinfo->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    m_cardinfo->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    m_cardinfo->verticalHeader()->setVisible(false);
    m_cardinfo->verticalHeader()->setDefaultSectionSize(40); //设置行距

    m_cardinfo->setColumnCount(5);

    m_cardinfo->setColumnWidth(0, 60);      //序号
    m_cardinfo->setColumnWidth(1, 0);       //流水号
    m_cardinfo->setColumnWidth(2, 90);     //项目名称
    m_cardinfo->setColumnWidth(3, 70);      //时间
    m_cardinfo->setColumnWidth(4, 100);      //姓名

    m_cardinfo->setRowCount(ROW_COUNT_MAX);
    for(int i=0;i<ROW_COUNT_MAX;i++)
    {
        for(int j=0;j<5;j++)
        {
            m_cardinfo->setItem(i,j,new QTableWidgetItem());
            m_cardinfo->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            m_cardinfo->item(i,j)->setFont(font);
            m_cardinfo->item(i,j)->setTextColor(QColor(200,111,30));
        }
    }

    m_cardinfo->selectRow(0);
//    connect(m_cardinfo, SIGNAL(cellClicked(int,int)), this, SLOT(slotcardclick())); // xufan 20180806
    connect(m_cardinfo, SIGNAL(itemSelectionChanged()), this, SLOT(slotcardclick()));
}

void XTableWidget::initprowidget()
{
    m_proinfo = new QTableWidget(this);
    m_proinfo->setGeometry(340,0,440,225);
    m_proinfo->setSelectionBehavior(QTableWidget::SelectRows);
    m_proinfo->setEditTriggers(QTableWidget::NoEditTriggers);
    m_proinfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_proinfo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_proinfo->setSelectionMode(QAbstractItemView::SingleSelection);

    //m_resulttab->verticalHeader()->setVisible(false);   //隐藏列表头
    //m_resulttab->verticalHeader()->setMinimumWidth(40);
    m_proinfo->horizontalHeader()->setFixedHeight(40); //设置表头的高度
    m_proinfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;};" + FONTFAMILY + FONT_SIZE_6); //设置表头背景色 skyblue
    m_proinfo->setStyleSheet("selection-background-color:lightblue;" + FONTFAMILY + FONT_SIZE_6); //设置选中背景色
    m_proinfo->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

    m_proinfo->verticalHeader()->setVisible(false);
    m_proinfo->verticalHeader()->setDefaultSectionSize(30); //设置行距

    m_proinfo->setColumnCount(4);
    m_proinfo->setHorizontalHeaderLabels(QStringList()<<tr("Lot No.")<<tr("Result")<<tr("RV")<<tr("Unit"));

    m_proinfo->setColumnWidth(0,130);
    m_proinfo->setColumnWidth(1,140);
    m_proinfo->setColumnWidth(2,96);
    m_proinfo->setColumnWidth(3,80);

    m_proinfo->setRowCount(6);
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<4;j++)
        {
            m_proinfo->setItem(i,j,new QTableWidgetItem());
            m_proinfo->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            m_proinfo->item(i,j)->setFont(font);
            m_proinfo->item(i,j)->setTextColor(QColor(200,111,30));
			if (j==3){
				m_proinfo->item(i,j)->setStyleSheet(FONTFAMILY + FONT_SIZE_5););
			}
        }
    }
}

void XTableWidget::initeditwidget()
{
    m_editgroup = new QWidget(this);
    m_editgroup->setGeometry(320,0,420,245);

    m_prompt = new XLabel(m_editgroup);
    m_prompt->setGeometry(0,0,420,60);
    m_prompt->setStyleSheet(FONTFAMILY + FONT_SIZE_7);
    //m_prompt->setText(tr("当前测试卡条码无法识别\n直接输入或选择最近批次"));
    m_prompt->setText(tr("The lot No. cannot be identified, please \ninput lot No. directly or select the existing"));

    m_btlb = new XLabel(m_editgroup);
    m_btlb->setGeometry(0,70,100,40);
	m_btlb->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    m_btlb->setText(tr("Lot No.:Y"));

    m_inbatch = new XLineEdit(m_editgroup);
    m_inbatch->setGeometry(100,70,200,40);

    m_ok = new XButton(m_editgroup);
    m_ok->setGeometry(320,70,90,40);
    m_ok->setText(tr("Confirm"));
    connect(m_ok, SIGNAL(sigbtnclick()), this, SLOT(slotokclick()));

    m_batch = new QTableWidget(m_editgroup);
    m_batch->setGeometry(0,120,420,94);
    m_batch->setSelectionBehavior(QTableWidget::SelectItems);
    m_batch->setEditTriggers(QTableWidget::NoEditTriggers);
    m_batch->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_batch->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_batch->setSelectionMode(QAbstractItemView::SingleSelection);
    m_batch->horizontalHeader()->setVisible(false);   //隐藏列表头
    m_batch->setStyleSheet("selection-background-color:lightblue;" + FONTFAMILY + FONT_SIZE_6); //设置选中背景色

    m_batch->verticalHeader()->setVisible(false);
    m_batch->verticalHeader()->setDefaultSectionSize(30); //设置行距

    m_batch->setColumnCount(3);

    m_batch->setColumnWidth(0,138);
    m_batch->setColumnWidth(1,138);
    m_batch->setColumnWidth(2,138);

    m_batch->setRowCount(3);
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            m_batch->setItem(i,j,new QTableWidgetItem());
            m_batch->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            m_batch->item(i,j)->setFont(font);
            m_batch->item(i,j)->setTextColor(QColor(200,111,30));
		 
        }
    }
    connect(m_batch, SIGNAL(cellClicked(int,int)), this, SLOT(slotbatchclick(int,int)));
    m_editgroup->hide();
}

void XTableWidget::initnoteswidget()
{
    char ugc1[27]={'1','2','3','0','#','(',')','<','>',
                   '4','5','6','-','&','[',']','/','\\',
                   '7','8','9','+','*','{','}','.'};
    char ugc2[27]={'A','B','C','D','E','F','G','H','I',
                   'J','K','L','M','N','O','P','Q','R',
                   'S','T','U','V','W','X','Y','Z'};

    for(int i=0;i<26;i++)
    {
        m_btUGC[0][i] = ugc1[i];
        m_btUGC[1][i] = ugc2[i];
    }

    m_notesgroup = new QWidget(this);
    m_notesgroup->setGeometry(310,0,420,245);

    m_notesprompt = new XLabel(m_notesgroup);
    m_notesprompt->setGeometry(10,0,400,30);
    m_notesprompt->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    //m_notesprompt->setText(tr("备注信息最多输入32个字符"));
    m_notesprompt->setText(tr("Input note information with 32 characters at most"));

    m_notesUGC = new QTextEdit(m_notesgroup);
    m_notesUGC->setGeometry(10,30,340,80);
    m_notesUGC->setStyleSheet("color: rgb(255, 0, 255);" + FONTFAMILY + FONT_SIZE_6);
    m_notesUGC->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_notesUGC->moveCursor(QTextCursor::End);

    m_notesdel = new XButton(m_notesgroup);
    m_notesdel->setGeometry(350,30,60,38);
    m_notesdel->setText(tr("DEL"));
    connect(m_notesdel, SIGNAL(sigbtnclick()), this, SLOT(slotnotesdel()));

    m_notesok = new XButton(m_notesgroup);
    m_notesok->setGeometry(350,70,60,38);
    m_notesok->setText(tr("OK"));
    connect(m_notesok, SIGNAL(sigbtnclick()), this, SLOT(slotnotesok()));

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<9;j++)
        {
            m_input[i*9+j] = new XButton(m_notesgroup);
            m_input[i*9+j]->setGeometry(10+45*j,110+45*i,40,40);
            m_input[i*9+j]->setText(QString(m_btUGC[0][i*9+j]) );
            connect(m_input[i*9+j], SIGNAL(sigbtnclick(QString)), this, SLOT(slotinputclick(QString)));
        }
    }
    m_input[26]->setText(tr("英"));

    m_notesgroup->hide();

    connect(CI::get_qrscan(), SIGNAL(sigScanInput(QString)),this, SLOT(slotScanInput(QString)));//外部条码扫描完成
}

void XTableWidget::xinsert_row(XTestCard *card)
{
    if(m_cardlist.count() >= ROW_COUNT_MAX)
    {
        delete m_cardlist[0];
        m_cardlist[0] = NULL;
        m_cardlist.removeFirst();
    }
    m_cardlist.append(card);
    xupdate_tab();
    xsetcur_row(card);
}

void XTableWidget::xsetcur_row(XTestCard *card)
{
    if(card != NULL)
    {
        for(int row=0; row<m_cardlist.count(); row++)
        {
            if(m_cardlist.at(row) == card)
            {
                m_cardinfo->setCurrentCell(row, 0);
                slotcardclick();
                break;
            }
        }
    }
}
XTestCard * XTableWidget::xgetcur_row()
{
    int currrow = m_cardinfo->currentRow();
    if((currrow>=0) && (currrow<m_cardlist.count()))
    {
        return m_cardlist.at(currrow);
    }
    else
    {
        return NULL;
    }
}

void XTableWidget::xupdate_tab()
{
    for(int row=0;row<m_cardinfo->rowCount();row++)
    {
        m_cardinfo->item(row,0)->setText("");
        m_cardinfo->item(row,1)->setText("");
        m_cardinfo->item(row,2)->setText("");
        m_cardinfo->item(row,3)->setText("");
//        m_cardinfo->item(row,4)->setText("");
    }

    for(int row = 0; row < m_cardlist.count(); row++)
    {
        m_cardinfo->item(row,0)->setText(QString::number(m_cardlist.at(row)->getsernum()));
        m_cardinfo->item(row,1)->setText(m_cardlist.at(row)->getresdata()->swinum);
        m_cardinfo->item(row,2)->setText("---");
        if(m_cardlist.at(row)->hasbatch())
        {
            m_cardinfo->item(row,2)->setText(m_cardlist.at(row)->m_res.pro[0].name);

            if(!m_cardlist.at(row)->gettestend())
            {
                if(m_cardlist.at(row)->getneedtest())
                {
                    if(m_cardlist.at(row)->hascode())
                    {
                        m_cardinfo->item(row,3)->setText(QTime().addSecs(m_cardlist.at(row)->getsurptime()).toString("mm:ss"));
                    }
                    else
                    {
                        m_cardinfo->item(row,3)->setText(QTime().addSecs(m_cardlist.at(row)->getawaytime()).toString("mm:ss"));
                    }
                }
                else
                {
                    m_cardinfo->item(row,3)->setText(tr("Error"));
                }
            }
            else
            {
                m_cardinfo->item(row,3)->setText(tr("Done"));
            }
        }
        if(m_cardinfo->item(row,4)->text().isEmpty())
        {
            m_cardinfo->item(row,4)->setText("---");
        }
    }
}
void XTableWidget::showproinfo()
{
    int currrow = m_cardinfo->currentRow();
    m_proinfo->show();
    m_print->show();
    //m_edit->show();
    m_clear->show();
    qDebug()<<"----------currrow:"<<currrow;
    m_editgroup->hide();
    m_notesgroup->hide();
    if((currrow>=0) && (currrow<m_cardlist.count()))
    {
        XTestCard * cardInfo = m_cardlist.at(currrow);

//        qDebug() << "curr row recordID =" << cardInfo->getsavepos();

        //需要测试 有条码
        if(cardInfo->getneedtest()&&cardInfo->hascode())
        {
            cardInfo->truntoresdata();
            RES_DATA *pres = cardInfo->getresdata();
            int procount = pres->unite;

            if((cardInfo->getargdecode()->getcardtype() == 0)||
               (cardInfo->getargdecode()->getcardtype() == 2))
            {
                setproline(procount);
                for(int i=0;i<procount;i++)
                {
                    m_proinfo->item(i,0)->setText(cardInfo->getbatch());
                    if(cardInfo->gettestend())
                        m_proinfo->item(i,1)->setText(pres->pro[i].result);
                    else
                        m_proinfo->item(i,1)->setText("");
                    m_proinfo->item(i,2)->setText(pres->pro[i].refer);
                    m_proinfo->item(i,3)->setText(pres->pro[i].unit);
/*
                    //ID自增
                    if(!(cardInfo->m_bIsSave) && !(cardInfo->m_bIsAutoID))
                    {
                        qDebug()<<"----------Auto ID----------";
                        cardInfo->SetAutoID(true);

                        QString qStrNowDate = CCommonInterface::GetNowDateString();
                        QSqlQuery sqlQuery = CDataBase::GetPatientID(qStrNowDate);
                        QSqlQuery sqlQueryTb = CDataBase::GetPatientFromTb(qStrNowDate);

                        int iID = 0;
                        QList<int> iList;
                        while(sqlQuery.next())
                        {
                            iList<<sqlQuery.value(0).toInt();
                        }

                        if(iList.count() == 0)
                        {
                            iID = 0;
                        }
                        else
                        {
                            for(int i=0;i<iList.count();i++)
                            {
                                if(iList.at(i)>iID)
                                {
                                    iID = iList.at(i);
                                }
                            }
                        }
                        qDebug()<<"GetPatientID from TablePatient.   ID:"<<iID;
                        int iIdTb = 0;
                        QList<int> iListTb;
                        while(sqlQueryTb.next())
                        {
                            iListTb<<sqlQueryTb.value(0).toInt();
                        }

                        if(iListTb.count() == 0)
                        {
                            iIdTb = 0;
                        }
                        else
                        {
                            for(int i=0;i<iListTb.count();i++)
                            {
                                if(iListTb.at(i)>iIdTb)
                                {
                                    iIdTb = iListTb.at(i);
                                }
                            }
                        }

                        if((iID == 0) && (iIdTb == 0))
                        {
                            m_strId = qStrNowDate+tr("001");

                            QString qStrPatientID;
                            for(int i=0;i<m_cardlist.count();i++)
                            {
                                qStrPatientID = m_cardlist.at(i)->GetPatientID();
                                if(qStrPatientID.left(6).compare(qStrNowDate) == 0)
                                {
                                    m_strId = QString::number(qStrPatientID.toInt() + 1);
                                }
                            }
                        }
                        else if(iID < iIdTb)
                        {
                            m_strId = QString::number(iIdTb+1);
                        }
                        else if(iID > iIdTb)
                        {
                            m_strId = QString::number(iID+1);
                        }
                        else if(iID == iIdTb)
                        {
                            m_strId = QString::number(iID+1);
                        }
                        else
                        {
                            qDebug()<<"---Error---";
                        }

                        qDebug()<<"GetPatientID from TableBody.   ID:"<<iID;

                        cardInfo->m_sID = m_strId;
                        cardInfo->setnotes(m_strId);
                        emit sigUpdateID(m_strId);
                    }
*/
                    if (cardInfo->getAutoID() == false)
                    {
                        cardInfo->SetAutoID(true);

                        char cstrID[16] = {0};
                        sprintf(cstrID, "%03d", m_ID++);
                        m_strId.clear();
                        m_strId = CI::GetNowDateString();
                        m_strId.append(cstrID);

                        cardInfo->id = m_strId;

                        cardInfo->m_sID = m_strId;
                        cardInfo->setnotes(m_strId);
                        emit sigUpdateID(m_strId);
                    }
                }
            }
            else if(cardInfo->getargdecode()->getcardtype() == 1)
            {
                setproline(1);
                m_proinfo->item(0,0)->setText(tr("Standard cartridge"));
                if(m_cardlist.at(currrow)->gettestend())
                {
                    bool bok = true;
                    for(int i=0;i<procount;i++)
                    {
                        double tt = pres->pro[i].result.toDouble();
                        double up = cardInfo->getargdecode()->getPnInfo(i)->thup;
                        double dw = cardInfo->getargdecode()->getPnInfo(i)->thdw;

                        if((tt>up) || (tt<dw))
                        {
                            bok = false;
                            //break;
                        }
                    }
                    if(bok)
                        m_proinfo->item(0,1)->setText(tr("Calibrate successfully"));
                    else
                        m_proinfo->item(0,1)->setText(tr("Calibrate failure"));
                }


                m_proinfo->item(0,2)->setText("");
                m_proinfo->item(0,3)->setText("");
            }

        }
    }
}

void XTableWidget::slotcardclick()
{
    QFont font = m_cardinfo->horizontalHeader()->font();
    font.setBold(true);
    m_cardinfo->horizontalHeader()->setFont(font);

    int currrow = m_cardinfo->currentRow();
    setproline(0);

    if((currrow >= 0) && (currrow < m_cardlist.count()))
    {
        XTestCard * cardInfo = m_cardlist.at(currrow);

        //还没测试 没扫描到 没有批号
        if(!cardInfo->gettestend()&&!cardInfo->canscan()&&!cardInfo->hasbatch())
        {
            //m_prompt->setText(tr("当前测试卡批号无法识别\n直接输入或选择最近批号"));
            m_prompt->setText(tr("The lot No. cannot be identified, please input\nlot No. directly or select the existing lot No."));
            showeditbatch();
        }
        else if(!cardInfo->gettestend()&&!cardInfo->canscan()&&!cardInfo->hascode())
        {
            //m_prompt->setText(tr("无当前批号信息，请插入芯片\n或更改其他批号测试"));
            m_prompt->setText(tr("No lot number, please insert the SD card or use other lot number for testing."));
            showeditbatch();
        }
        else
        {
            showproinfo();
        }
        emit sigclickrow(cardInfo);
    }
    else
    {
        showproinfo();
        emit sigclickrow(NULL);
    }
}

void XTableWidget::slotbatchclick(int row, int column)
{
    QString batch = m_batch->item(row,column)->text().remove('Y');
    m_inbatch->setText(batch);
}

void XTableWidget::xclear()
{
    for(int row=0;row<m_cardlist.count();row++)
    {
        delete m_cardlist[row];
        m_cardlist[row] = NULL;
    }

    m_cardlist.clear();

    for(int row=0;row<m_cardinfo->rowCount();row++)
    {
        m_cardinfo->item(row,0)->setText("");
        m_cardinfo->item(row,1)->setText("");
        m_cardinfo->item(row,2)->setText("");
        m_cardinfo->item(row,3)->setText("");
        m_cardinfo->item(row,4)->setText("");
    }
    setproline(0);
    showproinfo();
}

void XTableWidget::setproline(int line)
{
    for(int row=0; row<m_proinfo->rowCount() && row < 6;row++)
    {
        m_proinfo->item(row,0)->setText("");
        m_proinfo->item(row,1)->setText("");
        m_proinfo->item(row,2)->setText("");
        m_proinfo->item(row,3)->setText("");
    }

    for(int i=0;i<line;i++)
        m_proinfo->setRowHidden(i,false);

    for(int i=line;i<6;i++)
        m_proinfo->setRowHidden(i,true);

    m_proinfo->setGeometry(340,0,440,225-30*(6-line));
}


void XTableWidget::slotprintclick()
{
    int currrow = m_cardinfo->currentRow();
    if((currrow>=0) && (currrow<m_cardlist.count()))
    {
        //测试完成
        if(m_cardlist.at(currrow)->gettestend())
        {
            if(CSysInfo::get_cfg(CFG_QT_PRTREF).toBool())
                CCommonInterface::get_printf()->set_bpfrefer(true);
            else
                CCommonInterface::get_printf()->set_bpfrefer(false);
            CCommonInterface::get_printf()->Printf_topage(m_cardlist.at(currrow)->getresdata());
        }
    }
}

void XTableWidget::sloteditclick()
{
    int iRow = m_cardinfo->currentRow();

    if(m_cardinfo->item(iRow,0)->text().isEmpty())
        return;

    QString qStrSerNum = m_cardinfo->item(iRow,1)->text();
    qDebug() << "qStrSerNum =" << qStrSerNum;

    bool isPatientExist = CDataBase::IsPatientExist(qStrSerNum);
    if(isPatientExist)
    {
        QSqlQuery query = CDataBase::GetPatientInfo(qStrSerNum);
        while(query.next())
        {
            QString qStrID = query.value(1).toString();
            QString qStrName = query.value(2).toString();
            QString qStrSex = query.value(3).toString();
            QString qStrAge = query.value(4).toString();
            QString qStrSmpNum = query.value(5).toString();
            QString qStrSmpType = query.value(6).toString();

            qDebug() << "qStrName =" << qStrName;

            m_pPatientInfo->SetSerNum(m_cardinfo->item(iRow,1)->text().toInt());
            m_pPatientInfo->ShowData(qStrID,qStrName,qStrSex,qStrAge,qStrSmpNum,qStrSmpType);
        }
    }
    else
    {
        m_pPatientInfo->SetSerNum(m_cardinfo->item(iRow,1)->text().toInt());
        m_pPatientInfo->ShowData(m_cardlist.at(iRow)->m_sID,
                                 m_cardlist.at(iRow)->m_sName,
                                 m_cardlist.at(iRow)->m_sSex,
                                 m_cardlist.at(iRow)->m_sAge,
                                 m_cardlist.at(iRow)->m_sSmpNum,
                                 m_cardlist.at(iRow)->m_sSmpType);

        qDebug() << "m_cardlist.at(" << iRow << ")->m_sName =" << m_cardlist.at(iRow)->m_sName;

        if(!m_cardlist.at(iRow)->m_bIsSave)
        {
            m_pPatientInfo->SetAutoID(m_cardlist.at(iRow)->m_sID);
        }
    }

    m_pPatientInfo->setCurrEditCtrl(m_cardlist.at(iRow), m_cardinfo->item(iRow, 4));
    m_pPatientInfo->show();
//    m_edit->setEnabled(false);

    /*
    if(!m_proinfo->isHidden())
    {
        int currrow = m_cardinfo->currentRow();
        if((currrow>=0) && (currrow<m_cardlist.count()))
        {
#if 0
            //还没测试 没扫描到 没有批号
            if(!m_cardlist.at(currrow)->gettestend()&&!m_cardlist.at(currrow)->canscan())
            {
                if(!m_cardlist.at(currrow)->hasbatch() || !m_cardlist.at(currrow)->hascode())
                {
                    m_prompt->setText(tr("更改批次将影响测试结果\n请谨慎操作，按返回退出"));
                    showeditbatch();
                }
            }
#endif
            m_notesUGC->clear();
            m_notesUGC->setText(m_cardlist.at(currrow)->getnotes());

            //emit sigsetnote();

            m_print->hide();
            m_edit->hide();
            m_clear->hide();

            m_proinfo->hide();
            m_notesgroup->show();
        }
    }*/
}

void XTableWidget::slotclearclick()
{
    emit sigclickclear();
}

void XTableWidget::slotbackclick()
{
    if(m_proinfo->isHidden())
    {
        m_proinfo->show();
        m_print->show();
        //m_edit->show();
        m_clear->show();

        m_editgroup->hide();
        m_notesgroup->hide();
    }
    else
    {
        emit sigclickback();
    }
}

void XTableWidget::showeditbatch()
{
    int max = CDataBase::get_MaxAdss(TABLE_BODY_NAME);

    QSqlQuery query = CDataBase::select_Record(TABLE_BODY_NAME,"SERNUM",QString::number(max),"<=",true);

    if(query.next())
    {
        int row=0,column=0;
        for(column=0;column<3;column++)
        {
            for(row=0;row<3;row++)
            {
                m_batch->item(row,column)->setText("");
            }
        }
        row=0;column=0;
        do{
            QString batch = query.value(1).toString();

            if(m_batch->findItems(batch,Qt::MatchExactly).count() == 0)
            {
                m_batch->item(row,column)->setText(batch);
                if(++column >= 3)
                {
                    row++;
                    column = 0;
                }
            }
            if(row >= 3)
                break;
        }while(query.next());
    }
    m_inbatch->setText(m_batch->item(0,0)->text().remove('Y'));

    m_proinfo->hide();
    m_notesgroup->hide();
    m_editgroup->show();
}

void XTableWidget::shownotesedit()
{
    m_proinfo->hide();
    m_notesgroup->show();
}

void XTableWidget::slotokclick()
{
    int currrow = m_cardinfo->currentRow();
    if((currrow>=0) && (currrow<m_cardlist.count()))
    {
        if(m_inbatch->text().length() == 7)
            emit sigsetbatch(m_cardlist.at(currrow),"Y"+m_inbatch->text());
        else
            //m_prompt->setText(tr("批号有误，请重新输入\n直接输入或选择最近批次"));
            m_prompt->setText(tr("The lot No. is incorrect.Please input the\nlot No. directly or select the existing lot No."));
    }
}

void XTableWidget::slotinputclick(QString text)
{
    if((text != "英") && (text != "数"))
    {
        QString cur = m_notesUGC->toPlainText();
        if(cur.length()>=32)
            return;
        cur.append(text);
        m_notesUGC->clear();
        m_notesUGC->setText(cur);
        m_notesUGC->moveCursor(QTextCursor::End);
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

void XTableWidget::slotnotesdel()
{
    /*
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
    QApplication::sendEvent(m_notesUGC, &keyPress);
    */
    QString cur = m_notesUGC->toPlainText();
    if(cur.length()>0)
    {
        m_notesUGC->clear();
        m_notesUGC->setText(cur.left(cur.length()-1));
    }
}

bool XTableWidget::setIdToCardList(QString qStrID, QString qStrName, QString qStrSex, QString qStrAge, QString qStrSmpNum, QString qStrSmpType)
{
    int currrow = m_cardinfo->currentRow();

    if(currrow < 0 && m_cardlist.count() >= currrow)
        return false;

    //----------------------------保存ID号---------------------------
    m_cardlist.at(currrow)->setnotes(qStrID, qStrName, qStrSex, qStrAge, qStrSmpNum, qStrSmpType);

    if(m_cardlist.at(currrow)->gettestend())
    {
        int dataId = m_cardlist.at(currrow)->getsavepos();
        QSqlQuery query = CDataBase::select_Record(TABLE_BODY_NAME,"SERNUM",m_cardlist.at(currrow)->getsavepos());

        if(query.next() && m_strId != query.value(23).toString())
        {
            QDateTime cur = QDateTime::currentDateTime();
            CDataBase::updata_Record(TABLE_BODY_NAME,"NOTES","SERNUM", m_strId, dataId);
            CDataBase::updata_Record(TABLE_BODY_NAME,"UPDATED","SERNUM", cur, dataId);
            m_cardlist.at(currrow)->setupdatedtime(cur);
        }
    }

    return true;
}

void XTableWidget::updateID()
{
    /*
    if(setIdToCardList())
    {
        int currrow = m_cardinfo->currentRow();

        emit sigclickrow(m_cardlist.at(currrow));//更新信息面板

        m_notesUGC->clear();
        m_notesUGC->setText(m_strId);

        if (!m_bShowBatch)
            m_cardinfo->item(currrow, 2)->setText(m_strId);

        m_proinfo->show();
        m_print->show();
        m_edit->show();
        m_clear->show();

        m_notesgroup->hide();
    }
    */
}

void XTableWidget::slotnotesok()
{
    if (m_notesUGC->toPlainText().length() > 0)
    {
        m_strId = m_notesUGC->toPlainText();
        updateID();
    }
}

extern MainWindow * g_pMainWindow;

void XTableWidget::slotScanInput(QString strId)
{
    if (g_pMainWindow->getCurrWinIndex() != DETET_WG)
        return;

#if 0
    int currrow = m_cardinfo->currentRow();

    if((currrow >= 0) && (currrow < m_cardlist.count()))
    {
        //还没测试 没扫描到 没有批号
        if(!m_cardlist.at(currrow)->gettestend()&&!m_cardlist.at(currrow)->canscan()&&!m_cardlist.at(currrow)->hasbatch())
        {
            return;
        }
        else if(!m_cardlist.at(currrow)->gettestend()&&!m_cardlist.at(currrow)->canscan()&&!m_cardlist.at(currrow)->hascode())//无SD卡
        {
            return;
        }
        else
        {
            m_notesUGC->clear();
            m_notesUGC->setText(strId.left(64));
            slotnotesok();
        }
    }
#else
    if (m_proinfo->isVisible()) //测试结果信息表有显示
    {
        m_strId = strId.left(64);
        updateID();
    }
#endif
}

void XTableWidget::refresh()
{
    xupdate_tab();
    slotcardclick();
}

void XTableWidget::slotAddPatientInfo(QString qStrID, XTestCard * card)
{
    m_strId = qStrID;
#if 0
    int currrow = 0;
    XTestCard * current = NULL;
    QList<XTestCard *>::Iterator iter = m_cardlist.begin();

    for (; iter != m_cardlist.end(); ++iter, ++currrow) {
        if ((*iter)->getsavepos() == serNum) {
            current = *iter;
            break;
        }
    }

    if (current == NULL) {
        qDebug() << "not found" << serNum << "in m_cardlist !";
        current = m_cardlist.at(m_cardinfo->currentRow());

        if (current == NULL)
            return;
    }

    current->setnotes(qStrID, qStrName, qStrSex, qStrAge, qStrSmpNum, qStrSmpType);

    if(current->gettestend())
    {
        int dataId = current->getsavepos();
        QSqlQuery query = CDataBase::select_Record(TABLE_BODY_NAME,"SERNUM",current->getsavepos());

        if(query.next() && m_strId != query.value(23).toString())
        {
            QDateTime cur = QDateTime::currentDateTime();
            CDataBase::updata_Record(TABLE_BODY_NAME,"NOTES","SERNUM", m_strId, dataId);
            CDataBase::updata_Record(TABLE_BODY_NAME,"UPDATED","SERNUM", cur, dataId);
            current->setupdatedtime(cur);
        }
    }

    emit sigclickrow(current);//更新信息面板
    m_cardinfo->item(currrow,4)->setText(qStrName);

    m_notesUGC->clear();
    m_notesUGC->setText(m_strId);

    //        if (!m_bShowBatch)
    //            m_cardinfo->item(currrow, 2)->setText(m_strId);

    m_proinfo->show();
    m_print->show();
    //m_edit->show();
    m_clear->show();

    m_notesgroup->hide();
#else
    if (card)
        emit sigclickrow(card);//更新信息面板

    m_notesUGC->clear();
    m_notesUGC->setText(card->id);

    m_proinfo->show();
    m_print->show();
    m_clear->show();

    m_notesgroup->hide();
//    timerBtnEdit->start(3000);
#endif
}

void XTableWidget::slotClickCurve()
{
    int iRow = m_cardinfo->currentRow();
    QString qStrSerNum = m_cardinfo->item(iRow,1)->text();

    if(qStrSerNum.isEmpty())
        return;

    QSqlQuery query = CDataBase::select_Record(FLUDT_BODY_NAME,"SERNUM",qStrSerNum);

    if(query.next())
    {
        QByteArray arr(query.value(1).toByteArray().left(DATA_MAX_SIZE * 4));
        emit this->sinSetYgCurveData(arr, DETET_WG);
    }
}

void XTableWidget::initID()
{
    QString qStrNowDate = CCommonInterface::GetNowDateString();
    int patientID = CDataBase::GetIdFromPatient(qStrNowDate);
    int recordID = CDataBase::GetIdFromTb(qStrNowDate); // 获取的是自定义ID

    qDebug()<<"initID patientID =" << patientID << ", recordID =" << recordID;

    if((patientID == 0) && (recordID == 0))
    {
        m_ID = 1;
    }
    else
    {
        patientID = QString::number(patientID).right(3).toInt();
        recordID = QString::number(recordID).right(3).toInt();
        m_ID = (patientID > recordID) ? patientID : recordID;
        m_ID++;
    }

    qDebug() << "initID" << m_ID;
}

//void XTableWidget::timeoutBtnEdit()
//{
//    timerBtnEdit->stop();
//    m_edit->setEnabled(true);
//}

//void XTableWidget::delayEanbleBtnEdit()
//{
//    timerBtnEdit->start(5000);
//}

/*
    QString qStrNowDate = CCommonInterface::GetNowDateString();
    QSqlQuery sqlQuery = CDataBase::GetPatientID(qStrNowDate);

    QList<int> iList;
    while(sqlQuery.next())
    {
        iList<<sqlQuery.value(0).toInt();
    }

    int iID = 0;
    if(iList.count() == 0)
    {
        m_pLeID->setText(qStrNowDate+tr("001"));
    }
    else
    {
        for(int i=0;i<iList.count();i++)
        {
            if(iList.at(i)>iID)
            {
                iID = iList.at(i);
            }
        }
        m_pLeID->setText(QString::number(iID+1));
    }
*/

/*
QList<int> iList;
while(sqlQuery.next())
{
    iList<<sqlQuery.value(0).toInt();
}

int iID = 0;
if(iList.count() == 0)
{
    m_strId = qStrNowDate+tr("001");
    m_cardlist.at(currrow)->setnotes(m_strId);
    emit sigUpdateID(m_strId);
}
else
{
    for(int i=0;i<iList.count();i++)
    {
        if(iList.at(i)>iID)
        {
            iID = iList.at(i);
        }
    }
    m_strId = QString::number(iID+1);
    m_cardlist.at(currrow)->setnotes(m_strId);
    emit sigUpdateID(m_strId);
}

*/







