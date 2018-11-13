#include "PlotLines.h"

CPlotLines::CPlotLines(QWidget *parent) :
    CBaseFunWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0, 0, 800, 480);
    //this->setStyleSheet("font: 16pt wenquanyi");

    m_dMax = 0;
    m_pUnite = (UNITE_T *)malloc(sizeof(UNITE_T));
    memset(m_pUnite,0,sizeof(UNITE_T));

    m_pLegend = new QwtLegend;

    m_pQwtPlot = new QwtPlot(this);
    m_pQwtPlot->setGeometry(20,40,760,400);
    m_pQwtPlot->setStyleSheet("color:rgb(255, 255, 255);");
    QwtText title;
    title.setText(tr("Test trend chart"));
    m_pQwtPlot->setTitle(title);

//    m_pBtReturn = new XButton(this);
//    m_pBtReturn->setText(tr("←"));
//    m_pBtReturn->setGeometry(10,230,50,30);
//    connect(m_pBtReturn,SIGNAL(sigbtnclick()),this,SLOT(BtReturnClicked()));
}

void CPlotLines::showEvent(QShowEvent *)
{
    DrawLines();
}

void CPlotLines::DrawLines()
{
    qDebug()<<"Start DrawLines";
    m_pQwtPlot->detachItems();
    //---------设置画布---------//
    QwtPlotCanvas *canvas=new QwtPlotCanvas();
    canvas->setPalette(Qt::white);
    //canvas->setBorderRadius(10);
    m_pQwtPlot->setCanvas(canvas);
    m_pQwtPlot->plotLayout()->setAlignCanvasToScales(true);
    //-----------设置x，y坐标和范围--------------//
    m_pQwtPlot->setAxisTitle(QwtPlot::yLeft,tr("%1  ( %2 )").arg(vecTestData.at(0)->GetItem()).arg(vecTestData.at(0)->GetUnit()));
    m_pQwtPlot->setAxisTitle(QwtPlot::xBottom,tr(""));//Times
    m_pQwtPlot->setAxisScale(QwtPlot::yLeft,0.0,m_dMax + m_dMax/10);
    m_pQwtPlot->setAxisScale(QwtPlot::xBottom,0,vecTestData.count()/vecTestData.at(0)->GetUnite()+1);
    m_pQwtPlot->setAxisMaxMajor(QwtPlot::xBottom,vecTestData.count()/vecTestData.at(0)->GetUnite()+1);
    m_pQwtPlot->setAxisMaxMinor(QwtPlot::xBottom,0);
    //----------------设置栅格线-------------------//
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX(true);//设置网格线
    grid->enableY(true);
    grid->setMajorPen(Qt::black,0,Qt::DotLine);
    grid->attach(m_pQwtPlot);
    //-----------------开始画图----------------------//
    qDebug()<<"Start DrawLines 01";
    for(int i=0;i<m_iUnite;i++)
    {
        GetSpots(i);
        QwtPlotCurve *curve=new QwtPlotCurve(vecTestData.at(i)->GetItem());
        curve->setPen(Qt::blue,2);//设置曲线颜色 粗细
        curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化

        //QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::yellow),QPen(Qt::red,2),QSize(6,6));//设置样本点的颜色、大小
        QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::blue),QPen(Qt::blue,4),QSize(4,4));//设置样本点的颜色、大小
        curve->setSymbol( symbol );//添加样本点形状

        qDebug()<<m_PolygonF;
        curve->setSamples(m_PolygonF);
        curve->attach(m_pQwtPlot);
        curve->setLegendAttribute(curve->LegendShowLine);//显示图例的标志，这里显示线的颜色。
    }



    //--------------设置图例可以被点击来确定是否显示曲线--------------//
    //QwtLegend *legend = new QwtLegend;
    m_pLegend->setDefaultItemMode( QwtLegendData::Checkable );//图例可被点击
    m_pQwtPlot->insertLegend(m_pLegend,QwtPlot::RightLegend );
    connect(m_pLegend,SIGNAL(checked(const QVariant &,bool,int)), SLOT(showItem(const QVariant &,bool,int)));//点击图例操作

    QwtPlotItemList items = m_pQwtPlot->itemList(QwtPlotItem::Rtti_PlotCurve);//获取画了多少条曲线,如果为获取其他形状，注意改变参数
    for ( int i = 0; i < items.size(); i++ )
    {
        if ( i == 0 )
        {
            const QVariant itemInfo = m_pQwtPlot->itemToInfo( items[i] );

            QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel *>( m_pLegend->legendWidget( itemInfo ) );
            if ( legendLabel )
                legendLabel->setChecked(true);

            GetMaxValue(i);
            m_pQwtPlot->setAxisScale(QwtPlot::yLeft,0.0,m_dArrMax + m_dArrMax/10);

            items[i]->setVisible(true);
        }
        else
        {
            items[i]->setVisible(false);
        }
    }

    m_pQwtPlot->replot();
    m_pQwtPlot->setAutoReplot(true);//设置自动重画，相当于更新
}

//点击图例，显示相应的曲线
void CPlotLines::showItem(const QVariant &itemInfo,bool on,int iIndex)
{
    qDebug()<<"bool on:"<<on;
    QwtPlotItemList items = m_pQwtPlot->itemList(QwtPlotItem::Rtti_PlotCurve);
    for(int i=0;i<items.size();i++)
    {
        const QVariant itemInfoTemp = m_pQwtPlot->itemToInfo( items[i] );
        QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel *>( m_pLegend->legendWidget( itemInfoTemp ) );
        if ( legendLabel )
            legendLabel->setChecked(false);
        items.at(i)->setVisible(false);
        if(items[i] == m_pQwtPlot->infoToItem(itemInfo))
        {
            GetMaxValue(i);
            m_pQwtPlot->setAxisScale(QwtPlot::yLeft,0.0,m_dArrMax + m_dArrMax/10);
            m_pQwtPlot->setAxisTitle(QwtPlot::yLeft,tr("%1  ( %2 )").arg(vecTestData.at(i)->GetItem()).arg(vecTestData.at(i)->GetUnit()));

            const QVariant itemInfoTemp = m_pQwtPlot->itemToInfo( items[i] );
            QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel *>( m_pLegend->legendWidget( itemInfoTemp ) );
            if ( legendLabel )
                legendLabel->setChecked(true);
            items.at(i)->setVisible(true);
        }
    }

//    QwtPlotItem *plotItem = m_pQwtPlot->infoToItem(itemInfo);
//    if ( plotItem )
//        plotItem->setVisible( on );
}

void CPlotLines::SetValue(QVector<CTestData *> vecData)
{
    int iTemp = 0;
    double dRes = 0;
    double max = 0;
    const double EPSINON = 0.00001;
    QString qStrTemp;
    vecTestData = vecData;
    m_iUnite = vecTestData.at(0)->GetUnite();
    memset(m_pUnite,0,sizeof(UNITE_T));
    m_pQwtPlot->detachItems();
    for(int i=0;i<vecTestData.count();i++)
    {
        for(int j=0;j<m_iUnite;j++)
        {
            strcpy(m_pUnite->pro[j].item,vecTestData.at(i)->GetItem().toLocal8Bit().data());
            qStrTemp = vecTestData.at(i)->GetResult();
            if(qStrTemp.mid(0,1).compare(tr("<"))==0)
            {
                qStrTemp.remove(0,1);
                dRes = qStrTemp.toDouble() - (qStrTemp.toDouble())/10;
                if((max-qStrTemp.toDouble())<EPSINON)
                {
                    max = dRes;
                }
                m_pUnite->pro[j].result[iTemp] = dRes;
            }
            else if(qStrTemp.mid(0,1).compare(tr(">"))==0)
            {
                qStrTemp.remove(0,1);
                dRes = qStrTemp.toDouble() + (qStrTemp.toDouble())/10;
                if((max-qStrTemp.toDouble())<EPSINON)
                {
                    max = dRes;
                }
                m_pUnite->pro[j].result[iTemp] = dRes;
            }
            else
            {
                m_pUnite->pro[j].result[iTemp] = qStrTemp.toDouble();
            }
            if((max-qStrTemp.toDouble())<EPSINON)
            {
                max = qStrTemp.toDouble();
            }
            strcpy(m_pUnite->pro[j].unit,vecTestData.at(i)->GetUnit().toLocal8Bit().data());
            if(m_iUnite>1 && j!=(m_iUnite-1))
            {
                i++;
            }
        }
        iTemp++;
    }
    m_dMax = max;
    qDebug()<<"CPlotLines End SetValue.";
}

void CPlotLines::GetSpots(int iIndex)
{
    const double EPSINON = 0.00001;
    m_PolygonF.clear();
    for(int i=0;i<vecTestData.count()/vecTestData.at(0)->GetUnite();i++)
    {
        if((m_pUnite->pro[iIndex].result[i]-0)>EPSINON)
        {
            m_PolygonF<<QPointF(i+1,m_pUnite->pro[iIndex].result[i]);
        }
    }
}

double CPlotLines::GetMaxValue(int iIndex)
{
    const double EPSINON = 0.00001;
    double max = m_pUnite->pro[iIndex].result[0];
    m_PolygonF.clear();
    for(int i=0;i<vecTestData.count()/vecTestData.at(0)->GetUnite();i++)
    {
        if((max-m_pUnite->pro[iIndex].result[i]) < EPSINON)
        {
            max = m_pUnite->pro[iIndex].result[i];
        }
    }
    m_dArrMax = max;
    return max;
}

void CPlotLines::BtReturnClicked()
{
    emit sigperform(CURETRACE_WG);
}

void CPlotLines::mousePressEvent(QMouseEvent * e)
{
    emit sigperform(CURETRACE_WG);
    e->accept();
}


