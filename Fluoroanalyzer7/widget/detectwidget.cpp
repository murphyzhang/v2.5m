#include "detectwidget.h"


CDetectWidget::CDetectWidget(QWidget *parent): CBaseFunWidget(parent)
{
    m_currcardnum = 0;
    m_runcheck = false;
    m_scancard = NULL;
    m_detectcard = NULL;
    m_xtabwg = new XTableWidget(this);
    m_xtabwg->setGeometry(10,130,780,300);

    connect(m_xtabwg, SIGNAL(sigclickback()), this, SLOT(slotclickback()));//返回
    connect(m_xtabwg, SIGNAL(sigclickrow(XTestCard *)), this, SLOT(slottabclick(XTestCard *)));
    connect(m_xtabwg, SIGNAL(sigclickclear()), this, SLOT(slotclickclear()));//清卡
    connect(m_xtabwg, SIGNAL(sigsetbatch(XTestCard*,QString)), this, SLOT(Slotsetbatch(XTestCard*,QString)));

    //曲线
    connect(m_xtabwg,SIGNAL(sinSetYgCurveData(QByteArray,ENUM_WIDGET)),this,SLOT(SetCurveData(QByteArray,ENUM_WIDGET)));
    connect(m_xtabwg,SIGNAL(sigUpdateID(QString)),this,SLOT(SetUpdateID(QString)));

    m_drawcase = new xDrawCaseWidget(this);
    m_drawcase->setGeometry(10,50,m_drawcase->width(),m_drawcase->height());

    m_drawinfo = new XDrawInfoWidget(this);
    m_drawinfo->setGeometry(350,50,m_drawinfo->width()+40,m_drawinfo->height());

    m_detecttime = new QTimer;
    connect(m_detecttime, SIGNAL(timeout()), this, SLOT(slotDetect()));

#if defined(DEVICE_TEST)
    m_pTimerDeviceTest = new QTimer;
    connect(m_pTimerDeviceTest, SIGNAL(timeout()), this, SLOT(slotDeviceTest()));

    m_test = false;
#endif

    connect(CI::get_incard(),     SIGNAL(sigining()),     this, SLOT(slotincard()));      //收到进卡信号
    connect(CI::get_incard(),     SIGNAL(siginsucc()),    this, SLOT(slotincardsucc()));  //收到进卡完成信号
    connect(CI::get_qrscan(),     SIGNAL(sigqrscanend()), this, SLOT(slotqrscan()));      //内部条码扫描完成
    connect(CI::get_detection(),  SIGNAL(sigstartsucc()), this, SLOT(slotdetectstart())); //开始测试
    connect(CI::get_detection(),  SIGNAL(sigrecvsucc()),  this, SLOT(slotdetectsucc()));  //收到所有荧光值

    connect(CI::get_conveyor(),   SIGNAL(sigrun1step()),  this, SLOT(Slotrun1step()));    //皮带走一格信号
    connect(CI::get_conveyor(),   SIGNAL(sigrunend()),    this, SLOT(Slotrunend()));      //皮带停止信号
    connect(CI::get_conveyor(),   SIGNAL(sigcheckover()), this, SLOT(Slotcheckover()));   //停止检测信号

    connect(CI::get_opteep(),     SIGNAL(SigNewBatch(QString)), this, SLOT(SlotWaitCode(QString)));
    connect(CI::get_datacom(),    SIGNAL(SigNewBatch(QString)), this, SLOT(SlotWaitCode(QString)));

    m_promptbox = new XPromptBox();
    m_promptbox->hide();
    connect(m_promptbox, SIGNAL(sigclickok(int)), this, SLOT(Slotpromptok(int)));
    connect(m_promptbox, SIGNAL(sigclickcancel(int)), this, SLOT(Slotpromptcs(int)));

    for(int i = 0; i < 14; i++)
    {
        m_conveeg[i] = NULL;
    }

    m_currunstop = true;
    m_bShowBatch = true;    //默认显示批号

    m_jiffyes = 0;
}

void CDetectWidget::showEvent(QShowEvent *)
{
    if(m_conveeg[INSERT_CARD_POS] == NULL) //插卡处无卡
    {
        CCommonInterface::get_incard()->set_inallow(true);//设置允许进卡
    }
    else
    {
        //        qDebug() << "CDetectWidget::showEvent().CARD";
    }

    if(!m_detecttime->isActive())
    {
        m_detecttime->start(1000);
        int curr = CDataBase::get_MaxAdss(TABLE_BODY_NAME);

        QSqlQuery query = CDataBase::select_Record(TABLE_BODY_NAME,"SERNUM",curr);
        if(query.next())
        {
            m_lastbatch = query.value(1).toString();
        }
        m_currdtime = 0;
    }

    m_bShowBatch = CSysInfo::get_cfg(CFG_QT_TWC).toBool();
}

void CDetectWidget::hideEvent(QHideEvent *)
{
    CCommonInterface::get_incard()->set_inallow(false);
    //m_detecttime->stop();
}

//void CDetectWidget::insertnewcard()
//{
//    m_currcardnum++;
//    XTestCard *newcard = new XTestCard(m_currcardnum);

//    if(newcard !=  NULL)
//    {
//        m_conveeg[INSERT_CARD_POS] = newcard;
//        m_conveeg[INSERT_CARD_POS]->setstatestr(tr("等待识别"));
//        m_xtabwg->xinsert_row(newcard);
//    }
//}


void CDetectWidget::leftshiftnstep(int nstep)
{
    if(nstep > 0)
    {
        m_currunstop = false;
        CCommonInterface::get_incard()->set_inallow(false);
        CCommonInterface::get_conveyor()->runnstep(nstep);
    }
}

//是否需要扫描
bool CDetectWidget::needtoscanqr()
{
    bool ret = false;
    if(m_conveeg[SCAN_CARD_POS] != NULL) //有卡
    {
        if(m_conveeg[SCAN_CARD_POS]->canscan()) //需要扫描
            ret = true;
    }
    return ret;
}

bool CDetectWidget::needtodetect()
{
    bool ret = false;
    if(m_conveeg[TEST_CARD_POS] != NULL) //有卡
    {
        if(m_conveeg[TEST_CARD_POS]->getneedtest())
            if(m_conveeg[TEST_CARD_POS]->cantest())
                ret = true;
    }
    return ret;
}

void CDetectWidget::updatainfo(XTestCard *card)
{
    m_drawinfo->cleartext();

    if(card != NULL)
    {
        if(card->getneedtest() && card->hascode())
        {
            if(card->gettestend())
            {
                m_drawinfo->set_dttimetext(card->getendtime().toString("hh:mm:ss"));
                m_drawinfo->set_storetext(QString::number(card->getsavepos()));
            }
        }
        m_drawinfo->set_intimetext(card->getinserttime().toString("hh:mm:ss"));
        m_drawinfo->set_statetext(card->getstatestr());

        if (m_bShowBatch)
            m_drawinfo->set_notetext(card->getnotes());//ID
        else
            m_drawinfo->set_notetext(card->getbatch());//batch

        m_drawinfo->set_updated(card->getupdatedtime().toString("yyyy-MM-dd hh:mm:ss"));// update time
    }
}

int CDetectWidget::hascandetcard()
{
    int ret=0;
    for(int idex=0;idex<=TEST_CARD_POS;idex++)
    {
        if(m_conveeg[idex] != NULL)
        {
            if(m_conveeg[idex]->getneedtest())//需要测试
            {
                if(m_conveeg[idex]->hascode())//有条码
                {
                    if(!m_conveeg[idex]->gettestend())//还没测试
                    {
                        ret++;
                    }
                }
            }
        }
    }
    return ret;
}

void CDetectWidget::clearcard()
{
    CCommonInterface::get_incard()->set_inallow(false);
    m_runcheck = true;
    m_xtabwg->setEnabled(false);

    m_promptbox->settitle(tr("Clearing the Test Cartridge, please wait."));
    m_promptbox->setnobutton();
    m_promptbox->setpromptser(3);
    m_promptbox->show();

    updatainfo(NULL);

    m_scancard = NULL;
    m_detectcard = NULL;

    m_currcardnum = 0;
    for(int idex=0;idex<=TEST_CARD_POS;idex++)
    {
        m_conveeg[idex] = NULL;
    }
    m_xtabwg->xclear();
    //leftshiftnstep(18);
    CCommonInterface::get_conveyor()->start_check();
}

void CDetectWidget::setcardbatch(XTestCard *card, QString batch)
{
    if((card != NULL) && !batch.isNull())
    {
        card->setbatch(batch);

        QSqlQuery query = CDataBase::select_Record(BATCH_BODY_NAME,"BATCH",batch);
        if(query.next())
        {
            card->setcode(query.value(2).toByteArray());
#ifdef PET_HOSPITAL //宠物医院用
            if((card->getargdecode()->getcardtype() == 1) || (card->getargdecode()->getcardtype() == 2))
            {
                card->setstatestr(tr("Waiting"));
            }
            else
            {
                card->setstatestr(tr("Illegal item"));
                card->setneedtest(false);
            }
#else
            if((card->getargdecode()->getcardtype() == 1) || (card->getargdecode()->getcardtype() == 0))
            {
                card->setstatestr(tr("Waiting"));
            }
            else
            {
                card->setstatestr(tr("Illegal item"));
                card->setneedtest(false);
            }
#endif
        }
        else
        {
            card->setstatestr(tr("Insert chip"));
        }

        for(int idex=0;idex<=TEST_CARD_POS;idex++)
        {
            if(m_conveeg[idex] != NULL) //找到最后插入的有效卡
            {
                if(m_conveeg[idex]->getneedtest())
                    if(m_conveeg[idex] == card)
                        m_lastbatch = card->getbatch();//设置默认批号
                break;
            }
        }
#if 0
        if(hascandetcard() == 1) //只有一张有效卡时 默认测试时间就是它
        {
            for(int idex=0;idex<=TEST_CARD_POS;idex++)
            {
                if(m_conveeg[idex] != NULL)
                {
                    if(m_conveeg[idex]->getneedtest())//需要测试
                    {
                        if(m_conveeg[idex]->hascode())//有条码
                        {
                            if(!m_conveeg[idex]->gettestend())//还没测试
                            {
                                m_currdtime = card->getargdecode()->getreatime();
                                break;
                            }
                        }
                    }

                }
            }
        }
        qDebug()<<"reatime = "<<m_currdtime;
        if(card->hascode()) //新设的批号有条码，判断是否时间一样，时间不一样不能测试
        {
            if(m_currdtime != card->getargdecode()->getreatime())
            {
                card->setstatestr(tr("Lot No. conflict"));
                card->setneedtest(false);
            }
        }
        m_xtabwg->xsetcur_row(card);
#else
        m_currdtime = 0;
        int curpos = 0;
        for(int idex=SCAN_CARD_POS;idex<=TEST_CARD_POS;idex++)
        {
            if(m_conveeg[idex] == card)
            {
                curpos = idex;
                break;
            }
        }

        for(int idex=curpos+1;idex<=TEST_CARD_POS;idex++)
        {
            if(m_conveeg[idex] != NULL)
            {
                if(m_conveeg[idex]->getneedtest())//需要测试
                {
                    if(m_conveeg[idex]->hascode())//有条码
                    {
                        if(!m_conveeg[idex]->gettestend())//还没测试
                        {
                            m_currdtime = m_conveeg[idex]->getsurptime(); //获得前一张卡还剩下多少时间
                            break;
                        }
                    }
                }

            }
        }

        if((m_currdtime != 0) && card->hascode()) //新设的批号有条码，判断时间是否比较长，否则不能测试
        {
            if((m_currdtime+CSysInfo::get_cfg(CFG_JC_TIMESPE).toInt()/1000) > card->getsurptime())
            {
                card->setstatestr(tr("Time conflict"));
                card->setneedtest(false);
            }
        }
#endif
        m_xtabwg->xsetcur_row(card);
    }
}

void CDetectWidget::slotclickback()
{
    //#if defined(DEVICE_TEST)
    //    m_test = false;
    //#endif

    emit sigperform(WAIT_WG);
}

void CDetectWidget::slottabclick(XTestCard *card)
{
    m_drawcase->setcardselet(-1);

    if(card != NULL)
    {
        for(int idex=0;idex<=TEST_CARD_POS;idex++)
        {
            if(m_conveeg[idex] == card) //找到最后插入的有效卡
            {
                m_drawcase->setcardselet(idex);
                break;
            }
        }
    }
    else
    {
        m_drawinfo->ClearID();
    }

    updatainfo(card);
}

//清卡按钮单击事件
void CDetectWidget::slotclickclear()
{
#if defined(DEVICE_TEST)
    m_test = false;
#endif

    if(!m_runcheck)
    {
        if(CCommonInterface::get_qrscan()->qrscanend() &&
                CCommonInterface::get_incard()->incardend() &&
                CCommonInterface::get_conveyor()->conveyorend() &&
                (hascandetcard() == 0))
        {

            clearcard();
        }
        else
        {
            m_xtabwg->setEnabled(false);
            m_promptbox->settitle(tr("Other cartridges are waiting for test,\n please confirm to clear or not.\n\nConfirm：OK  Cancel： Return"));
            m_promptbox->setnsec(6); 
            m_promptbox->setwhichbutton(1);
            m_promptbox->setpromptser(0);
            m_promptbox->show();
        }
    }
}

//循环线程
void CDetectWidget::slotDetect()
{
    m_jiffyes++;

    //上次传送带已经走完 且 现在传送带没走
    if(m_currunstop && CCommonInterface::get_conveyor()->conveyorend())
    {
        //没有在扫描 且 需要扫描
        if(CCommonInterface::get_qrscan()->qrscanend() && needtoscanqr())
        {
            qDebug() << m_conveeg[SCAN_CARD_POS]->getsernum() << "card scan 1d code. jiffy =" << m_jiffyes;

            CCommonInterface::set_prompttitle(tr("Getting the lot number, Please wait!"));
            m_drawcase->setscanqrstate(true);
            CCommonInterface::get_qrscan()->start_scan();//开始扫描条码
            m_scancard = m_conveeg[SCAN_CARD_POS];
            m_scancard->setstatestr(tr("Identifing"));
        }

        //没有在检测 且 需要检测
        if(CCommonInterface::get_detection()->detetend() && needtodetect())
        {
            qDebug() << "detect start. jiffy =" << m_jiffyes;

            m_drawcase->setdetetstate(true);
            CCommonInterface::get_detection()->detetstart();
        }

        if(CCommonInterface::get_qrscan()->qrscanend() &&   //条码扫描完成
                CCommonInterface::get_detection()->detetend() && //测试完成
                CCommonInterface::get_incard()->incardend())     //进卡完成
        {
            bool bshift = false;
            int runnstep = 0;
            if(m_conveeg[INSERT_CARD_POS] != NULL) //插卡处有卡，判断是否移动 1
            {
                if((m_conveeg[TEST_CARD_POS] == NULL) ||           // 测试位无卡
                        !m_conveeg[TEST_CARD_POS]->getneedtest()|| // 测试位的卡不需要测试
                        m_conveeg[TEST_CARD_POS]->gettestend() ||  // 测试位的卡测试完成
                        !m_conveeg[TEST_CARD_POS]->hasbatch() ||   // 测试位的卡无批号
                        !m_conveeg[TEST_CARD_POS]->hascode())      // 测试位的卡无条码
                {
                    bshift = true;  // 传送带需要移动 1
                    runnstep = 1;   // 移动1步骤
                    qDebug()<<"new in card. run 1. jiffy =" << m_jiffyes;
                }
            }

            if(!bshift)
            {
                int index = TEST_CARD_POS;
                //查找最近一张有效卡
                while(index > INSERT_CARD_POS)
                {
                    //有卡 需要测试 还没测试 有条码
                    if((m_conveeg[index] != NULL) &&
                            m_conveeg[index]->getneedtest() &&
                            !m_conveeg[index]->gettestend() &&
                            m_conveeg[index]->hascode())
                    {
                        break;
                    }
                    index--;
                }

                if(index > INSERT_CARD_POS)
                {
                    int time = m_conveeg[index]->getsurptime();
//                    qDebug() << m_conveeg[index]->getsernum() << "card remainder" << time << "s. jiffy =" << m_jiffyes;

                    if(time <= STOP_INCARD_TIME) // 剩余时间 <= 8; 准备测试
                    {
                        bshift = true;
                        runnstep = TEST_CARD_POS-index;
                        qDebug()<<"need conveyor run" << runnstep << ". jiffy =" << m_jiffyes;
                    }
                }
            }

            if(bshift)
            {
                leftshiftnstep(runnstep);//卡槽皮带往左运行 runnstep 个步骤
            }
        }
    }

    m_xtabwg->xupdate_tab();    // 更新左侧已进卡列表信息
    m_drawcase->updatecase(m_conveeg);
}

//收到进卡信号，第INSERT_CARD_POS位置置成有卡状态
void CDetectWidget::slotincard()
{
    //    qDebug()<<"slotincard";
    CCommonInterface::set_prompttitle(tr("Inserting cartridge, please wait..."));
    m_xtabwg->setEnabled(false);
    m_drawcase->setincardstate(true);
}

//收到进卡完成信号
void CDetectWidget::slotincardsucc()
{
    //    qDebug()<<"slotincardsucc";
    m_drawcase->setincardstate(false);
    //m_drawcase->setnposcolor(INSERT_CARD_POS,QColor(Qt::blue));

    //    insertnewcard();
    m_currcardnum++;
    XTestCard *newcard = new XTestCard(m_currcardnum);
    if(newcard !=  NULL)
    {
        m_conveeg[INSERT_CARD_POS] = newcard;
        m_conveeg[INSERT_CARD_POS]->setstatestr(tr("Identifing"));
        m_xtabwg->xinsert_row(newcard);
    }

    CCommonInterface::get_incard()->set_inallow(false);
    m_drawcase->updatecase(m_conveeg);
}

//条码扫描完成回调
void CDetectWidget::slotqrscan()
{
    m_xtabwg->setEnabled(true);
    QString tmpstr;
    if(m_scancard != NULL) //当前位置有卡
    {
        if(CCommonInterface::get_qrscan()->getqrcode(&tmpstr)) //扫描条码成功
        {
            setcardbatch(m_scancard,"Y"+tmpstr);
            if(!this->isHidden() && !m_scancard->hascode())
            {
                // m_promptbox->settitle(tr("当前批号%1，找不到对应数据，请插入芯片！\n确认：等待插入  取消：取消测试").arg(m_scancard->getbatch()));
                m_promptbox->settitle(tr("Current Lot No. %1. Cannot find data. Please insert chip.        \n Confirmation: wait for insert  Cancel: cancel test").arg(m_scancard->getbatch()));
                m_promptbox->setnsec(6);
                m_promptbox->setwhichbutton(0);
                m_promptbox->setpromptser(2);
                m_promptbox->show();
            }
        }
        else if(!this->isHidden())
        {
            m_scancard->setstatestr(tr("Unidentifiable"));
            m_promptbox->settitle(tr("Fail to recognize the current lot number,\ntacit to use the last lot number!\nConfirm：%1  Cancel：Manual editing").arg(m_lastbatch));
            m_promptbox->setnsec(6);
            m_promptbox->setwhichbutton(0);
            m_promptbox->setpromptser(1);
            m_promptbox->show();
        }

        m_needbatch = m_scancard;
        m_xtabwg->xsetcur_row(m_scancard);
        m_scancard->setnoscan();
    }

    m_scancard = NULL;
    m_drawcase->setscanqrstate(false);
}

//开始测试回调
void CDetectWidget::slotdetectstart()
{
    CCommonInterface::set_prompttitle(tr("Cartridge is being tested."));

    if( ! this->isHidden() && (m_conveeg[INSERT_CARD_POS] == NULL) )
        CCommonInterface::get_incard()->set_inallow(true);

    m_detectcard = m_conveeg[TEST_CARD_POS];
    if(m_detectcard != NULL)
    {
        qDebug() << "ser" << m_detectcard->getsernum() << "card detecting";
        m_detectcard->setstatestr(tr("Testing"));
        if(m_xtabwg->xgetcur_row() == m_detectcard)
        {
            updatainfo(m_detectcard);
        }
    }

#if defined(DEVICE_TEST)
    m_test = true;
#endif
}

//收到所有荧光值
void CDetectWidget::slotdetectsucc()
{
    m_drawcase->setdetetstate(false);
    //当前位置有卡
    if(m_detectcard != NULL)
    {
        m_detectcard->setstatestr(tr("Complete"));
        m_detectcard->settestend(true);

        unsigned int * fldata = CCommonInterface::get_detection()->getfldata();

        //        for(int i=0;i<600;i++)
        //        {
        //            fldata[i] = fldata[i]*(CSysInfo::get_cfg(CFG_FY_CORRECT).toDouble());
        //        }
        m_detectcard->setfldata(fldata);

        m_detectcard->getformula()->CalculateRes();

        int ser = CDataBase::get_InsertAdss(TABLE_BODY_NAME);

        m_detectcard->setsavepos(ser);

        for(int i=0;i<m_detectcard->getargdecode()->getunite() && i < 6;i++)
        {
            m_detectcard->setresdata(i,m_detectcard->getformula()->Get_ResStr(i));
        }

        m_detectcard->truntoresdata();
        m_detectcard->setupdatedtime(QDateTime::currentDateTime());
        QByteArray flstr;
        CCommonInterface::fldata_toflstring(fldata,&flstr);

        qDebug() << "card type" << m_detectcard->getargdecode()->getcardtype();

        if((m_detectcard->getargdecode()->getcardtype() == 0) ||
                (m_detectcard->getargdecode()->getcardtype() == 2))
        {
            QList<QVariant> res;
            res.append(ser);
            res.append(m_detectcard->getbatch());
            res.append(m_detectcard->getendtime());
            res.append(m_detectcard->getresdata()->unite);

            for(int i=0;i<m_detectcard->getresdata()->unite && i < 6;i++)
            {
                res.append(m_detectcard->getresdata()->pro[i].name);
                res.append(m_detectcard->getresdata()->pro[i].unit);
                res.append(m_detectcard->getresdata()->pro[i].result);
            }

            for(int i=m_detectcard->getresdata()->unite;i<6;i++)
            {
                res.append("");
                res.append("");
                res.append("");
            }

            res.append(m_detectcard->getnotes());
            res.append(m_detectcard->getupdatedtime());

            if(CDataBase::record_IsExist(TABLE_BODY_NAME, "SERNUM", ser)) //记录存在
            {
                CDataBase::del_Record(TABLE_BODY_NAME, "SERNUM", ser);
            }
            CDataBase::add_Record(TABLE_BODY_NAME, res);

            res.clear();
            res.append(ser);
            res.append(flstr);

            if(CDataBase::record_IsExist(FLUDT_BODY_NAME, "SERNUM", ser)) //记录存在
            {
                CDataBase::del_Record(FLUDT_BODY_NAME, "SERNUM", ser);
            }
            CDataBase::add_Record(FLUDT_BODY_NAME, res);

            //保存患者信息
            if(m_detectcard->m_bIsSave)
            {
                QList<QVariant> patientInfoList;
                patientInfoList.clear();
                patientInfoList.append(ser);
                patientInfoList.append(m_detectcard->m_sID);
                patientInfoList.append(m_detectcard->m_sName);
                patientInfoList.append(m_detectcard->m_sSex);
                patientInfoList.append(m_detectcard->m_sAge);
                patientInfoList.append(m_detectcard->m_sSmpNum);
                patientInfoList.append(m_detectcard->m_sSmpType);

                bool isPatientExist = CDataBase::IsPatientExist(QString::number(ser));
                if(isPatientExist)
                {
                    CDataBase::UpdataPatientInfo(patientInfoList);
                }
                else
                {
                    CDataBase::add_Record(PATIENT_BODY_NAME,patientInfoList);
                }
            }
        }

        if(CCommonInterface::get_datacom()->isBuildCommt())
        {
            QByteArray send;
            char a[5] = {0};
            memcpy(a,&ser,4);
            send.append(a,4);//流水号
            send.append(flstr);
            CCommonInterface::get_datacom()->SendData(send, COM_RPT, DATA_FLUO);

            qDebug() << "slotdetectsucc()" << ser;

            //推送
            send.clear();
            if(CCommonInterface::get_datacom()->orgHistData(ser, &send))
            {
                qDebug() << "orgHistData() true";
#if 0
                SEND_ST tmp;
                tmp.comT = COM_RPT;
                tmp.dataT = DATA_HIST;
                tmp.data.append(send);
                CCommonInterface::get_datacom()->subPackageData(tmp);
#else
                CCommonInterface::get_datacom()->SendData(send, COM_RPT, DATA_HIST);
#endif
            }
        }
        else if(CSysInfo::get_cfg(CFG_QT_AUTOPRT).toBool())
        {
            if(CSysInfo::get_cfg(CFG_QT_PRTREF).toBool())
                CCommonInterface::get_printf()->set_bpfrefer(true);
            else
                CCommonInterface::get_printf()->set_bpfrefer(false);

            CCommonInterface::get_printf()->Printf_topage(m_detectcard->getresdata());
        }

        m_xtabwg->xsetcur_row(m_detectcard);
    }

#ifdef DEVICE_TEST
    if (m_test)
        m_pTimerDeviceTest->start(13000);//7秒间隔为4秒, 13秒间隔为10秒
#else
    m_detectcard = NULL;

    int index = TEST_CARD_POS - 1;
    //查找最近一张有效卡
    while(index > INSERT_CARD_POS)
    {
        //有卡 需要测试 还没测试 有条码
        if((m_conveeg[index] != NULL) &&
                m_conveeg[index]->getneedtest() &&
                !m_conveeg[index]->gettestend() &&
                m_conveeg[index]->hascode())
        {
            break;
        }
        index--;
    }

    if(index == INSERT_CARD_POS)//无卡
    {
        CCommonInterface::get_optbuzzer()->ringdiii();
    }
#endif

    qDebug() << "CDetectWidget::slotdetectsucc() done";
}

void CDetectWidget::SlotWaitCode(QString batch)
{
    if(!this->isHidden())
    {
        int idex = TEST_CARD_POS;
        //查找最近一张有效卡
        while(idex > INSERT_CARD_POS)
        {
            //找到有批号没条码的卡
            if((m_conveeg[idex] != NULL) && (m_conveeg[idex]->hasbatch()) && (!m_conveeg[idex]->hascode()))
            {
                if(batch == m_conveeg[idex]->getbatch())
                {
                    setcardbatch(m_conveeg[idex],batch);
                }
            }
            idex--;
        }
    }
}

void CDetectWidget::Slotrun1step()
{
    //    qDebug()<<"Slotrun1step";

    if(m_conveeg[TEST_CARD_POS] != NULL)
        m_conveeg[TEST_CARD_POS]->settestend();

    for(int i = TEST_CARD_POS; i > INSERT_CARD_POS; i--)
    {
        m_conveeg[i] = m_conveeg[i - 1];
    }

    m_conveeg[INSERT_CARD_POS] = NULL;
    //m_drawcase->setleftnstep(1);
    m_drawcase->updatecase(m_conveeg);
}

void CDetectWidget::Slotrunend()
{
    //    qDebug()<<"Slotrunend";
    m_currunstop = true;
    if(!this->isHidden())
    {
        m_xtabwg->setEnabled(true);
        if(m_runcheck)
        {
            m_runcheck = false;
            //CCommonInterface::get_conveyor()->start_check();
            CCommonInterface::get_incard()->set_inallow(true);
            m_promptbox->hide();
        }
        else if(m_conveeg[INSERT_CARD_POS] == NULL) //插卡处无卡
            CCommonInterface::get_incard()->set_inallow(true);

    }
}

void CDetectWidget::Slotcheckover()
{
    if(!this->isHidden())
    {
        //        qDebug()<<"Slotcheckover";
        CCommonInterface::get_incard()->set_inallow(true);
        m_xtabwg->setEnabled(true);
        m_promptbox->hide();
    }
}

void CDetectWidget::Slotpromptok(int ser)
{
    if(ser == 0)
    {
        if(!this->isHidden())
            clearcard();
    }
    else if(ser == 1)
    {
        setcardbatch(m_needbatch,m_lastbatch);
    }
    else if(ser == 2)
    {
    }
    m_xtabwg->setEnabled(true);
}

void CDetectWidget::Slotpromptcs(int ser)
{
    if(ser == 0)
    {
    }
    else if(ser == 1)
    {
        m_xtabwg->xsetcur_row(m_needbatch);
        //m_needbatch = NULL;
    }
    else if(ser == 2)
    {
        m_needbatch->setneedtest(false);
        m_needbatch->setstatestr(tr("Cancel"));
    }
    m_xtabwg->setEnabled(true);
}

void CDetectWidget::Slotsetbatch(XTestCard *card, QString batch)
{
    setcardbatch(card,batch);
    if((card != NULL)&&!card->hascode())
    {
        m_needbatch = card;
        m_promptbox->settitle(tr("当前批号%1，找不到对应数据，请插入芯片！\n确认：等待插入  取消：取消测试").arg(card->getbatch()));
        m_promptbox->setnsec(6);
        m_promptbox->setwhichbutton(0);
        m_promptbox->setpromptser(2);
        m_promptbox->show();
    }
}

void CDetectWidget::slotChangeID(bool isBatch)
{
    m_bShowBatch = isBatch;
    m_xtabwg->refresh();
}

#if defined(DEVICE_TEST)
void CDetectWidget::slotDeviceTest()
{
    qDebug() << "CDetectWidget::slotDeviceTest()";
    m_pTimerDeviceTest->stop();

    if(CCommonInterface::get_detection()->detetend())
    {
        m_drawcase->setdetetstate(true);
        CCommonInterface::get_detection()->detetstart();
    }
}
#endif

void CDetectWidget::SetCurveData(QByteArray data, ENUM_WIDGET returnWg)
{
    emit SendCurveData(data,returnWg);
    emit sigperform(YG_WG);
}

void CDetectWidget::SetUpdateID(QString qStrID)
{
    qDebug() << "CDetectWidget::SetUpdateID()" << qStrID;
    m_drawinfo->UpdateID(qStrID);
}







