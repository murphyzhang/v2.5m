#include "TableWidget.h"

CTableWidget::CTableWidget(QWidget *parent) :
    QTableWidget(parent),m_nCurPos(0)
{

    // m_Pixmap.load("7back.bmp");

    //    this->setColumnCount(2);
    //    this->setRowCount(2);

    //    QPushButton * pBtn = new QPushButton("sfdsf");
    //    this->setCellWidget(0, 0, pBtn);


    //    // this->ui->tableWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    //   pBtn->setWindowOpacity(1.0);

    ////    QPalette pal = palette();
    ////    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    ////    pBtn->setPalette(pal);
    //   this->setColumnWidth(1, 100);

    this->verticalScrollBar()->setFixedWidth(50);
    this->setSelectionMode(QAbstractItemView::NoSelection);

}




void CTableWidget::paintEvent(QPaintEvent *e)
{
    //    QPainter paint(this);
    //    paint.drawPixmap(0, 0, this->width(), this->height(), m_Pixmap);
    return QTableWidget::paintEvent(e);
}

void CTableWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    return QTableWidget::mouseDoubleClickEvent(event);
}

void CTableWidget::mousePressEvent(QMouseEvent *event)
{

    //    QTableView：：verticalScrollBar()->setSliderPosition()；  //设置当前滑动条的位置
    //    QTableView：：verticalScrollBar()->maximum();             //滑动条能移动的最大位置
    //    QTableView：：verticalScrollBar()->value();                   //获得当前滑动条的位置
    int nPos = verticalScrollBar()->sliderPosition();
//    qDebug() << "mouse y=" << event->y() << "  this->y()=" << y() << height();
    if(event->y() > height()/2)
    {
//        if(nPos < 14)
//            this->verticalScrollBar()->setSliderPosition(nPos + 1);
        if(++m_nCurPos >= 5) m_nCurPos = 5;
        this->verticalScrollBar()->setSliderPosition(m_nCurPos);
    }
    else
    {
//        if(nPos > 0)
//            this->verticalScrollBar()->setSliderPosition(nPos - 1);
        if(--m_nCurPos < 0) m_nCurPos = 0;
        this->verticalScrollBar()->setSliderPosition(m_nCurPos);
    }

//    qDebug() << "CTableWidget::mousePressEvent(QMouseEvent *event) nPos="<< nPos << "m_nCurPos="<<m_nCurPos;
    this->update();

    return QTableWidget::mousePressEvent(event);
}
