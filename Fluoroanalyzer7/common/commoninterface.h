#ifndef COMMONINTERFACE_H
#define COMMONINTERFACE_H
#include <QObject>

#include "conveyor.h"
#include "cincard.h"
#include "detection.h"
#include "qrscan.h"
#include "global_define.h"
#include "configfile.h"
#include "xprintf.h"
#include "widget/showtitlewidget.h"
#include "xioctl.h"

#include "argdecode.h"
#include "Formula.h"
#include "opteeprom.h"
#include "datacom.h"
#include "DataBase.h"
#include "insertcard.h"
#include "optbuzzer.h"


#define X_PROINFO_MAX 100

#define CI CCommonInterface

class CCommonInterface : public QObject
{
    Q_OBJECT
public:
    CCommonInterface();
    static QByteArray get_sysversion();

    static void star_all();
    static CConveyor* get_conveyor();
    static CInCard* get_incard();
    static CDetection* get_detection();
    static CQrScan* get_qrscan();
    static CXPrintf* get_printf();
    static COptEeprom* get_opteep();
    static CDataCom* get_datacom();
    static CInsertCard* get_insertcard();
    static XOptBuzzer* get_optbuzzer();



    static void init_showtitle();
    static CShowTitleWidget* get_showtitle(int tl);
    static void set_prompttitle(QString str);


    static bool fldata_toflstring(unsigned int *data,QByteArray *str);
    static bool flstring_tofldata(QByteArray &str,unsigned int *data);

    static int get_runmode();
    static void set_runmode(int mode);

    static bool get_isinsertcard();
    static QString get_currbatch();//获取当前批号
    static void set_currbatch(QString batch);

    static void set_currcode(QByteArray code);
    static QByteArray get_currcode();
    static XArgDecode* get_currproinfo();
    static CFormula* get_formula();

    static QString GetNowDateString();

public slots:

    static void SlotWaitCode(QString batch);
    static void SlotCnntStus(bool succ);
private:
    static int m_verify;

    static CConveyor *m_conveyor;
    static CInCard *m_incard;
    static CDetection *m_detection;
    static CQrScan *m_qrscan;
    static CXPrintf *m_xprintf;
    static COptEeprom *m_opteeprom;
    static XOptBuzzer *m_optbuzzer;
    static XArgDecode *m_proinfo;
    static CFormula *m_formula;
    static CDataCom *m_datacom;

    static int m_searchscope;
    static int m_areascope;
    static int m_basescope;
    static CShowTitleWidget *m_title[4];
    static int m_testMode;
    static QString m_batch;


    //static CInsertCard m_insertcard;


    static bool b_printf;   //是否结束打印
    static bool b_pfrefer;  //是否打印参考值

};

#endif // COMMONINTERFACE_H
