#ifndef XLINEEDIT_H
#define XLINEEDIT_H

#include <QLineEdit>
#include "widget/inputwidget.h"

class XLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit XLineEdit(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event) ;
    void hideEvent(QHideEvent *);
    void setPanelenable(bool enable);

signals:
    void siglineeditclick(XLineEdit *id);
public slots:
    /* 槽用于接收从输入面板输入的字符 */
    void slotTextActer(QString text);


private:
    /* 关联的输入面板 */
    XInPutWidget *inputPanel;
    bool m_showPanel;
};

#endif // XLINEEDIT_H
