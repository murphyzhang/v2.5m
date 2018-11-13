#ifndef SYSINITWIDGET_H
#define SYSINITWIDGET_H

#include "basefunwidget.h"
#include "common/commoninterface.h"
#include "common/DataBase.h"

class CSysInitWidget : public CBaseFunWidget
{
    Q_OBJECT

public:
    explicit CSysInitWidget(QWidget *parent = 0);
    ~CSysInitWidget();

    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
private slots:
    void showtime();
private:
    QPixmap m_bgPixmap;
    QLabel *m_label;
    CShowTitleWidget *m_title[4];
    QTimer *m_timer;
};

#endif // SYSINITWIDGET_H
