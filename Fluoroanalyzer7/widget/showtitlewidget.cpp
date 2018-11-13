#include "showtitlewidget.h"
#include "common/configurate.h"
//#include "ui_showtitlewidget.h"

CShowTitleWidget::CShowTitleWidget(QWidget *parent) :
    QWidget(parent)
{
    m_label = new XLabel(this);
    set_geometry(0,0,400,40);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0x00));
    this->setPalette(pal);
    //this->setStyleSheet("font: 14pt wenquanyi");
    m_label->setStyleSheet("color:rgb(255, 255, 255);" + FONTFAMILY + FONT_SIZE_3);



}

CShowTitleWidget::~CShowTitleWidget()
{
    //delete ui;
}

void CShowTitleWidget::set_text(QString title)
{
    m_label->setText(title);
    m_title = title;
}

void CShowTitleWidget::set_timer(int ms)
{
    if(ms>0)
    {
        m_timer = new QTimer;
        connect(m_timer, SIGNAL(timeout()), this, SLOT(updata_title()));
        m_timer->start(ms);
        updata_title();
    }
    else
    {
        m_timer->stop();
        m_label->clear();
    }
}

void CShowTitleWidget::updata_title()
{
    QDateTime cDataTime;
    cDataTime.setTime(QTime::currentTime());
    cDataTime.setDate(QDate::currentDate());
    QString title = cDataTime.toString("yyyy-MM-dd hh:mm:ss");
    m_label->setText(title);
}

void CShowTitleWidget::set_geometry(int x, int y, int w, int h)
{
    this->setGeometry(x,y,w,h);
    m_label->setGeometry(0,0,w,h);
}

void CShowTitleWidget::set_alignment(Qt::Alignment ag)
{
    m_label->setAlignment(ag);
}

void CShowTitleWidget::setprompt()
{
    m_promptshow = new QTextEdit(this);
    m_promptshow->setStyleSheet("color:rgb(255, 255, 255);border:none;" + FONTFAMILY + FONT_SIZE_6);
    m_promptshow->setReadOnly(true);
    m_promptshow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_promptshow->setGeometry(0,0,400,40);
    m_promptshow->setAlignment(Qt::AlignVCenter);
    m_promptshow->verticalScrollBar()->setSliderPosition(0);
    //m_promptshow->setAlignment();
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x0,0x0,0x0,0x00));
    m_promptshow->setPalette(pal);
    m_srcoll = new QTimer;
    connect(m_srcoll, SIGNAL(timeout()), this, SLOT(scroll_prompt()));
    m_srcoll->start(100);
}

void CShowTitleWidget::scroll_prompt()
{
    if(m_startsrcoll)
    {
        m_promptshow->verticalScrollBar()->setSliderPosition(m_currpos);
        m_currpos+=2;
        if(m_currpos >= 56)
        {
            m_startsrcoll = false;
            m_promptshow->setText(m_textshow);
        }
    }
    if(!m_textshow.isEmpty() && (m_texttime++ >= 70))
    {
        m_texttime = 0;
        m_promptshow->setText("");
        m_textshow.clear();
    }

}

void CShowTitleWidget::showtext(QString text)
{
    if(!m_textshow.isEmpty())
    {
        m_currpos = 14;
        m_startsrcoll = true;
        m_textshow.append("\n");
    }
    m_textshow.append(text);
    m_texttime = 0;
    m_promptshow->setText(m_textshow);
    m_textshow = text;
}
