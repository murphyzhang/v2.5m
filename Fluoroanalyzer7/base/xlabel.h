#ifndef XLABEL_H
#define XLABEL_H

#include <QLabel>

class XLabel : public QLabel
{
    Q_OBJECT
public:
    explicit XLabel(QWidget *parent = 0);
    void mousePressEvent ( QMouseEvent * event);
signals:
    void clicked();


public slots:

};

#endif // XLABEL_H
