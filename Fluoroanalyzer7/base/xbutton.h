#ifndef XBUTTON_H
#define XBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <QFileInfo>
#include <common/configfile.h>

class XButton : public QPushButton
{
    Q_OBJECT
public:
    explicit XButton(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

    void setpresspng(QString url);
    void setreleasepng(QString url);
    void settextcolor(QColor color);
signals:
    void sigbtnclick();
    void sigbtnclick(QString text);
public slots:
private:
    QColor m_color;

    QPixmap m_bgpre;
    QPixmap m_bgrel;

    bool m_pressed;
};

#endif // XBUTTON_H
