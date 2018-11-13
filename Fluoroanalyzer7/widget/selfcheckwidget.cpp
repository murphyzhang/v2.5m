#include "selfcheckwidget.h"
#include "common/configurate.h"


CSelfCheckWidget::CSelfCheckWidget(QWidget *parent): CBaseFunWidget(parent)
{
    m_title = new QLabel(tr("Self-checking, please wait......"),this);
    m_title->setStyleSheet("color:rgb(255, 255, 255);" + FONTFAMILY + FONT_SIZE_1);
    m_title->setGeometry(100,70,500,40);

    movie = new QMovie(QString(X_BGPIC_PATH) + QString("wait.gif"));


    /*m_strlist<<tr("进卡模块检测...")\
            <<tr("测试模块检测...")\
           <<tr("条码模块检测...")\
          <<tr("通信模块检测...")\
         <<tr("传动模块检测...");
   */
    m_strlist<<tr("Insertion module detection...")\
            <<tr("Test module detection...")\
           <<tr("Bar code module detection...")\
          <<tr("Communication module detection...")\
         <<tr("Transmission module detection...");
   /* m_strinfo<<("进卡口有异物")\
            <<("通信线路异常")\
           <<("通信线路异常")\
          <<("通信线路异常")\
         <<("无法定位卡槽"); 
		 */
	m_strinfo<<("There are sth. wrong at the entrance")\
            <<("Communication module exception")\
           <<("Communication module exception")\
          <<("Communication module exception")\
         <<("Unable to locate card slot");

    for(int i = 0; i < CHECK_NUM; i++)
    {
        m_checkpro[i].m_namestr = m_strlist.at(i);
        m_checkpro[i].m_infostr = m_strinfo.at(i);
        m_checkpro[i].m_checkname = new QLabel(this);
        m_checkpro[i].m_checkinfo = new QLabel(this);
        m_checkpro[i].m_checkicon = new QLabel(this);
        m_checkpro[i].m_checkname->setStyleSheet("color:rgb(255, 255, 255);" + FONTFAMILY + FONT_SIZE_2);
        m_checkpro[i].m_checkinfo->setStyleSheet("color:rgb(176, 23, 31);" + FONTFAMILY + FONT_SIZE_2);
        m_checkpro[i].m_checkname->setGeometry(60,130+60*i,590,40);
        m_checkpro[i].m_checkinfo->setGeometry(380,130+60*i,250,40);
        m_checkpro[i].m_checkicon->setGeometry(710,130+60*i,40,40);

        m_checkpro[i].m_checked = NOT_CHECK;
        m_checkpro[i].m_checkok = false;
    }

    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotcheckfun()));

    m_timer->start(10);
    m_candidi = true;
}

CSelfCheckWidget::~CSelfCheckWidget()
{

}


void CSelfCheckWidget::showEvent(QShowEvent *)
{
}


void CSelfCheckWidget::slotcheckfun()
{
    m_timer->stop();
/*
    int it;
    for(it = 0;it<CHECK_NUM;it++)
    {
        if(m_checkpro[it].m_checked != END_CHECK)
            break;
    }
    if(it < CHECK_NUM)
    {
        if(m_checkpro[it].m_checked == NOT_CHECK)
        {
            m_checkpro[it].m_checkname->setText(m_checkpro[it].m_namestr);


            if(it == 0)
            {
                if(CCommonInterface::get_incard()->start_check())
                {
                    m_checkpro[it].m_checkicon->setMovie(movie);
                    movie->start();
                    m_checkpro[it].m_checked = WAT_CHECK;
                }
            }
            else if(it == 1)
            {
                if(CCommonInterface::get_conveyor()->start_check())
                {
                    m_checkpro[it].m_checkicon->setMovie(movie);
                    movie->start();
                    m_checkpro[it].m_checked = WAT_CHECK;
                }
            }
            else if(it == 2)
            {
                if(CCommonInterface::get_detection()->start_check())
                {
                    m_checkpro[it].m_checkicon->setMovie(movie);
                    movie->start();
                    m_checkpro[it].m_checked = WAT_CHECK;
                }
            }
            m_timer->start(10);
        }
        else if(m_checkpro[it].m_checked == WAT_CHECK)
        {

            if(it == 0)
            {
                if(CCommonInterface::get_incard()->isend_check())
                {
                    m_checkpro[it].m_checkok = CCommonInterface::get_incard()->state_check();
                    m_checkpro[it].m_checked = ING_CHECK;
                }
            }
            else if(it == 1)
            {
                if(CCommonInterface::get_conveyor()->isend_check())
                {
                    m_checkpro[it].m_checkok = CCommonInterface::get_conveyor()->state_check();
                    m_checkpro[it].m_checked = ING_CHECK;
                }
            }
            else if(it == 2)
            {
                if(CCommonInterface::get_detection()->isend_check())
                {
                    m_checkpro[it].m_checkok = CCommonInterface::get_detection()->state_check();
                    m_checkpro[it].m_checked = ING_CHECK;
                }
            }
            m_timer->start(10);
        }
        else if(m_checkpro[it].m_checked == ING_CHECK)
        {
            QPixmap pic;
            if(m_checkpro[it].m_checkok)
            {
                pic.load(QString(BACKGROUND_PATH) + QString("succ.png"));
            }
            else
            {
                pic.load(QString(BACKGROUND_PATH) + QString("fail.png"));
            }
            m_checkpro[it].m_checkicon->setPixmap(pic.scaled(40,40));
            m_checkpro[it].m_checked = END_CHECK;
            m_timer->start(10);
        }

    }
*/
    int it;
    for(it = 0; it < 4; it++)
    {
        if(it < CHECK_NUM)
        {
            if(m_checkpro[it].m_checked == NOT_CHECK)
            {
                m_checkpro[it].m_checkname->setText(m_checkpro[it].m_namestr);

                if(it == 0)//进卡模块检测
                {
                    if(CCommonInterface::get_incard()->start_check())
                    {
                        m_checkpro[it].m_checkicon->setMovie(movie);
                        movie->start();
                        m_checkpro[it].m_checked = WAT_CHECK;
                    }
                }
                else if(it == 1)//430测试模块检测
                {
                    if(CI::get_detection()->start_check())
                    {
                        m_checkpro[it].m_checkicon->setMovie(movie);
                        movie->start();
                        m_checkpro[it].m_checked = WAT_CHECK;

                        sleep(1);
                        CI::get_detection()->updateCardSlotVerify();
                    }
                }
                else if(it == 2)//条码模块检测
                {
                    if(CCommonInterface::get_qrscan()->start_check())
                    {
                        m_checkpro[it].m_checkicon->setMovie(movie);
                        movie->start();
                        m_checkpro[it].m_checked = WAT_CHECK;
                    }
                }
                else if(it == 3)//通信模块检测
                {
                    if(1)
                    {
                        m_checkpro[it].m_checkicon->setMovie(movie);
                        movie->start();
                        m_checkpro[it].m_checked = WAT_CHECK;
                    }
                }
            }
            else if(m_checkpro[it].m_checked == WAT_CHECK)
            {
                if(it == 0)
                {
                    if(CCommonInterface::get_incard()->isend_check())
                    {
                        m_checkpro[it].m_checkok = CCommonInterface::get_incard()->state_check();
                        m_checkpro[it].m_checked = ING_CHECK;
                    }
                }
                else if(it == 1)
                {
                    if(CI::get_detection()->isend_check() && CI::get_detection()->isCardSlotVerify())
                    {
                        m_checkpro[it].m_checkok = CCommonInterface::get_detection()->state_check();
                        m_checkpro[it].m_checked = ING_CHECK;
                    }
                }
                else if(it == 2)
                {
                    if(CCommonInterface::get_qrscan()->isend_check())
                    {
                        m_checkpro[it].m_checkok = CCommonInterface::get_qrscan()->state_check();
                        m_checkpro[it].m_checked = ING_CHECK;
                    }
                }
                else if(it == 3)
                {
                    if(1)
                    {
                        m_checkpro[it].m_checkok = true;
                        m_checkpro[it].m_checked = ING_CHECK;
                    }
                }
            }
            else if(m_checkpro[it].m_checked == ING_CHECK)
            {
                QPixmap pic;
                if(m_checkpro[it].m_checkok)
                {
                    pic.load(QString(X_BGPIC_PATH) + QString("succ.png"));
                }
                else
                {
                    pic.load(QString(X_BGPIC_PATH) + QString("fail.png"));
                    m_checkpro[it].m_checkinfo->setText(m_checkpro[it].m_infostr);
                }
                m_checkpro[it].m_checkicon->setPixmap(pic.scaled(40,40));
                m_checkpro[it].m_checked = END_CHECK;
            }
            else
            {

            }

        }
    }

    if(m_checkpro[1].m_checked == END_CHECK)//1和4不能同时测
    {
        if(m_checkpro[4].m_checked == NOT_CHECK)
        {
            m_checkpro[4].m_checkname->setText(m_checkpro[4].m_namestr);

            if(CCommonInterface::get_conveyor()->start_check())//卡槽皮带电机
            {
                m_checkpro[4].m_checkicon->setMovie(movie);
                movie->start();
                m_checkpro[4].m_checked = WAT_CHECK;
            }
        }
        else if(m_checkpro[4].m_checked == WAT_CHECK)
        {
            if(CCommonInterface::get_conveyor()->isend_check())
            {
                m_checkpro[4].m_checkok = CCommonInterface::get_conveyor()->state_check();
                m_checkpro[4].m_checked = ING_CHECK;
            }
        }
        else if(m_checkpro[4].m_checked == ING_CHECK)
        {
            QPixmap pic;
            if(m_checkpro[4].m_checkok)
            {
                pic.load(QString(X_BGPIC_PATH) + QString("succ.png"));
            }
            else
            {
                pic.load(QString(X_BGPIC_PATH) + QString("fail.png"));
                m_checkpro[4].m_checkinfo->setText(m_checkpro[4].m_infostr);
            }
            m_checkpro[4].m_checkicon->setPixmap(pic.scaled(40,40));
            m_checkpro[4].m_checked = END_CHECK;
        }
        else
        {
        }
    }

    if(m_checkpro[0].m_checked != END_CHECK ||\
            m_checkpro[1].m_checked != END_CHECK ||\
            m_checkpro[2].m_checked != END_CHECK ||\
            m_checkpro[3].m_checked != END_CHECK ||\
            m_checkpro[4].m_checked != END_CHECK)
    {
        m_timer->start(10);
    }
    else
    {
        if(m_checkpro[1].m_checkok == false)
        {
            m_timer->start(10);
            m_checkpro[1].m_checkok = CCommonInterface::get_detection()->state_check();
            if(m_candidi)
            {
                m_candidi = false;
                CCommonInterface::get_optbuzzer()->ringdidi();
            }
        }
        else
        {
            CCommonInterface::get_optbuzzer()->ringdiii();
            emit sigperform(WAIT_WG);
        }
    }
}
