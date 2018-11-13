#include <QtGui/QApplication>
#include "MainWindow.h"

#include <QWSServer>
#include "common/sysinfo.h"
#include "common/commoninterface.h"

MainWindow * g_pMainWindow;
QApplication * g_pApp;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    g_pApp = &a;

    //QApplication::setOverrideCursor(Qt::BlankCursor);
    //QApplication::restoreOverrideCursor();

    QWSServer::setCursorVisible(false);

//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);

    QFile qss(":/style.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    CSysInfo::power_on();

    CCommonInterface tmp;
    CCommonInterface::star_all();

    CDataBase::Init(X_BTDB_PATH);
    MainWindow mainwindow;
    g_pMainWindow = &mainwindow;
    mainwindow.show();

    CCommonInterface::init_showtitle();

    return a.exec();
}


