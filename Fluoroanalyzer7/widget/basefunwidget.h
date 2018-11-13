#ifndef BASEFUNWIDGET_H
#define BASEFUNWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QLabel>

#include "base/xbutton.h"
#include "base/Label.h"
#include "base/xlineedit.h"
#include "base/xlabel.h"

#include "common/configfile.h"
#include "common/global_define.h"

class CBaseFunWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CBaseFunWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

signals:
    void sigperform(ENUM_WIDGET turn_wg);
public slots:

private:
    QPixmap m_bgPixmap;
};

#endif // BASEFUNWIDGET_H
