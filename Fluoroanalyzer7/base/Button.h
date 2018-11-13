#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QtGui>
#include <QPixmap>

class CButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CButton(QWidget *parent = 0);
    CButton(QPixmap & pixmap1, QPixmap & pixmap2, QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

signals:
    void SigBtnClick();

public slots:

private :
    QPixmap m_pPress;
    QPixmap m_pRelease;
    int m_nPress; // 0没按下 1按下

    int nUseState; // 使用的是那种构造函数 0只有1个参数的 1有3个参数的

};

#endif // BUTTON_H
