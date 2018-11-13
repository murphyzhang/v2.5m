#include "commoninterface.h"
#include "common/configurate.h"
#include <QFile>



int CCommonInterface::m_verify;

CConveyor *CCommonInterface::m_conveyor;
CInCard *CCommonInterface::m_incard;
CDetection *CCommonInterface::m_detection;
CQrScan *CCommonInterface::m_qrscan;
CXPrintf *CCommonInterface::m_xprintf;
COptEeprom *CCommonInterface::m_opteeprom;
XOptBuzzer *CCommonInterface::m_optbuzzer;
XArgDecode *CCommonInterface::m_proinfo;
CFormula *CCommonInterface::m_formula;
CDataCom *CCommonInterface::m_datacom;

int CCommonInterface::m_searchscope;
int CCommonInterface::m_areascope;
int CCommonInterface::m_basescope;

CShowTitleWidget *CCommonInterface::m_title[4];

int CCommonInterface::m_testMode = 0;

QString CCommonInterface::m_batch;



//CInsertCard CCommonInterface::m_insertcard;



bool CCommonInterface::b_printf = true;
bool CCommonInterface::b_pfrefer = false;

CCommonInterface::CCommonInterface()
{
    m_conveyor = new CConveyor();
    m_incard = new CInCard();
    m_detection = new CDetection();
    m_qrscan = new CQrScan();
    m_xprintf = new CXPrintf();
    m_opteeprom = new COptEeprom();
    m_optbuzzer = new XOptBuzzer();
    m_proinfo = new XArgDecode();
    m_formula = new CFormula();
    m_datacom = new CDataCom();
//    qDebug()<<"---------init CCommonInterface-------------";
    connect(m_datacom, SIGNAL(SigCnntSucc(bool)), this, SLOT(SlotCnntStus(bool)));
    connect(m_datacom, SIGNAL(SigNewBatch(QString)), this, SLOT(SlotWaitCode(QString)));
    connect(m_opteeprom, SIGNAL(SigNewBatch(QString)), this, SLOT(SlotWaitCode(QString)));
}

QByteArray CCommonInterface::get_sysversion()
{
    return CURR_SYS_VERSIONS;
}

CConveyor * CCommonInterface::get_conveyor()
{
    return m_conveyor;
}

CInCard * CCommonInterface::get_incard()
{
    return m_incard;
}
CDetection * CCommonInterface::get_detection()
{
    return m_detection;
}

CQrScan * CCommonInterface::get_qrscan()
{
    return m_qrscan;
}

CXPrintf * CCommonInterface::get_printf()
{
    return m_xprintf;
}


COptEeprom * CCommonInterface::get_opteep()
{
    return m_opteeprom;
}

XOptBuzzer * CCommonInterface::get_optbuzzer()
{
    return m_optbuzzer;
}
XArgDecode * CCommonInterface::get_currproinfo()
{
    return m_proinfo;
}

CFormula * CCommonInterface::get_formula()
{
    return m_formula;
}

CDataCom * CCommonInterface::get_datacom()
{
    return m_datacom;
}

bool CCommonInterface::fldata_toflstring(unsigned int *data,QByteArray *str)
{

    if((data != NULL) && (str != NULL))
    {
        for(int i=0;i<600;i++)
        {
            str->append((data[i]>>24)&0xff);
            str->append((data[i]>>16)&0xff);
            str->append((data[i]>>8)&0xff);
            str->append((data[i])&0xff);
        }
    }
    return true;
}

bool CCommonInterface::flstring_tofldata(QByteArray &str, unsigned int *data)
{
    if((str.length() == 2400) && (data != NULL))
    {
        for(int i=0;i<600;i++)
        {
            int tmp = 0;
            for(int j=0;j<4;j++)
            {
                tmp<<=8;
                tmp += (uchar)str.at(i*4+j);
            }
            data[i] = tmp;
        }
    }
    return true;
}

void CCommonInterface::init_showtitle()
{
    m_title[0] = new CShowTitleWidget();
    m_title[0]->set_timer(1000);
    m_title[0]->set_geometry(10,0,400,40);
    m_title[0]->set_alignment(Qt::AlignLeft);
    m_title[0]->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_title[0]->show();

    m_title[1] = new CShowTitleWidget();
    m_title[1]->set_text(tr("Biotime FIA Analyzer"));
    m_title[1]->set_geometry(400,0,380,40);
    m_title[1]->set_alignment(Qt::AlignRight);
    m_title[1]->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_title[1]->show();

    m_title[2] = new CShowTitleWidget();
    m_title[2]->set_text(QString(""));
    m_title[2]->set_geometry(10,430,400,40);
    m_title[2]->set_alignment(Qt::AlignLeft);
    m_title[2]->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_title[2]->setprompt();
    m_title[2]->show();

    m_title[3] = new CShowTitleWidget();
    //m_title[3]->set_text(tr("宝太生物科技有限公司"));
    m_title[3]->set_text(tr("Biotime Biotechnology Co., Ltd."));
    m_title[3]->set_geometry(400,440,380,40);
    m_title[3]->set_alignment(Qt::AlignRight);
    m_title[3]->setStyleSheet(FONTFAMILY + FONT_SIZE_3);
    m_title[3]->show();
}

CShowTitleWidget * CCommonInterface::get_showtitle(int tl)
{
    if(tl<4)
        return m_title[tl];
}
void CCommonInterface::set_prompttitle(QString str)
{
    m_title[2]->showtext(str);
}

int CCommonInterface::get_runmode()
{
    return m_testMode;
}

void CCommonInterface::set_runmode(int mode)
{
    m_testMode = mode;
}

bool CCommonInterface::get_isinsertcard()
{
    if(XIoctl::get_gpdat('g',4)>0)
        return false;
    else
        return true;
}

QString CCommonInterface::get_currbatch()
{
    return m_batch;
}

void CCommonInterface::set_currbatch(QString batch)
{
    m_batch.clear();
    m_batch.append(batch);
}

void CCommonInterface::set_currcode(QByteArray code)
{
    m_proinfo->set_newcode(code);
}
QByteArray CCommonInterface::get_currcode()
{
    return m_proinfo->get_codestr();
}


CInsertCard * CCommonInterface::get_insertcard()
{
    //return &m_insertcard;
}




void CCommonInterface::star_all()
{
    m_conveyor->start();
    m_incard->start();
    m_detection->start();
    m_qrscan->start();
    m_opteeprom->start();
    //m_insertcard.start();
    m_optbuzzer->start();

    m_datacom->start();
}

void CCommonInterface::SlotWaitCode(QString batch)
{
    //m_title[2]->showtext(tr("芯片批号：%1").arg(batch));
    m_title[2]->showtext(tr("Lot No.：%1").arg(batch));
}

void CCommonInterface::SlotCnntStus(bool succ)
{
    if(succ)
        m_title[2]->showtext(tr("Connect to PC successfully"));
    else
        m_title[2]->showtext(tr("Disconnect to PC"));
}

QString CCommonInterface::GetNowDateString()
{
    QDateTime dtCur = QDateTime::currentDateTime();
    return dtCur.toString("yyMMdd");
}










