#include "Button.h"
#include "common/configurate.h"
#include <QPainter>

CButton::CButton(QWidget *parent) :
    QPushButton(parent)
{
    this->setStyleSheet(FONTFAMILY + FONT_SIZE_2);


    m_nPress = 0;
    nUseState = 0;    
    // this->setStyleSheet("background-color: rgb(255, 255, 0);");
    // this->setStyleSheet("color: rgb(255, 255, 255);");
    //    this->setStyleSheet("color: rgb(255, 255, 255);\
    //                        background-color: rgb(0, 0, 127);");



}

CButton::CButton(QPixmap & pixmap1, QPixmap & pixmap2, QWidget *parent):
    QPushButton(parent)
{
    m_pPress = pixmap1;
    m_pRelease = pixmap2;
    m_nPress = 0;
    nUseState = 1;

}

void CButton::mousePressEvent(QMouseEvent *e)
{
   // this->setStyleSheet("background-color: rgb(255, 255, 0);");
    m_nPress = 1;
    update();
}

void CButton::mouseReleaseEvent(QMouseEvent *e)
{
    // this->setStyleSheet("background-color: rgb(0, 0, 127);");
    m_nPress = 0;
    update();
    emit  SigBtnClick();
}

void CButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(m_nPress == 1)
    {
        painter.drawPixmap(0, 0, this->width(), this->height(), this->m_pPress);
    }
    else
    {
        painter.drawPixmap(0, 0, this->width(), this->height(), this->m_pRelease);
    }

    if(nUseState == 0)
    {
        return QPushButton::paintEvent(e);
    }
}
