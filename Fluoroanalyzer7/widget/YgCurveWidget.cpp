#include "YgCurveWidget.h"
#include "common/configurate.h"
#include "common/commoninterface.h"

#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_point_data.h>


CygCurveWidget::CygCurveWidget(QWidget *parent): CBaseFunWidget(parent)
{
#ifdef WIDGET_DEBUG
    qDebug() << "CygCurveWidget create";
#endif

    m_pPlot = new QwtPlot(this);
    m_pPlot->setGeometry(20, 40, 760, 400);
    m_pPlot->setStyleSheet(FONTFAMILY + FONT_SIZE_5);
//    m_pPlot->canvas()->setStyleSheet("background-color: rgb(0xff, 0xff, 0xff);");
    //    m_pPlot->canvas()->setMaximumHeight(300);
    //    //设置坐标轴的名称
    //    m_pPlot->setAxisTitle(QwtPlot::xBottom, "x->");
    //    m_pPlot->setAxisTitle(QwtPlot::yLeft, "y->");
    //设置坐标轴的范围
    m_pPlot->setAxisScale(QwtPlot::xBottom, 0, X_LEN);
    m_pPlot->setAxisScale(QwtPlot::yLeft, 0, Y_LEN);

    QPen pen(Qt::blue);
    pen.setWidth(3);
    m_pCurve = new QwtPlotCurve;
    m_pCurve->setPen(pen);
    m_pCurve->setStyle(QwtPlotCurve::Lines);
    m_pCurve->setCurveAttribute(QwtPlotCurve::Fitted, true);
    m_pCurve->attach(m_pPlot);

//    returnButton = new XButton(this);
//    returnButton->setGeometry(660, 416, 0, 0);
//    returnButton->setText(tr("返回"));
//    connect(returnButton, SIGNAL(sigbtnclick()), this, SLOT(slotclickback()));

    memset(pData, 0, X_LEN);
}

CygCurveWidget::~CygCurveWidget()
{
    delete m_pPlot;
    delete m_pCurve;
}

void CygCurveWidget::showEvent(QShowEvent *e)
{
    //计算曲线数据
    QVector<double> vecX;
    QVector<double> vecY;

    for (int x = 0; x < X_LEN; x++)
    {
        vecX.append(x);
        vecY.append(pData[x]);
    }

    //构造曲线数据
    QwtPointArrayData * const data = new QwtPointArrayData(vecX, vecY);
    m_pCurve->setData(data);//设置数据
    //    m_pCurve->attach(m_pPlot);
    m_pPlot->replot();
}

void CygCurveWidget::hideEvent(QHideEvent *)
{
    //    m_pCurve->detach();
    memset(pData, 0, (X_LEN * (sizeof(uint) )));
}

void CygCurveWidget::slotSetData(QByteArray arr, ENUM_WIDGET retWg)
{
    if (arr.length() > 0)
    {
#if 0
        uint * pun = (uint *)arr.data();

        for (int x = 0; x < X_LEN; x++)
        {
            pData[x] = pun[x] * (SCALE / 524287.0);
        }
#else
        CI::flstring_tofldata(arr, pData);

        for (int x = 0; x < X_LEN; x++)
        {
            pData[x] = pData[x] * (SCALE / 524287.0);
        }
#endif

        this->returnWg = retWg;
    }
}

void CygCurveWidget::mousePressEvent(QMouseEvent * e)
{
    emit this->sigperform(returnWg);
    e->accept();
}

