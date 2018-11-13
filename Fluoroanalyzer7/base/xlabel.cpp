#include "xlabel.h"
#include "common/configurate.h"

XLabel::XLabel(QWidget *parent): QLabel(parent)
{
    this->setStyleSheet("color:rgb(255, 255, 255);" + FONTFAMILY + FONT_SIZE_2);
}

void XLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
