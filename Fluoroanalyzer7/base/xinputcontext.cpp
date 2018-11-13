#include "xinputcontext.h"

XInputContext::XInputContext()
{
    /* 创建和输入法关联的面板 */
    inputPanel = new XInPutWidget;

    /* 并将characterGenerated和sendCharacter关联，当用户按键后，则继续由sendCharacter向目标Widget */
    connect(inputPanel, SIGNAL(characterGenerated(QChar)), this,SLOT(sendCharacter(QChar)));

}
bool XInputContext::filterEvent(const QEvent* event)
{

    //qDebug()<<event->type();
    return true;
    //if (event->type() == QEvent::RequestSoftwareInputPanel)
    {
        /* 当某个Widget请求软键盘输入时，显示软键盘 */
        updatePosition();
        inputPanel->show();
        return true;
    }
    //else if (event->type() == QEvent::CloseSoftwareInputPanel)
    {
        /* 当某个Widget请求关闭软键盘输入时，关闭软键盘 */
        inputPanel->hide();
        return true;
    }
    return false;
}

QString XInputContext::identifierName()
{
    return "XInputContext";
}

void XInputContext::reset()
{
}

bool XInputContext::isComposing() const
{
    return false;
}

QString XInputContext::language()
{
    return "en_US";
}

void XInputContext::sendCharacter(QChar character)
{
    QPointer<QWidget> w = focusWidget();

    if (!w)
        return;

    /* 当收到按键面板的按键输入后，分别向当前焦点Widget发送KeyPress和KeyRelease事件 */

    QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;
    QKeyEvent keyRelease(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString());
    QApplication::sendEvent(w, &keyRelease);
}



/* 根据当前焦点Widget的位置，调整输入面板的位置 */

void XInputContext::updatePosition()
{
    QWidget *widget = focusWidget();
    if (!widget)
        return;

    QRect widgetRect = widget->rect();
    QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
    panelPos = widget->mapToGlobal(panelPos);
    inputPanel->move(panelPos);
}
