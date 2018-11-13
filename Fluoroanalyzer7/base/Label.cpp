#include "Label.h"
#include "common/configurate.h"

CLabel::CLabel(QWidget *parent) :
    QLabel(parent)
{
    this->setStyleSheet("color: rgb(0, 0, 0);" + FONTFAMILY + FONT_SIZE_2);
}
void CLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
