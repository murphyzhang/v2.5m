#include "MessageBox.h"

CMessageBox::CMessageBox(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("font: 14pt wenquanyi");

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0x8b,0x8b,0xc5));
    this->setPalette(pal);
    this->setStyleSheet("color:white;");

    this->setGeometry(200,120,400,200);

    QPalette palette;
    palette.setColor(QPalette::WindowText,Qt::white);

    m_pLbPrompt = new QLabel(this);
    m_pLbPrompt->setGeometry(20,20,360,110);
    m_pLbPrompt->setAlignment(Qt::AlignCenter);
    m_pLbPrompt->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    m_pLbPrompt->setPalette(palette);
    m_pLbPrompt->setStyleSheet("color:white;");

    m_pBtConfirm = new XButton(this);
    m_pBtConfirm->setGeometry(80,150,100,40);
    m_pBtConfirm->setText(tr("Confirm"));
    m_pLbPrompt->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    connect(m_pBtConfirm,SIGNAL(sigbtnclick()),this,SLOT(BtConfirmClicked()));

    m_pBtCancel = new XButton(this);
    m_pBtCancel->setGeometry(220,150,100,40);
    m_pBtCancel->setText(tr("Cancel"));
    m_pLbPrompt->setStyleSheet(FONTFAMILY + FONT_SIZE_6);
    connect(m_pBtCancel,SIGNAL(sigbtnclick()),this,SLOT(BtCancelClicked()));
}

CMessageBox::~CMessageBox()
{
    //
}

void CMessageBox::SetType(int iType,QString qStrMessage)
{
    m_pLbPrompt->setText(qStrMessage);
    if(iType == BT_CONFIRM)
    {
        m_pBtCancel->hide();
        m_pBtConfirm->move(150,150);
    }
    else if(iType == BT_CONFIRM_CANCEL)
    {
        m_pBtCancel->show();
        m_pBtConfirm->move(80,150);
    }
}

void CMessageBox::BtConfirmClicked()
{
    emit SigConfirm();
    this->close();
}

void CMessageBox::BtCancelClicked()
{
    emit SigCancel();
    this->close();
}


