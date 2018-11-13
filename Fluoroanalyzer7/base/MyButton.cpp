#include "MyButton.h"

CMyButton::CMyButton(QWidget *parent): QPushButton(parent)
{
    m_pressed = false;
}

void CMyButton::mousePressEvent(QMouseEvent *e)
{
    m_pressed = true;
}

void CMyButton::mouseReleaseEvent(QMouseEvent *e)
{
    m_pressed = false;
    update();
    emit sigbtnclick(m_iNum);
}

void CMyButton::SetNum(int iNum)
{
    m_iNum = iNum;
}
