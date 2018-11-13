#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include <QPushButton>

class CMyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CMyButton(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void SetNum(int iNum);

signals:
    void sigbtnclick(int iNum);

public slots:

private:
    int m_iNum;
    bool m_pressed;

};

#endif // MYBUTTON_H
