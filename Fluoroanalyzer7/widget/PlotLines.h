#ifndef PLOTLINES_H
#define PLOTLINES_H

#include <QPen>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_plot_renderer.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_histogram.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_plot_magnifier.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_legend_label.h>
#include <qwt/qwt_column_symbol.h>
#include <qwt/qwt_series_data.h>
#include <qwt/qwt_symbol.h>
#include <qwt/qwt_picker_machine.h>
#include <qwt/qwt_scale_draw.h>
#include "widget/basefunwidget.h"
#include "common/TestData.h"
#include "common/commoninterface.h"

typedef struct PRO{
    char item[20];
    double result[20];
    char unit[20];
}PRO_T;

typedef struct UNITE{
    int iUnite;
    PRO_T pro[10];
}UNITE_T;

class DateScaleDraw : public QwtScaleDraw
{
public:
    DateScaleDraw( const QTime &base ):
        baseTime(base)
    {
    }
    virtual QwtText label(double v) const
    {
        QDateTime dt = QDateTime::fromTime_t(static_cast<int>(v));
        return dt.toString("yyyy-MM-dd");
    }
private:
    QTime baseTime;
};

class CPlotLines : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CPlotLines(QWidget *parent = 0);
    void SetValue(QVector<CTestData *> vecData);
    void DrawLines();
    void GetSpots(int iIndex);
    double GetMaxValue(int iIndex);
    void showEvent(QShowEvent *);
    void mousePressEvent(QMouseEvent *);

private Q_SLOTS:
    void showItem(const QVariant &itemInfo,bool on,int iIndex);//点击图例，显示相应的曲线
    void BtReturnClicked();

private:
    double m_dArrMax;
    double m_dMax;
    int m_iUnite;
    UNITE_T *m_pUnite;
    QwtPlot *m_pQwtPlot;
    QVector<CTestData *> vecTestData;
    QPolygonF m_PolygonF;
    XButton *m_pBtReturn;
    QwtLegend *m_pLegend;
};

#endif // PLOTLINES_H
