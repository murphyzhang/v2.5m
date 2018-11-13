#-------------------------------------------------
#
# Project created by QtCreator 2013-12-17T16:43:43
#
#-------------------------------------------------

QT       += core gui sql

TARGET = tqpda
TEMPLATE = app

LIBS += -lqwt
LIBS += -L $$PWD/syszux/ -lsyszuxpinyin
#LIBS += -L/home/workspace/MyFolder/syszux -lsyszuxpinyin

DEFINES += VER_LINUX _TTY_POSIX_
#DEFINES += VER_WINDOWS
#DEFINES += VER_ARM
#DEFINES  += LINUX_TEST

SOURCES += main.cpp\
        MainWindow.cpp \
    SerialPort/win_qextserialport.cpp \
    SerialPort/SerialPort.cpp \
    SerialPort/qextserialport.cpp \
    SerialPort/qextserialbase.cpp \
    SerialPort/posix_qextserialport.cpp \
    widget/waitwidget.cpp \
    widget/verifywidget.cpp \
    widget/timeeditwidget.cpp \
    widget/sysinitwidget.cpp \
    widget/showtitlewidget.cpp \
    widget/selfcheckwidget.cpp \
    widget/xtablewidget.cpp \
    widget/xdrawinfowidget.cpp \
    widget/xdrawcasewidget.cpp \
    widget/TableWidget.cpp \
    widget/menuwidget.cpp \
    widget/detectwidget.cpp \
    widget/basefunwidget.cpp \
    base/xlineedit.cpp \
    base/xlabel.cpp \
    base/xinputcontext.cpp \
    base/xbutton.cpp \
    base/Label.cpp \
    base/Button.cpp \
    common/xprintf.cpp \
    common/xioctl.cpp \
    common/SteppingMotor.cpp \
    common/qrscan.cpp \
    common/projectinfo.cpp \
    common/opteeprom.cpp \
    common/optbuzzer.cpp \
    common/insertcard.cpp \
    common/Formula.cpp \
    common/detection.cpp \
    common/datacom.cpp \
    common/DataBase.cpp \
    common/conveyor.cpp \
    common/commoninterface.cpp \
    common/cincard.cpp \
    common/argdecode.cpp \
    widget/inputwidget.cpp \
    common/testcard.cpp \
    widget/samplewidget.cpp \
    widget/recordwidget.cpp \
    widget/batchwidget.cpp \
    widget/promptbox.cpp \
    widget/aboutwidget.cpp \
    widget/syssetwidget.cpp \
    common/sysinfo.cpp \
    base/shutdown.cpp \
    base/MyButton.cpp \
    widget/PatientInfo.cpp \
    base/MyLineEdit.cpp \
    widget/syszuxpinyin.cpp \
    widget/DetailTest.cpp \
    widget/CureTrace.cpp \
    common/TestData.cpp \
    widget/SearchWidget.cpp \
    widget/DeleteChoose.cpp \
    widget/PlotLines.cpp \
    widget/MessageBox.cpp \
    widget/YgCurveWidget.cpp

HEADERS  += MainWindow.h \
    SerialPort/win_qextserialport.h \
    SerialPort/SerialPort.h \
    SerialPort/qextserialport.h \
    SerialPort/qextserialbase.h \
    SerialPort/posix_qextserialport.h \
    widget/waitwidget.h \
    widget/verifywidget.h \
    widget/timeeditwidget.h \
    widget/sysinitwidget.h \
    widget/showtitlewidget.h \
    widget/selfcheckwidget.h \
    widget/xtablewidget.h \
    widget/xdrawinfowidget.h \
    widget/xdrawcasewidget.h \
    widget/TableWidget.h \
    widget/menuwidget.h \
    widget/detectwidget.h \
    widget/basefunwidget.h \
    base/xlineedit.h \
    base/xlabel.h \
    base/xinputcontext.h \
    base/xbutton.h \
    base/Label.h \
    base/Button.h \
    common/xprintf.h \
    common/xioctl.h \
    common/SteppingMotor.h \
    common/qrscan.h \
    common/projectinfo.h \
    common/packet_def.h \
    common/opteeprom.h \
    common/optbuzzer.h \
    common/Ioctonv1.h \
    common/insertcard.h \
    common/global_define.h \
    common/Formula.h \
    common/detection.h \
    common/datacom.h \
    common/DataBase.h \
    common/conveyor.h \
    common/configfile.h \
    common/commoninterface.h \
    common/cincard.h \
    common/argdecode.h \
    widget/inputwidget.h \
    common/testcard.h \
    widget/samplewidget.h \
    widget/recordwidget.h \
    widget/batchwidget.h \
    widget/promptbox.h \
    widget/aboutwidget.h \
    widget/syssetwidget.h \
    common/sysinfo.h \
    common/configurate.h \
    base/shutdown.h \
    base/MyButton.h \
    widget/PatientInfo.h \
    base/MyLineEdit.h \
    widget/syszuxpinyin.h \
    widget/syszuxim.h \
    widget/DetailTest.h \
    widget/CureTrace.h \
    common/TestData.h \
    widget/SearchWidget.h \
    widget/DeleteChoose.h \
    widget/PlotLines.h \
    widget/MessageBox.h \
    widget/YgCurveWidget.h

FORMS    += MainWindow.ui \
    widget/inputwidget.ui \
    widget/syszuxpinyin.ui

RESOURCES += \
    image.qrc \
    style.qrc \
    syszuxpinyin.qrc

OTHER_FILES +=
