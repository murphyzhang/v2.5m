#include "MyLineEdit.h"

CMyLineEdit::CMyLineEdit(QWidget *parent)
   : QLineEdit(parent)
{
    setTextMargins(0,0,this->height(),0);
    //setStyleSheet("background-image:url(:/Image/button/clear.png);border:1px solid #536874;");

    QCompleter *completer = new QCompleter(this);//自动补全
    m_pListModel = new QStringListModel(valueList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setModel(m_pListModel);
    this->setCompleter(completer);
    this->setFocusPolicy(Qt::ClickFocus);

    m_pPushButton = new QPushButton(this);
    m_pPushButton->setFocusPolicy(Qt::NoFocus);
    m_pPushButton->setCursor(Qt::PointingHandCursor);
    m_pPushButton->resize(this->height()-4,this->height()-4);
    m_pPushButton->move(this->x()+this->width()-this->height()+2,2);
    m_pPushButton->setStyleSheet("QPushButton{background-color:transparent;border:0px;}");
    m_pPushButton->setIcon(QIcon(":/image/button/clear.png"));
    m_pPushButton->setIconSize(QSize(this->height()-4,this->height()-4));
    m_pPushButton->setFocusPolicy(Qt::NoFocus);
    connect(m_pPushButton,SIGNAL(clicked()),this,SLOT(BtClicked()));
}

void CMyLineEdit::AddValue(const QString &value)
{
    valueList.append(value);
    m_pListModel->setStringList(valueList);
}

void CMyLineEdit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //触发clicked信号
        emit SendText(this->text());
    }
    //将该事件传给父类处理
    QLineEdit::mousePressEvent(event);
}

void CMyLineEdit::EditComplete()
{
    QString text = this->text();
    if(QString::compare(text, QString("")) != 0)
    {
        bool flag = valueList.contains(text, Qt::CaseInsensitive);
        if(!flag)
        {
            AddValue(text);
        }
    }
}

void CMyLineEdit::SetMyGeometry(int iPosX, int iPosY, int iWidth, int iHeight)
{
    this->move(iPosX,iPosY);
    this->resize(iWidth,iHeight);

    m_pPushButton->resize(iHeight-4,iHeight-4);
    m_pPushButton->move(iWidth-iHeight+2,2);
    m_pPushButton->setIconSize(QSize(iHeight-4,iHeight-4));

    this->SetMyTextMargins(0,0,iHeight,0);
}

void CMyLineEdit::SetMyTextMargins(int iLeft, int iTop, int iRight, int iBottom)
{
    setTextMargins(iLeft,iTop,iRight,iBottom);
}

void CMyLineEdit::BtClicked()
{
    this->clear();
}

