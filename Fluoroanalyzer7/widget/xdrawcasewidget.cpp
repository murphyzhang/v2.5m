#include "xdrawcasewidget.h"
#include <QDebug>

xDrawCaseWidget::xDrawCaseWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0, 0, THIS_WIDTH, THIS_HEIGTH);

//    this->setAutoFillBackground(true);
//    this->setPalette(QPalette(QColor(244,164,96)));

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0x00));
    this->setPalette(pal);
    this->setStyleSheet("color:rgb(255, 255, 255)");

    m_view = new QGraphicsView(this);
    m_view->setGeometry(this->rect());
    //m_view->setPalette(pal);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setStyleSheet("background: transparent;border:0px");
    /**/
    m_scene = new QGraphicsScene;
    m_scene->setSceneRect(this->rect());
    m_view->setScene(m_scene);

    m_scene->clear();
    QPen pen(Qt::white,6);
    m_scene->addRect(3,3,THIS_WIDTH-6,THIS_HEIGTH-6,pen);
    pen.setWidth(4);
    m_scene->addRect(20,2,THIS_WIDTH-20*2,THIS_HEIGTH-4,pen);
    pen.setWidth(1);

    for(int i=0;i<14;i++)
    {
        m_case[13-i] = m_scene->addRect(26+18*i,12,14,56,pen,QBrush(Qt::white));
    }



    m_detettimer = new QTimer();
    connect(m_detettimer, SIGNAL(timeout()), this, SLOT(drawdetet()));
    /*
    QRadialGradient radial(7,0,7);
    radial.setColorAt(0,Qt::yellow);
    radial.setColorAt(1,Qt::white);
    m_detet = m_scene->addLine(28,23,36,23,QPen(QBrush(radial),6));
    */
    m_facula.load(QString(X_BGPIC_PATH) + QString("facula.png"));
    m_detet = m_scene->addPixmap(m_facula);
    m_detet->setOffset(26,15);
    m_detet->hide();

    m_incardtimer = new QTimer();
    connect(m_incardtimer, SIGNAL(timeout()), this, SLOT(drawincard()));
    m_incard = m_scene->addRect(260,66,14,2,pen,QBrush(Qt::blue));
    m_incard->hide();

    m_sacnqrtimer = new QTimer();
    connect(m_sacnqrtimer, SIGNAL(timeout()), this, SLOT(drawscanqr()));
    m_scanqr = m_scene->addRect(260,12,14,56,pen,QBrush(QColor(0,0xff,0xff)));
    m_scanqr->hide();

    pen.setWidth(1);
    pen.setColor(Qt::red);
    m_selet = m_scene->addRect(26,12,14,56,pen);
    m_selet->setVisible(false);

}

void xDrawCaseWidget::setnposcolor(int npos, QColor color)
{
    if(npos < 14)
    {
        //m_case[13-npos]->setPen(QPen(color,1));
        m_case[npos]->setBrush(QBrush(color));
    }
    //this->update();
}

void xDrawCaseWidget::setleftnstep(int nstep)
{
    if(nstep > 0)
    {
        for(int i=0;i<nstep;i++)
        {
            for(int j=13;j>0;j--)
            {
                //m_case[j]->setPen(m_case[j+1]->pen());
                m_case[j]->setBrush(m_case[j-1]->brush());
            }
            m_case[0]->setBrush(QBrush(Qt::white));
        }
    }
    this->update();
}

void xDrawCaseWidget::setdetetstate(bool start)
{
    if(start)
    {
        m_detet->show();
        m_detettimer->start(200);
    }
    else
    {
        m_detet->hide();
        m_detettimer->stop();
    }
}

void xDrawCaseWidget::drawdetet()
{
    QPointF pf(m_detet->offset());
    pf.setY(m_detet->offset().ry()+1);
    if(pf.ry()>(THIS_HEIGTH-30)) pf.setY(15);
    m_detet->setOffset(pf);
    this->update();
}

void xDrawCaseWidget::setincardstate(bool start)
{
    if(start)
    {
        QRectF rf = m_incard->rect();
        rf.setY(66);
        m_incard->setRect(rf);
        m_incard->show();
        m_incardtimer->start(200);
    }
    else
    {
        m_incard->hide();
        m_incardtimer->stop();
    }
}

void xDrawCaseWidget::drawincard()
{
    QRectF rf = m_incard->rect();
    if(rf.y()<=12)
    {
        rf.setY(66);
    }
    rf.setY(rf.y()-2);
    m_incard->setRect(rf);
    if(rf.y()<=12)
    {
        setincardstate(false);
    }
    this->update();
}

void xDrawCaseWidget::setscanqrstate(bool start)
{
    if(start)
    {
        m_sacnqrtimer->start(200);
        //m_scanqr->show();
        m_scanqr->setVisible(true);
    }
    else
    {
        m_sacnqrtimer->stop();
        //m_scanqr->hide();
        m_scanqr->setVisible(false);
    }
}

void xDrawCaseWidget::drawscanqr()
{
    if(m_scanqr->isVisible())
        m_scanqr->setVisible(false);
    else
        m_scanqr->setVisible(true);
}

void xDrawCaseWidget::updatecase(XTestCard *card[])
{
    for(int i = 0; i < WAITTEST_MAX; i++)
    {
        if(card[i] != NULL)
        {
            if(!card[i]->getneedtest()) //不测试
            {
                setnposcolor(i,Qt::lightGray);
            }
            else
            {
                if(card[i]->canscan()) //还未扫描
                {
                    setnposcolor(i,Qt::blue);
                }
                else if(!card[i]->hasbatch()) //扫不到条码
                {
                    setnposcolor(i,Qt::blue);
                }
                else if(!card[i]->hascode()) //没有信息
                {
                    setnposcolor(i,Qt::yellow);
                }
                else if(card[i]->gettestend()) //测试结束
                {
                    setnposcolor(i,Qt::gray);
                }
                else    //正常卡
                {
                    setnposcolor(i,Qt::green);
                }
            }
        }
        else //无卡
        {
            setnposcolor(i,Qt::white);
        }
    }

    this->update();
}

void xDrawCaseWidget::setcardselet(int ncard)
{
    if((ncard >=0) && (ncard <WAITTEST_MAX))
    {
        m_selet->setPos(18*(13-ncard),0);
        m_selet->setVisible(true);
    }
    else
        m_selet->setVisible(false);
}




