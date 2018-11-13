#include "xlineedit.h"
#include "common/configurate.h"

XLineEdit::XLineEdit(QWidget *parent): QLineEdit(parent)
{

    this->setStyleSheet("color: rgb(255, 0, 255);" + FONTFAMILY + FONT_SIZE_2);
    //this->setStyleSheet(style);

    /* 创建和输入法关联的面板 */
    inputPanel = new XInPutWidget;
    inputPanel->hide();
    /* 并将characterGenerated和sendCharacter关联，当用户按键后，则继续由sendCharacter向目标Widget */
    connect(inputPanel, SIGNAL(sigPBClicked(QString)), this,SLOT(slotTextActer(QString)));
    m_showPanel = true;
}
void XLineEdit::setPanelenable(bool enable)
{
    m_showPanel = enable;
}



void XLineEdit::slotTextActer(QString text)
{
//    qDebug()<<text;
    if(text == "DEL")
    {
        //this->setText(this->text().left(this->text().length() - 1));
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QApplication::sendEvent(this, &keyPress);
    }
    else if(text == "OK")
    {
        inputPanel->hide();
        this->deselect();
    }
    else
    {
        this->setText(this->text()+text);
    }
   /*
    QPointer<QWidget> w = focusWidget();
    qDebug()<<"sendCharacter = "<<character;
    if (!w)
        return;
    QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;
    QKeyEvent keyRelease(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString());
    QApplication::sendEvent(w, &keyRelease);
    */
}

void XLineEdit::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<"mousePressEvent";
    if(m_showPanel)
    {
        //inputPanel->xsetFocus(this);
        QRect widgetRect = this->rect();
        QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
        panelPos = this->mapToGlobal(panelPos);
        if((panelPos.x()+270) > QApplication::desktop()->width())
        {
            panelPos.setX(QApplication::desktop()->width() - 270);
        }
        if((panelPos.y()+145) > QApplication::desktop()->height())
        {
            panelPos.setY(QApplication::desktop()->height() - 145);
        }
        inputPanel->move(panelPos);
        inputPanel->show();
    }
    //this->setFocus(Qt::MouseFocusReason);
    //this->setFrame(false);
    emit siglineeditclick(this);
}

void XLineEdit::hideEvent(QHideEvent *)
{
    if(m_showPanel)
        inputPanel->hide();
}



