#ifndef STEPPINGMOTOR_H
#define STEPPINGMOTOR_H

#include <QObject>

#ifdef VER_LINUX
#include <sys/ioctl.h>
#include <sys/select.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <QFile>
#include <QtGui>
#include <QtCore>


#define MOTOR_PATH      ("/BioTime/motor.ini")  //采集电机一开机走的配置
#define MOTOR_1_PATH    ("/BioTime/motor1.ini") //采集电机工作时走的配置


typedef struct tagSPEED_STEP_T
{
    unsigned long nDirect; // 方向
    unsigned long nSpeed;  // 代表速度，速度值越小，速度越快  给50是， 1.285KHZ， 200是213HZ
    unsigned long nStepCount; // 代表段数
}SPEED_STEP_T;

typedef struct tagSTEP_RUN_INFO_T
{
    unsigned int nCount;	 // 代表多少段
    SPEED_STEP_T astSpeedSteps[10];
}STEP_RUN_INFO_T;



class CSteppingMotor
{
public:
    explicit CSteppingMotor();

    static int SteppingMotorInit();

    static int SteppingMotorFirstRun();
    static int SteppingMotorRun();

    static int SteppingIsRun(); //

    static int SteppingStop();

    static int Send2jin();
    static void Motor1RunN(STEP_RUN_INFO_T *run);
signals:

public slots:

private:
    static STEP_RUN_INFO_T m_stStepFirstRunInfo;
    static STEP_RUN_INFO_T m_stStepRunInfo;
    static int m_nFd;

   // static QFile m_file;

//    static int m_nDirect; // 方向
//    static int m_nSpeed;// 代表速度，速度值越小，速度越快  给50是， 1.285KHZ， 200是213HZ
//    static int m_nStepCount;// 代表段数
};

#endif // STEPPINGMOTOR_H
