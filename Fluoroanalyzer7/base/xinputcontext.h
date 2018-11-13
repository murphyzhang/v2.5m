#ifndef XINPUTCONTEXT_H
#define XINPUTCONTEXT_H

#include <QInputContext>
#include "widget/inputwidget.h"
class XInputContext : public QInputContext
{
    Q_OBJECT
public:
    XInputContext();
    /* 事件过滤器 */

    bool filterEvent(const QEvent* event);

    QString identifierName();
    QString language();

    bool isComposing() const;

    void reset();

private slots:

    /* 槽用于接收从输入面板输入的字符 */
    void sendCharacter(QChar character);

private:
    void updatePosition();

private:

    /* 关联的输入面板 */
    XInPutWidget *inputPanel;
};

#endif // XINPUTCONTEXT_H
