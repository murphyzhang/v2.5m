#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>
#include <QtGui>
#include <QtCore>
#include <QPixmap>
#include <QPainter>
#include <QScrollBar>

class CTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit CTableWidget(QWidget *parent = 0);

    void paintEvent(QPaintEvent *e);

    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:

public slots:

private:
    QPixmap m_Pixmap;
    int     m_nCurPos;

};

#endif // TABLEWIDGET_H
