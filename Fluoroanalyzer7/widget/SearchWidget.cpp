#include "SearchWidget.h"

CSearchWidget::CSearchWidget(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(180, 120, 440, 200);

    InitUI();

    m_pSyszux = new SyszuxPinyin();
    connect(m_pSyszux,SIGNAL(sendPinyin(QString)),this,SLOT(ConfirmString(QString)));
}

CSearchWidget::~CSearchWidget()
{
    //
}

void CSearchWidget::InitUI()
{
    m_pCbbKey = new QComboBox(this);
    m_pCbbKey->setGeometry(60,50,100,40);
    m_pCbbKey->insertItem(DATE_SEARCH,tr("Date"));
    m_pCbbKey->insertItem(ID_SEARCH,tr("ID"));
    m_pCbbKey->insertItem(NAME_SEARCH,tr("Name"));
    m_pCbbKey->setStyleSheet(FONTFAMILY + FONT_SIZE_4);

    m_pLineEdit = new CMyLineEdit(this);
    m_pLineEdit->SetMyGeometry(160,50,200,40);
    m_pLineEdit->setStyleSheet(FONTFAMILY + FONT_SIZE_4);
    connect(m_pLineEdit,SIGNAL(SendText(QString)),this,SLOT(LineEditClicked(QString)));

    m_pBtQuery = new XButton(this);
    m_pBtQuery->setText(tr("Search"));
    m_pBtQuery->setGeometry(110,110,100,40);
    connect(m_pBtQuery,SIGNAL(sigbtnclick()),this,SLOT(BtQueryClicked()));

    m_pBtCancel = new XButton(this);
    m_pBtCancel->setText(tr("Back"));
    m_pBtCancel->setGeometry(230,110,100,40);
    connect(m_pBtCancel,SIGNAL(sigbtnclick()),this,SLOT(BtCancelClicked()));

    m_pInputPanel = new XInPutWidget;
    m_pInputPanel->move(this->x()+80,this->y()+52);
    m_pInputPanel->hide();
    /* 并将characterGenerated和sendCharacter关联，当用户按键后，则继续由sendCharacter向目标Widget */
    connect(m_pInputPanel, SIGNAL(sigPBClicked(QString)), this,SLOT(slotTextActer(QString)));
}

void CSearchWidget::BtQueryClicked()
{
    if(m_pLineEdit->text().trimmed().isEmpty())
    {
        return;
    }
    emit EndInput(m_pLineEdit->text().trimmed(),m_pCbbKey->currentIndex());
    this->close();
}

void CSearchWidget::BtCancelClicked()
{
    emit sigCancel();
    this->close();
}

void CSearchWidget::LineEditClicked(QString qStrText)
{
    if(m_pCbbKey->currentIndex() == 0 || m_pCbbKey->currentIndex() == 1)
    {
        m_pSyszux->SetInputMethodEng();
    }
    else if(m_pCbbKey->currentIndex() == 2)
    {
        m_pSyszux->SetInputMethodEng();
    }

    m_pSyszux->lineEdit_window->setText(qStrText);
    m_pSyszux->show();
}

void CSearchWidget::slotTextActer(QString text)
{
    if(text == "DEL")
    {
        m_pLineEdit->setText(m_pLineEdit->text().left(m_pLineEdit->text().length() - 1));
//        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
//        QApplication::sendEvent(this, &keyPress);
    }
    else if(text == "OK")
    {
        m_pInputPanel->hide();
        m_pLineEdit->deselect();
    }
    else
    {
        m_pLineEdit->setText(m_pLineEdit->text()+text);
    }
}

void CSearchWidget::ConfirmString(QString qStrText)
{
    m_pLineEdit->setText(qStrText);
}

void CSearchWidget::ClearContent()
{
    m_pLineEdit->clear();
}

