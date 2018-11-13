#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class CLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CLabel(QWidget *parent = 0);
    void mousePressEvent ( QMouseEvent * event ) ;
signals:
    void clicked();

public slots:

};

#endif // LABEL_H
