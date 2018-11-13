#include "basefunwidget.h"
#include "common/configurate.h"

CBaseFunWidget::CBaseFunWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0, 0, 800, 480);
    this->setStyleSheet(FONTFAMILY + FONT_SIZE_2);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0x00));
    this->setPalette(pal);
    this->setStyleSheet("color:rgb(255, 255, 255)");
    m_bgPixmap.load(QString(X_BGPIC_PATH) + QString("background.jpg"));
}

void CBaseFunWidget::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.drawPixmap(0, 0, this->width(), this->height(), m_bgPixmap);
}
