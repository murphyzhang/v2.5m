#include "xdrawinfowidget.h"
#include "common/sysinfo.h"
#include "common/configurate.h"
#include <QFont>
#include <QDebug>


XDrawInfoWidget::XDrawInfoWidget(QWidget *parent): QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0, 0, 420, 80);

//    QPalette pal = palette();
//    pal.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0x00));
//    this->setPalette(pal);
//    this->setStyleSheet("color:rgb(255, 255, 255)");

    m_view = new QGraphicsView(this);
    m_view->setGeometry(this->rect());
    //m_view->setPalette(pal);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setStyleSheet("background: transparent;border:0px");

    m_scene = new QGraphicsScene;
    m_scene->setSceneRect(this->rect());
    m_view->setScene(m_scene);
    //m_scene->setPalette(pal);
    m_scene->clear();

    QPen pen(Qt::white,6);
    m_scene->addRect(3,3,420-6,80-6,pen);

#if 0
    QFont font("wenquanyi", 12, 50, false);

    m_intimename = m_scene->addSimpleText(tr("插卡时间:"),font);
    m_intimename->setFont(font);
    m_intimename->setBrush(QBrush(Qt::white));
    m_intimename->setPos(10,10);

    m_dttimename = m_scene->addSimpleText(tr("测试时间:"),font);
    m_dttimename->setFont(font);
    m_dttimename->setBrush(QBrush(Qt::white));
    m_dttimename->setPos(10,40);

    m_statename = m_scene->addSimpleText(tr("状态:"),font);
    m_statename->setFont(font);
    m_statename->setBrush(QBrush(Qt::white));
    m_statename->setPos(250,10);

    m_storename = m_scene->addSimpleText(tr("存储:"),font);
    m_storename->setFont(font);
    m_storename->setBrush(QBrush(Qt::white));
    m_storename->setPos(250,40);

    m_intime = m_scene->addSimpleText("",font);
    m_intime->setFont(font);
    m_intime->setBrush(QBrush(Qt::white));
    m_intime->setPos(110,10);

    m_dttime = m_scene->addSimpleText("",font);
    m_dttime->setFont(font);
    m_dttime->setBrush(QBrush(Qt::white));
    m_dttime->setPos(110,40);

    m_state = m_scene->addSimpleText("",font);
    m_state->setFont(font);
    m_state->setBrush(QBrush(Qt::red));
    m_state->setPos(310,10);

    m_store = m_scene->addSimpleText("",font);
    m_store->setFont(font);
    m_store->setBrush(QBrush(Qt::white));
    m_store->setPos(310,40);

    m_note1 = m_scene->addSimpleText("",font);
    m_note1->setFont(font);
    m_note1->setBrush(QBrush(Qt::white));
    m_note1->setPos(70,10);

    m_updatedname = m_scene->addSimpleText(tr("修改时间:"),font);
    m_updatedname->setFont(font);
    m_updatedname->setBrush(QBrush(Qt::white));
    m_updatedname->setPos(10,40);

    m_note2 = m_scene->addSimpleText("",font);
    m_note2->setFont(font);
    m_note2->setBrush(QBrush(Qt::white));
    m_note2->setPos(110,40);

    m_notename = m_scene->addSimpleText(tr("批号:"),font);
    m_notename->setFont(font);
    m_notename->setBrush(QBrush(Qt::white));
    m_notename->setPos(10,10);
#else

    m_intimename = new XLabel(this);
    m_dttimename = new XLabel(this);
    m_statename = new XLabel(this);
    m_storename = new XLabel(this);
    m_intime = new XLabel(this);
    m_dttime = new XLabel(this);
    m_state = new XLabel(this);
    m_store = new XLabel(this);
    m_note1 = new XLabel(this);
    m_updatedname = new XLabel(this);
    m_note2 = new XLabel(this);
    m_notename = new XLabel(this);

    m_intimename->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_dttimename->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_statename->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_storename->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_intime->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_dttime->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_state->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_store->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_note1->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_updatedname->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_note2->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_notename->setStyleSheet(FONTFAMILY + FONT_SIZE_3);

    m_intimename->setText(tr("Insert:"));
    m_dttimename->setText(tr("Test time:"));
    m_statename->setText(tr("State:"));
    m_storename->setText(tr("Store:"));
    m_intime->setText("");
    m_dttime->setText("");
    m_state->setText("");
    m_store->setText("");
    m_note1->setText("");
    m_updatedname->setText(tr("Date modified:"));
    m_note2->setText("");
    m_notename->setText(tr("Lot No.:"));

    m_intimename->move(10,10);
    m_dttimename->move(10,40);
    m_statename->move(230,10);
    m_storename->move(230,40);
    m_intime->move(110,10);
    m_dttime->move(110,40);
    m_state->move(290,10);
    m_store->move(290,40);
    m_note1->move(70,10);
    m_updatedname->move(10,40);
    m_note2->move(110,40);
    m_notename->move(10,10);

    connect(m_intimename, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_dttimename, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_statename, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_storename, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_intime, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_dttime, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_state, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_store, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_note1, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_updatedname, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_note2, SIGNAL(clicked()), this, SLOT(onClick()));
    connect(m_notename, SIGNAL(clicked()), this, SLOT(onClick()));
#endif

#if 0
    m_curmode = 0;
    set_showinfo();
#else
    set_shownote();
    m_curmode = 1;
#endif

    m_bShowBatch = true;  // 默认显示批号

//    qDebug() << "XDrawInfoWidget() done";
}

void XDrawInfoWidget::showEvent(QShowEvent *)
{
    m_bShowBatch = CSysInfo::get_cfg(CFG_QT_TWC).toBool();

    if (m_bShowBatch)
        m_notename->setText(tr("ID："));
    else
        m_notename->setText(tr("Lot:"));
}

void XDrawInfoWidget::onClick()
{
    mousePressEvent(NULL);
}

void XDrawInfoWidget::mousePressEvent(QMouseEvent *e)
{
    if(m_curmode)
    {
        set_showinfo();
        m_curmode = 0;
    }
    else
    {
        set_shownote();
        m_curmode = 1;
    }
}

void XDrawInfoWidget::set_intimetext(QString text)
{
    m_intime->setText(text);
}

void XDrawInfoWidget::set_dttimetext(QString text)
{
    m_dttime->setText(text);
}

void XDrawInfoWidget::set_statetext(QString text)
{
    m_state->setText(text);
}

void XDrawInfoWidget::set_storetext(QString text)
{
    m_store->setText(text);
}

void XDrawInfoWidget::cleartext()
{
    m_intime->setText("");
    m_dttime->setText("");
    m_state->setText("");
    m_store->setText("");

    m_note1->setText("");
}

void XDrawInfoWidget::set_showinfo()
{
    m_intimename->show();
    m_dttimename->show();
    m_statename->show();
    m_storename->show();

    m_intime->show();
    m_dttime->show();
    m_state->show();
    m_store->show();
    m_notename->hide();
    m_note1->hide();
    m_updatedname->hide();
    m_note2->hide();
}

void XDrawInfoWidget::set_shownote()
{
    m_intimename->hide();
    m_dttimename->hide();
    m_statename->hide();
    m_storename->hide();

    m_intime->hide();
    m_dttime->hide();
    m_state->hide();
    m_store->hide();
    m_notename->show();
    m_note1->show();
    m_updatedname->hide();
    m_note2->hide();
}

void XDrawInfoWidget::set_notetext(QString text)
{
    m_note1->setText(text.trimmed().left(64));

    if (m_note1->isVisible())
    {
        m_note1->hide();
        m_note1->show();
    }
}

void XDrawInfoWidget::set_updated(QString text)
{
    m_note2->setText(text);

    if (m_note2->isVisible())
    {
        m_note2->hide();
    }
}

void XDrawInfoWidget::UpdateID(QString qStrID)
{
    m_note1->setText(qStrID);
}

void XDrawInfoWidget::ClearID()
{
    m_note1->setText("");
}


