#include "promptbox.h"
#include "common/configurate.h"

XPromptBox::XPromptBox(QWidget *parent): QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(200, 120, 400, 225);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0x8b,0x8b,0xaf));
    this->setPalette(pal);

    m_1sec = new QTimer();
    connect(m_1sec, SIGNAL(timeout()), this, SLOT(slot1sec()));
    m_runnsec = 0;
    m_showtext = new XLabel(this);
    m_showtext->setGeometry(10,25,380,120);
    m_showtext->setAlignment(Qt::AlignCenter);
    m_showtext->setStyleSheet("color:rgb(255, 255, 255);" + FONTFAMILY + FONT_SIZE_6);
    m_showtext->setWordWrap(true);

	m_ok_text = "Confirm";
	m_cancel_text = "Cancel";
	
    m_ok = new XButton(this);
    m_ok->setGeometry(30,165,140,50);
    m_ok->setText(m_ok_text);
    connect(m_ok, SIGNAL(sigbtnclick()), this, SLOT(slotclickok()));

    m_cancel = new XButton(this);
    m_cancel->setGeometry(230,165,140,50);
    m_cancel->setText(m_cancel_text);
    connect(m_cancel, SIGNAL(sigbtnclick()), this, SLOT(slotclickcancel()));

    m_whichone = 0;
	

}

void XPromptBox::showEvent(QShowEvent *)
{
    m_showtext->setText(m_title);
    m_ok->setText(m_ok_text);
    m_cancel->setText(m_cancel_text);
    if(m_runnsec > 0)
    {
        if(m_whichone == 0)
        {
            m_ok->setText(m_ok_text +"("+QString::number(m_runnsec)+")");
        }
        else
        {
            m_cancel->setText(m_cancel_text+"("+QString::number(m_runnsec)+")");
        }
        m_1sec->start(1000);
    }
}
void XPromptBox::hideEvent(QHideEvent *)
{
    m_1sec->stop();
}
void XPromptBox::slot1sec()
{
    if(m_whichone == 0)
    {
        m_ok->setText(m_ok_text+"("+QString::number(m_runnsec)+")");
        m_cancel->setText(m_cancel_text);
    }
    else
    {
        m_ok->setText(m_ok_text);
        m_cancel->setText(m_cancel_text+"("+QString::number(m_runnsec)+")");
    }
    m_runnsec--;
    if(m_runnsec < 0)
    {
        if(m_whichone == 0)
        {
            emit sigclickok(m_promptser);
        }
        else
        {
            emit sigclickcancel(m_promptser);
        }
        this->hide();
    }
}

void XPromptBox::setnsec(int nsec)
{
    if(nsec == 0)
    {
        m_ok->setVisible(false);
        m_cancel->setVisible(false);
        m_runnsec = 0;
        m_1sec->stop();
    }
    else
    {
        m_ok->setVisible(true);
        m_cancel->setVisible(true);
        m_runnsec = nsec;
    }

}

void XPromptBox::settitle(QString title)
{
    m_title = title;
    m_showtext->setText(m_title);
}

void XPromptBox::setwhichbutton(int button)
{
    m_whichone = button;
}

void XPromptBox::slotclickcancel()
{
    this->hide();
    emit sigclickcancel(m_promptser);
}

void XPromptBox::slotclickok()
{
    this->hide();
    emit sigclickok(m_promptser);
}

void XPromptBox::setpromptser(int ser)
{
    m_promptser = ser;
}

void XPromptBox::setnobutton()
{
    setnsec(0);
}

void XPromptBox::showprompt(QString title, int time, int button, XBUTTON_EM def)
{
    m_showtext->setText(title);
    if(time > 0)
    {
        m_runnsec = time;
    }
    if(button == XBT_NUL)
    {

    }
    else if((button&XBT_YES) == XBT_YES)
    {

    }
    else if((button&XBT_CMS) == XBT_CMS)
    {

    }

    this->show();
}
void XPromptBox::setOkText(QString okText)
{
    m_ok_text = okText;
    m_ok->setText(m_ok_text);
}

