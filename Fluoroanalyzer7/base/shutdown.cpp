#include "shutdown.h"
#include "QApplication"

CShutdown::CShutdown(QWidget * parent): QPushButton(parent)
{
    m_parent = parent;

    QString strPath = QString(X_BGPIC_PATH) + "poweroff.png";

    if(QFileInfo(strPath).isFile())
        m_pixmap.load(strPath);

    m_promptbox = new XPromptBox();
    m_promptbox->hide();
    connect(m_promptbox, SIGNAL(sigclickok(int)), this, SLOT(slotpromptok(int)));
    connect(m_promptbox, SIGNAL(sigclickcancel(int)), this, SLOT(slotpromptcs(int)));
    //m_promptbox->settitle(tr("是否关机！\n\n确认：关机      取消：返回"));
    m_promptbox->settitle(tr("OK:Power off        Cancel:Return"));
	m_promptbox->setOkText(tr("OK"));
    connect(this, SIGNAL(clicked()), this, SLOT(onClick()));
}

CShutdown::~CShutdown()
{
    delete m_promptbox;
}

void CShutdown::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    paint.drawPixmap(0, 0, this->width(), this->height(), m_pixmap);
}

void CShutdown::onClick(bool checked)
{
    m_parent->setEnabled(false);

    m_promptbox->setnsec(6);
    m_promptbox->setwhichbutton(1);
    m_promptbox->setpromptser(0);
    m_promptbox->show();
}

void CShutdown::slotpromptok(int ser)
{
    extern QApplication * g_pApp;
    g_pApp->exit(0);
    system("halt");
}

void CShutdown::slotpromptcs(int ser)
{
    m_parent->setEnabled(true);
    m_promptbox->hide();
}
