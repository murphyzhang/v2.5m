#ifndef YGCURVE_H
#define YGCURVE_H


#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

#include "basefunwidget.h"
#include "common/Formula.h"


#define X_LEN   DATA_MAX_SIZE
#define Y_LEN   (2500)
#define SCALE   (2500)



class CygCurveWidget: public CBaseFunWidget
{
    Q_OBJECT

public:
    explicit CygCurveWidget(QWidget *parent = 0);
    virtual ~CygCurveWidget();

    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *);

private:
    void mousePressEvent(QMouseEvent *);

public slots:
    void slotSetData(QByteArray arr, ENUM_WIDGET retWg);

private slots:
//    void slotclickback();

private:
    //    CButton * m_pbtnBack;
    QwtPlot * m_pPlot;
    QwtPlotCurve * m_pCurve;
    uint pData[X_LEN];
//    XButton *returnButton;

    ENUM_WIDGET returnWg;
};

#endif // YGCURVE_H
