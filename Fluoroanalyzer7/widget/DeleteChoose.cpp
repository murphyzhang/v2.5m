#include "DeleteChoose.h"

CDeleteChoose::CDeleteChoose(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(250, 140, 300, 160);
    this->setStyleSheet("font: 14pt wenquanyi");

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0x8b,0x8b,0xc5));
    this->setPalette(pal);

    m_pBtDelCurrent = new XButton(this);
    //m_pBtDelCurrent->setText(tr("删除当前"));
    m_pBtDelCurrent->setText(tr("Del selected"));
    m_pBtDelCurrent->setGeometry(30,40,130,30);
    m_pBtDelCurrent->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pBtDelCurrent,SIGNAL(sigbtnclick()),this,SLOT(BtDelCurrentClicked()));

    m_pBtDelAll = new XButton(this);
    //m_pBtDelAll->setText(tr("删除全部"));
    m_pBtDelAll->setText(tr("Del all"));
    m_pBtDelAll->setGeometry(180,40,90,30);
    m_pBtDelAll->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pBtDelAll,SIGNAL(sigbtnclick()),this,SLOT(BtDelAllClicked()));

    m_pBtReturn = new XButton(this);
    //m_pBtReturn->setText(tr("返回"));
    m_pBtReturn->setText(tr("Back"));
    m_pBtReturn->setGeometry(100,100,100,30);
    m_pBtReturn->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    connect(m_pBtReturn,SIGNAL(sigbtnclick()),this,SLOT(BtReturnClicked()));
}

CDeleteChoose::~CDeleteChoose()
{
    //delete ui;
}

void CDeleteChoose::BtDelCurrentClicked()
{
    this->close();
    emit DelSelect(BTDELCURRENT);
}

void CDeleteChoose::BtDelAllClicked()
{
    this->close();
    emit DelSelect(BTDELALL);
}

void CDeleteChoose::BtReturnClicked()
{
    this->close();
    emit DelSelect(BTRETURN);
}
