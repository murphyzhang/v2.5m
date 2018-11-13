#include "sysinitwidget.h"
#include "common/configurate.h"

CSysInitWidget::CSysInitWidget(QWidget *parent) :
    CBaseFunWidget(parent)
{

    m_label = new QLabel(this);
    m_label->setGeometry(50,220,700,40);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet("color:rgb(255, 255, 255);" + FONTFAMILY + FONT_SIZE_1);
    m_label->setText(tr("System is initializing. Please wait."));
    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(showtime()));
}

CSysInitWidget::~CSysInitWidget()
{

}

void CSysInitWidget::showEvent(QShowEvent *)
{
    m_timer->start(1000);
//    CDataBase::Init(X_BTDB_PATH);
}

void CSysInitWidget::hideEvent(QHideEvent *)
{

}

void CSysInitWidget::showtime()
{
    m_timer->stop();
    emit sigperform(CHECK_WG);
}
