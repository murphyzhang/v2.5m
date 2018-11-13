#include "timeeditwidget.h"
#include "common/configurate.h"
#include <QDebug>

#include <stdio.h>

CTimeEditWidget::CTimeEditWidget(QWidget *parent) :
    CBaseFunWidget(parent)
{
//    qDebug()<<"CTimeEditWidget";
    m_dt[0] = new QPushButton(this);
    m_dt[0]->setGeometry(70,210,100,40);
    m_dt[0]->setStyleSheet(FONTFAMILY + FONT_SIZE_1);
    connect(m_dt[0], SIGNAL(clicked()), this, SLOT(slotclicktime()));

    for(int i=1;i<6;i++)
    {
        m_label[i] = new QLabel(this);
        m_label[i]->setGeometry(60+112*i,210,20,40);
        m_label[i]->setStyleSheet(FONTFAMILY + FONT_SIZE_1);
        m_label[i]->setAlignment(Qt::AlignCenter);

        m_dt[i] = new QPushButton(this);
        m_dt[i]->setGeometry(80+112*i,210,80,40);
        m_dt[i]->setStyleSheet(FONTFAMILY + FONT_SIZE_1 + "");
        connect(m_dt[i], SIGNAL(clicked()), this, SLOT(slotclicktime()));
    }
    m_label[1]->setText("-");
    m_label[2]->setText("-");

    m_label[4]->setText(":");
    m_label[5]->setText(":");

    m_add = new XButton(this);
    m_add->setGeometry(70,150,100,50);
    m_add->setText(tr("▲"));
    connect(m_add, SIGNAL(sigbtnclick()), this, SLOT(slotclickadd()));


    m_sub = new XButton(this);
    m_sub->setGeometry(70,260,100,50);
    m_sub->setText(tr("▼"));
    connect(m_sub, SIGNAL(sigbtnclick()), this, SLOT(slotclicksub()));

    m_save = new XButton(this);
    m_save->setGeometry(450,350,120,50);
    //m_save->setText(tr("保存"));
    m_save->setText(tr("Save"));
    connect(m_save, SIGNAL(sigbtnclick()), this, SLOT(slotclicksave()));

    m_back = new XButton(this);
    m_back->setGeometry(600,350,120,50);
    //m_back->setText(tr("返回"));
    m_back->setText(tr("Back"));
    connect(m_back, SIGNAL(sigbtnclick()), this, SLOT(slotclickback()));

    m_setn = 0;

}

void CTimeEditWidget::slotclickadd()
{
    switch(m_setn)
    {
    case 0:
    {
        m_datetime = m_datetime.addYears(1);
    }break;
    case 1:
    {
        m_datetime = m_datetime.addMonths(1);
    }break;
    case 2:
    {
        m_datetime = m_datetime.addDays(1);
    }break;
    case 3:
    {
        m_datetime = m_datetime.addSecs(3600);
    }break;
    case 4:
    {
        m_datetime = m_datetime.addSecs(60);
    }break;
    case 5:
    {
        m_datetime = m_datetime.addSecs(1);
    }break;
    default:
        break;
    }
    timetoshow();
}

void CTimeEditWidget::slotclicksub()
{
    switch(m_setn)
    {
    case 0:
    {
        m_datetime = m_datetime.addYears(-1);
    }break;
    case 1:
    {
        m_datetime = m_datetime.addMonths(-1);
    }break;
    case 2:
    {
        m_datetime = m_datetime.addDays(-1);
    }break;
    case 3:
    {
        m_datetime = m_datetime.addSecs(-3600);
    }break;
    case 4:
    {
        m_datetime = m_datetime.addSecs(-60);
    }break;
    case 5:
    {
        m_datetime = m_datetime.addSecs(-1);
    }break;
    default:
        break;
    }
    timetoshow();
}

void CTimeEditWidget::showEvent(QShowEvent *)
{
    m_setn = 0;
    m_add->setGeometry(70,150,100,50);
    m_sub->setGeometry(70,260,100,50);
//    qDebug()<<"showEvent000";
    m_datetime = QDateTime::currentDateTime();
    timetoshow();
//    qDebug()<<"showEvent111";
}

void CTimeEditWidget::slotclicksave()
{
    SetCurDateTime(m_datetime.toString("yyyy-MM-dd hh:mm:ss"));
    emit sigperform(MENU_WG);
}
void CTimeEditWidget::slotclickback()
{
    emit sigperform(MENU_WG);
}

void CTimeEditWidget::slotclicktime()
{
//    qDebug()<<"slotclicktime"<<this->focusWidget();
    int i = 0;
    while(i<6)
    {
        if(this->focusWidget() == m_dt[i])
        {
            m_add->setGeometry(70+112*i,150,100,50);
            m_sub->setGeometry(70+112*i,260,100,50);
            m_setn = i;
            break;
        }
        i++;
    }
}

void CTimeEditWidget::timetoshow()
{
    //qDebug() << m_datetime;
    m_dt[0]->setText(m_datetime.toString("yyyy"));
    m_dt[1]->setText(m_datetime.toString("MM"));
    m_dt[2]->setText(m_datetime.toString("dd"));

    m_dt[3]->setText(m_datetime.toString("hh"));
    m_dt[4]->setText(m_datetime.toString("mm"));
    m_dt[5]->setText(m_datetime.toString("ss"));
}

int CTimeEditWidget::SetCurDateTime(QString strTime)
{
#ifdef VER_LINUX
    char cOne = '"';
    QString strCommand;
    strCommand.append("date -s ");
    strCommand.append(cOne);
    strCommand.append(strTime);
    strCommand.append(cOne);

    qDebug() << "system = "
             << system(strCommand.toStdString().c_str());
//    usleep(100);
    system("hwclock -w");
#endif
}
