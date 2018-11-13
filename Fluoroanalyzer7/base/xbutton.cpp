#include "xbutton.h"
#include <QDebug>
#include <QVariant>
#include "common/configurate.h"


XButton::XButton(QWidget *parent): QPushButton(parent)
{
    this->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    /*
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0x00));
    this->setPalette(pal);
    this->setStyleSheet("color:rgb(255, 255, 255)");
    */

    setpresspng(QString(X_BGPIC_PATH) + QString("btnpre.png"));
    setreleasepng(QString(X_BGPIC_PATH) + QString("btnrel.png"));
    //settextcolor(QColor(0xff,0xd7,0x00));
    settextcolor(QColor(0xff,0xff,0xff));
    this->setGeometry(0, 0, m_bgrel.width(), m_bgrel.height());


    m_pressed = false;

}
void XButton::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);

    //qDebug()<<"text "<<this->text();

    if(m_pressed)
    {
        paint.drawPixmap(0, 0, this->width(), this->height(), m_bgpre);

    }
    else
    {
        paint.drawPixmap(0, 0, this->width(), this->height(), m_bgrel);
        //QPushButton::paintEvent(e);
    }
    paint.setPen(m_color);
    paint.drawText(this->rect(),Qt::AlignCenter,this->text());
}

void XButton::mousePressEvent(QMouseEvent *e)
{
    m_pressed = true;
    update();
}

void XButton::mouseReleaseEvent(QMouseEvent *e)
{
    m_pressed = false;
    update();
    emit sigbtnclick();
    emit sigbtnclick(this->text());
}

void XButton::setpresspng(QString url)
{
    if(QFileInfo(url).isFile())
        m_bgpre.load(url);
}

void XButton::setreleasepng(QString url)
{
    if(QFileInfo(url).isFile())
        m_bgrel.load(url);
}

void XButton::settextcolor(QColor color)
{
    m_color = color;
}
