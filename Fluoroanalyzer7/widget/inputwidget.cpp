#include "inputwidget.h"
#include "ui_inputwidget.h"
#include "common/configurate.h"

XInPutWidget::XInPutWidget(QWidget *parent) :
    QWidget(parent),
    form(new Ui::XInPutWidget)
{
    form->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),this, SLOT(saveFocusWidget(QWidget*,QWidget*)));
    this->setStyleSheet("color:rgb(0, 0, 0);" + FONTFAMILY + FONT_SIZE_2);
    m_pblist << form->btn0 << form->btn1 << form->btn2 << form->btn3 << form->btn4
             << form->btn5 << form->btn6 << form->btn7 << form->btn8 << form->btn9
             << form->btnDel << form->btnOK;
    for(int i=0;i<m_pblist.count();i++)
    {
        connect(m_pblist.at(i), SIGNAL(clicked()),&signalMapper, SLOT(map()));
        signalMapper.setMapping(m_pblist.at(i), m_pblist.at(i)->text());
    }

    connect(&signalMapper, SIGNAL(mapped(QString)),this, SLOT(buttonClicked(QString)));

}

XInPutWidget::~XInPutWidget()
{

}
bool XInPutWidget::event(QEvent *e)
{
    /*
    //qDebug()<<"XInPutWidget event"<<e->type();
    switch (e->type()) {

    case QEvent::WindowActivate:
        if (lastFocusedWidget)
            lastFocusedWidget->activateWindow();
        break;

    default:
        break;
    }
    */
    return QWidget::event(e);
}

void XInPutWidget::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus)) {
        //lastFocusedWidget = newFocus;
    }
}



void XInPutWidget::buttonClicked(QString text)
{
//    qDebug()<<"buttonClicked "<<text;
    emit sigPBClicked(text);
}

void XInPutWidget::xsetFocus(QWidget *newFocus)
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus)) {
        lastFocusedWidget = newFocus;
    }
}
