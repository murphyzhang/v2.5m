#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget/selfcheckwidget.h"
#include "widget/sysinitwidget.h"
#include "widget/waitwidget.h"
#include "widget/detectwidget.h"
#include "common/global_define.h"
#include "widget/menuwidget.h"
#include "widget/timeeditwidget.h"
#include "widget/verifywidget.h"
#include "widget/recordwidget.h"
#include "widget/batchwidget.h"

#include "widget/samplewidget.h"
#include "widget/syssetwidget.h"
#include "widget/aboutwidget.h"
#include "widget/DetailTest.h"
#include "widget/CureTrace.h"
#include "widget/PlotLines.h"
#include "widget/YgCurveWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);

    ENUM_WIDGET getCurrWinIndex();


signals:


private slots:
    void slotswitchpage(ENUM_WIDGET turn_wg);
    void ShowTestInfo(int iSer);
    void ShowCureTrace(int iSer,QString qStrName,QString qStrSex,QString qStrAge);
    void ShowPlotLines(QVector<CTestData *> vecData);

private:
    Ui::MainWindow *ui;
    CBaseFunWidget *m_currwidget;
    ENUM_WIDGET m_currWinIndex;
    QMap<ENUM_WIDGET, CBaseFunWidget *> widgetmap;

    CSysInitWidget *m_sysinit;
    CSelfCheckWidget *m_check;
    CWaitWidget *m_waitmennu;
    CDetectWidget *m_detectwidget;
    CMenuWidget *m_menuwidget;
    CTimeEditWidget *m_timewidget;
    CVerifyWidget *m_verifywidget;
    CRecordWidget *m_recordwidget;
    CBatchWidget *m_batchwidget;
    CSampleWidget *m_samplewidget;
    CAboutWidget *m_aboutwidget;
    CSyssetWidget *m_syssetwidget;
    CygCurveWidget  *m_ygCurveWidget;

    CDetailTest *m_pDetailTest;
    CCureTrace *m_pCureTrace;
    CPlotLines *m_pPlotLines;
};

#endif // MAINWINDOW_H
