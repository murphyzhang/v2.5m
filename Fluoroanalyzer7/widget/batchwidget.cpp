#include "batchwidget.h"
#include "common/configurate.h"


#define SHOW_LINE 11


CBatchWidget::CBatchWidget(QWidget *parent): CBaseFunWidget(parent)
{
    m_spacename = new XLabel(this);
    m_spacename->setText(tr("Data"));
    m_spacename->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    m_spacename->setGeometry(30, 50, 80, 30);

    m_spacevaule = new XLabel(this);
    m_spacevaule->setText(QString("0/")+QString::number(CDataBase::get_RecordMax(BATCH_BODY_NAME)));
    m_spacevaule->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    m_spacevaule->setGeometry(100, 50, 220, 30);

    m_batchrecord = new QTableWidget(this);
    m_batchrecord->setStyleSheet("color:rgb(0, 0, 0);" + FONTFAMILY + FONT_SIZE_6);
    m_batchrecord->setGeometry(20,80, 650, 340);
    m_batchrecord->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_batchrecord->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_batchrecord->setSelectionBehavior(QTableWidget::SelectRows);
    m_batchrecord->setEditTriggers(QTableWidget::NoEditTriggers);
    m_batchrecord->setSelectionMode(QAbstractItemView::SingleSelection);
    m_batchrecord->verticalHeader()->setVisible(false);   //隐藏列表头
    //m_batchrecord->horizontalHeader()->setVisible(false);
    m_batchrecord->horizontalHeader()->setMinimumHeight(45);
    m_batchrecord->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    m_batchrecord->setColumnCount(4);
    m_batchrecord->setColumnWidth(0,80);
    m_batchrecord->setColumnWidth(1,140);
    m_batchrecord->setColumnWidth(2,80);
    m_batchrecord->setColumnWidth(3,350);
    QStringList headers;
    headers << QObject::tr("No.") << QObject::tr("Lot") << QObject::tr("Time") << QObject::tr("Item");
    m_batchrecord->setHorizontalHeaderLabels(headers);

    m_batchrecord->setRowCount(SHOW_LINE);
    for(int row=0;row<SHOW_LINE;row++)
    {
        m_batchrecord->setItem(row,0,new QTableWidgetItem());
        m_batchrecord->item(row,0)->setText("");
        m_batchrecord->setItem(row,1,new QTableWidgetItem());
        m_batchrecord->item(row,1)->setText("");
        m_batchrecord->setItem(row,2,new QTableWidgetItem());
        m_batchrecord->item(row,2)->setText("");
        m_batchrecord->setItem(row,3,new QTableWidgetItem());
        m_batchrecord->item(row,3)->setText("");
        m_batchrecord->setRowHeight(row,27);
    }


    m_pageup = new XButton(this);
    m_pageup->setGeometry(680,90,100,50);
    m_pageup->setText(tr("▲"));
    connect(m_pageup, SIGNAL(sigbtnclick()), this, SLOT(slotclickpageup()));

    m_pagedown = new XButton(this);
    m_pagedown->setGeometry(680,155,100,50);
    m_pagedown->setText(tr("▼"));
    connect(m_pagedown, SIGNAL(sigbtnclick()), this, SLOT(slotclickpagedown()));

    m_search = new XButton(this);
    m_search->setGeometry(680,220,100,50);
    m_search->setText(tr("Search"));
    connect(m_search, SIGNAL(sigbtnclick()), this, SLOT(slotclicksearch()));

    m_del1 = new XButton(this);
    m_del1->setGeometry(680,285,100,50);
    m_del1->setText(tr("Delete"));
    connect(m_del1, SIGNAL(sigbtnclick()), this, SLOT(slotclickdel1()));
/*
    m_deln = new XButton(this);
    m_deln->setGeometry(680,220,100,50);
    m_deln->setText("清空");
    connect(m_deln, SIGNAL(sigbtnclick()), this, SLOT(slotclickdeln()));
*/
    m_back = new XButton(this);
    m_back->setGeometry(680,350,100,50);
    m_back->setText(tr("Back"));
    connect(m_back, SIGNAL(sigbtnclick()), this, SLOT(slotclickback()));


    connect(CCommonInterface::get_datacom(), SIGNAL(SigNewBatch(QString)), this, SLOT(slotrecvnewbatch(QString)));
    connect(CCommonInterface::get_opteep(), SIGNAL(SigNewBatch(QString)), this, SLOT(slotrecvnewbatch(QString)));



    m_searchlabel = new XLabel(this);
    m_searchlabel->setText(tr("Lot No.: Y"));
    m_searchlabel->setGeometry(140, 200, 150, 50);
    m_searchlabel->hide();

    m_searchedit = new XLineEdit(this);
    m_searchedit->setGeometry(300,200,200,50);
    m_searchedit->hide();

    m_searchprompt = new XLabel(this);
    m_searchprompt->setText(tr("Please enter 7 digital numbers"));
    m_searchprompt->setGeometry(140, 260, 460, 50);
    m_searchprompt->hide();

    m_ok = new XButton(this);
    m_ok->setGeometry(280,320,120,50);
    m_ok->setText(tr("Confirm"));
    connect(m_ok, SIGNAL(sigbtnclick()), this, SLOT(slotclickOK()));
    m_ok->hide();

    m_promptbox = new XPromptBox();
    m_promptbox->hide();
    connect(m_promptbox, SIGNAL(sigclickok(int)), this, SLOT(slotpromptok(int)));
    connect(m_promptbox, SIGNAL(sigclickcancel(int)), this, SLOT(slotpromptcs(int)));

}

void CBatchWidget::showEvent(QShowEvent *)
{
    m_batchrecord->show();
    m_searchprompt->hide();
    m_searchlabel->hide();
    m_searchedit->hide();
    m_ok->hide();


    m_spacevaule->setText(QString::number(CDataBase::get_RecordNum(BATCH_BODY_NAME))+"/"+QString::number(CDataBase::get_RecordMax(BATCH_BODY_NAME)));
    int first = CDataBase::get_MaxAdss(BATCH_BODY_NAME);
    updatarecord(BATCH_BODY_NAME,first,true);
}

void CBatchWidget::slotclickpageup()
{
    int first = m_batchrecord->item(0,0)->text().toInt()+1;
    int max = CDataBase::get_MaxAdss(BATCH_BODY_NAME);
    if(first > max) return;
    updatarecord(BATCH_BODY_NAME,first,true);
}

void CBatchWidget::slotclickpagedown()
{
    int first = m_batchrecord->item(SHOW_LINE-1,0)->text().toInt()-1;
    int min = CDataBase::get_MinAdss(BATCH_BODY_NAME);
    if(first < min) return;
    updatarecord(BATCH_BODY_NAME,first,true);
}

void CBatchWidget::slotclicksearch()
{
    if(!m_batchrecord->isHidden())
    {

        m_del1->setEnabled(false);
        //m_deln->setEnabled(false);
        m_pageup->setEnabled(false);
        m_pagedown->setEnabled(false);

        m_searchprompt->setText(tr("Please enter 7 digital numbers"));
        m_searchedit->clear();
        m_batchrecord->hide();
        m_searchprompt->show();
        m_searchlabel->show();
        m_searchedit->show();
        m_ok->show();
    }
}

void CBatchWidget::slotclickdel1()
{
    if((m_batchrecord->currentRow() < 0) || (m_batchrecord->currentRow() >= SHOW_LINE))
        return;
    QString batch = m_batchrecord->item(m_batchrecord->currentRow(),1)->text();
    if(batch.length()<=0) return;
    this->setEnabled(false);
    //m_promptbox->settitle(tr("此操作将删除%0批号数据，且不可恢复！\n\n确认：执行删除    取消：返回").arg(batch));
    m_promptbox->settitle(tr("This operation will delete the lot number of %0 and cannot be restored!!\n\nOK: delete       Cancel: return").arg(batch));
    m_promptbox->setnsec(6);
    m_promptbox->setwhichbutton(1);
    m_promptbox->setpromptser(0);
    m_promptbox->show();

}

void CBatchWidget::slotclickdeln()
{
    this->setEnabled(false);
    //m_promptbox->settitle(tr("此操作将清空所有批号数据，且不可恢复！\n\n确认：执行删除    取消：返回"));
    m_promptbox->settitle(tr("This operation will empty all batches and data, and can not be recovered!\n\nOK: delete        Cancel: return"));
    m_promptbox->setnsec(6);
    m_promptbox->setwhichbutton(1);
    m_promptbox->setpromptser(1);
    m_promptbox->show();
}

void CBatchWidget::slotclickback()
{
    if(m_batchrecord->isHidden())
    {
        m_del1->setEnabled(true);
        //m_deln->setEnabled(true);
        m_pageup->setEnabled(true);
        m_pagedown->setEnabled(true);
        m_batchrecord->show();
        m_searchprompt->hide();
        m_searchlabel->hide();
        m_searchedit->hide();
        m_ok->hide();
    }
    else
    {
        emit sigperform(RECD_WG);
    }

}

void CBatchWidget::updatarecord(QString table,int first,bool up_down)
{

    int row = 0;
    for(int i=row;i<SHOW_LINE;i++)
    {
        m_batchrecord->item(i,0)->setText("");
        m_batchrecord->item(i,1)->setText("");
        m_batchrecord->item(i,2)->setText("");
        m_batchrecord->item(i,3)->setText("");
    }

    if(first <= 0) return;

    if(up_down)//往下更新
    {
        //QSqlQuery query ;
        //query.exec(QObject::tr("SELECT * FROM %0 WHERE SERNUM <= %1 ORDER BY SERNUM DESC").arg(TABLE_BODY_NAME).arg(first));
        QSqlQuery query = CDataBase::select_Record(table,"SERNUM",QString::number(first),"<=",true);

        for(int i=0;i<SHOW_LINE;i++)
        {
            if(query.next())
            {
                QString ser = query.value(0).toString();
                QString batch = query.value(1).toString();
                QByteArray code = query.value(2).toByteArray();
                XArgDecode tmp(code);
                if(tmp.isvalid())
                {
                    QString pro = tmp.getAllPnName(",");
                    QString time = QString::number(tmp.getreatime());
                    m_batchrecord->item(i,0)->setText(ser);
                    m_batchrecord->item(i,1)->setText(batch);
                    m_batchrecord->item(i,2)->setText(time + "s");
                    m_batchrecord->item(i,3)->setText(pro);
                }
            }
            else
                break;
        }

    }
    else//往上更新
    {
        //QSqlQuery query ;
        //query.exec(QObject::tr("SELECT * FROM %0 WHERE SERNUM >= %1 ORDER BY SERNUM ASC").arg(TABLE_BODY_NAME).arg(first));
        QSqlQuery query = CDataBase::select_Record(table,"SERNUM",QString::number(first),">=",false);

        for(int i=0;i<SHOW_LINE;i++)
        {
            if(query.next())
            {
                QString ser = query.value(0).toString();
                QString batch = query.value(1).toString();
                QByteArray code = query.value(2).toByteArray();
                XArgDecode tmp(code);
                if(tmp.isvalid())
                {
                    QString pro = tmp.getAllPnName(",");
                    QString time = QString::number(tmp.getreatime());
                    m_batchrecord->item(SHOW_LINE-i-1,0)->setText(ser);
                    m_batchrecord->item(SHOW_LINE-i-1,1)->setText(batch);
                    m_batchrecord->item(SHOW_LINE-i-1,2)->setText(time + "s");
                    m_batchrecord->item(SHOW_LINE-i-1,3)->setText(pro);
                }
            }
            else
                break;
        }
        if(m_batchrecord->item(0,0)->text().isEmpty())
        {
            int ser=0;
            for(int i=1;i<SHOW_LINE;i++)
            {
                if(!m_batchrecord->item(i,0)->text().isEmpty())
                {
                   ser  = m_batchrecord->item(i,0)->text().toInt();
                   updatarecord(table,ser,true);
                   break;
                }
            }
        }
    }

}

void CBatchWidget::removebatch(int row)
{
    int ser = m_batchrecord->item(row,0)->text().toInt();
    m_batchrecord->removeRow(row);
    CDataBase::del_Record(BATCH_BODY_NAME, "SERNUM", ser);

}

void CBatchWidget::slotrecvnewbatch(QString batch)
{
    if(!this->isHidden())
    {
        m_spacevaule->setText(QString::number(CDataBase::get_RecordNum(BATCH_BODY_NAME))+"/"+QString::number(CDataBase::get_RecordMax(BATCH_BODY_NAME)));
        QSqlQuery query = CDataBase::select_Record(BATCH_BODY_NAME,"BATCH",batch);
        if(query.next())
        {
            QString ser = query.value(0).toString();
            QString batch = query.value(1).toString();
            QByteArray code = query.value(2).toByteArray();
            for(int i=0;i<SHOW_LINE;i++)
            {
                if(m_batchrecord->item(i,0)->text() == ser)
                {
                    XArgDecode tmp(code);
                    if(tmp.isvalid())
                    {
                        QString pro = tmp.getAllPnName(",");
                        QString time = QString::number(tmp.getreatime());
                        //m_batchrecord->item(i,0)->setText(ser);
                        m_batchrecord->item(i,1)->setText(batch);
                        m_batchrecord->item(i,2)->setText(time);
                        m_batchrecord->item(i,3)->setText(pro);
                    }
                    break;
                }
            }
        }
    }
}

void CBatchWidget::slotclickOK()
{
    if(m_searchedit->text().length() == 7)
    {
        QString batch;
        batch.append("Y");
        batch.append(m_searchedit->text());

        QSqlQuery query = CDataBase::select_Record(BATCH_BODY_NAME,"BATCH",batch);
        if(query.next())
        {

            m_del1->setEnabled(true);
            //m_deln->setEnabled(true);
            m_pageup->setEnabled(true);
            m_pagedown->setEnabled(true);
            m_batchrecord->show();
            m_searchprompt->hide();
            m_searchlabel->hide();
            m_searchedit->hide();
            m_ok->hide();

            int first = query.value(0).toInt();
            updatarecord(BATCH_BODY_NAME,first,false);
        }
        else
        {
            m_searchprompt->setText(tr("This batch not fond!"));
        }
    }
}

void CBatchWidget::slotpromptok(int ser)
{
    if(ser == 0)
    {

        if((m_batchrecord->currentRow() < 0) || (m_batchrecord->currentRow() >= SHOW_LINE))
            return;
        int ser = m_batchrecord->item(m_batchrecord->currentRow(),0)->text().toInt();
        //if(ser == 0) return;
        CDataBase::del_Record(BATCH_BODY_NAME,"SERNUM",ser);

        int first = m_batchrecord->item(0,0)->text().toInt();
        if(m_batchrecord->currentRow() == 0)
        {
            first = m_batchrecord->item(1,0)->text().toInt();
        }

        updatarecord(BATCH_BODY_NAME,first,true);
        m_batchrecord->setCurrentCell(0,0);
        m_spacevaule->setText(QString::number(CDataBase::get_RecordNum(BATCH_BODY_NAME))+"/"+QString::number(CDataBase::get_RecordMax(BATCH_BODY_NAME)));

    }
    else if(ser == 1)
    {
        CDataBase::del_Record(BATCH_BODY_NAME);
        for(int i=0;i<SHOW_LINE;i++)
        {
            m_batchrecord->item(i,0)->setText("");
            m_batchrecord->item(i,1)->setText("");
            m_batchrecord->item(i,2)->setText("");
            m_batchrecord->item(i,3)->setText("");
        }
        m_spacevaule->setText(QString::number(CDataBase::get_RecordNum(BATCH_BODY_NAME))+"/"+QString::number(CDataBase::get_RecordMax(BATCH_BODY_NAME)));

    }
    this->setEnabled(true);
}

void CBatchWidget::slotpromptcs(int ser)
{
    this->setEnabled(true);
}


