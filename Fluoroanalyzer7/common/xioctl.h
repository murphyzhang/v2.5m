#ifndef XIOCTL_H
#define XIOCTL_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef  VER_WINDOWS // VER_LINUX
#include <sys/select.h>
#include <sys/ioctl.h>
#endif
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <QtGui>
#include <QtCore>
#include <QByteArray>

#include "common/configfile.h"

#define DEBUG_ME_LEDS			0
#define DEVICE_NAME			"step"

/* 应用程序执行ioctl(fd, cmd, arg)时的第2个参数 */
#define IOCTL_GPIO_OFF			0
#define IOCTL_GPIO_ON			1
#define IOCTL_PULL_OFF			10
#define IOCTL_PULL_ON			11
#define IOCTL_GPIO_READ			2
#define IOCTL_STEP_RUN			3
#define IOCTL_RED_GPIO_REG		12
#define IOCTL_WRIT_GPIO_REG		13
#define IOCTL_X_STEP_RUN		14
#define IOCTL_TIMER_STAUS		15
#define IOCTL_TIMER_STOP		16


//定时器状态
#define TIMER_IN_USED			0
#define TIMER_UN_USED			1


#define TIMER_X_STEP	0/*定时器用于产生x电机脉冲*/
#define TIMER_ad_STEP	1/*定时器用于产生采样电机脉冲*/


typedef enum tagGPIO_TYPE_E
{
    S3C2410_GPACON,//0
    S3C2410_GPADAT,//1
    S3C2410_GPBCON,//2
    S3C2410_GPBDAT,//3
    S3C2410_GPBUP,//4
    S3C2416_GPBSEL,//5
    S3C2410_GPCCON,//6
    S3C2410_GPCDAT,//7
    S3C2410_GPCUP,//8
    S3C2410_GPDCON,//9
    S3C2410_GPDDAT,//10
    S3C2410_GPDUP,//11
    S3C2410_GPECON,//12
    S3C2410_GPEDAT,//13
    S3C2410_GPEUP,//14
    S3C2410_GPFCON,//15
    S3C2410_GPFDAT,//16
    S3C2410_GPFUP,//17
    S3C2410_GPGCON,//18
    S3C2410_GPGDAT,//19
    S3C2410_GPGUP	//20

}GPIO_TYPE_E;

typedef enum
{
    INSIDE_SCANMODEL,
    OUTSIDE_SCANMODEL
}SCAN_MODEL_E;


#define MOTOR_PATH      ("/BioTime/motor.ini")  //采集电机一开机走的配置

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

#define  SPEED_STEP_BUFF_SIZE   (4)


class XIoctl
{
public:
    XIoctl();
    static bool get_group(char group,GPIO_TYPE_E *con,GPIO_TYPE_E *dat);
    static int get_gpdat(char group,int io);
    static void set_gpdat(char group,int io,int data);

    static unsigned  int  TimeState();
    static void StopTime(); // 停止定时器

    /********************************** A3977 荧光采集电机 ***************************************/
    static void InitMotor1();
    static void Motor1RunN(STEP_RUN_INFO_T &run);
    static unsigned int Motor1IsRun();
    static void Motor1Stop();

    /**********************************  卡槽电机 ************************************************/
    static void InitMotor2();
    static void Motor2RunN(SPEED_STEP_T &run);

    /********************************** L293DD 进卡电机 ******************************************/
    static void InitDcMotor();
    static void SetDcMotorRun();
    static void SetDcMotorStop();

    /********************************** 面板LED灯 ************************************************/
    static void InitLED();
    static void SetLEDAllow();
    static void SetLEDUNAllow();

    /********************************** OP701 进卡检测光耦 ****************************************/
    static void InitOPCard();
    static int GetOPCardData();

    /********************************** OP702 卡槽运行步数检测光耦 *********************************/
    static void InitOPConveyor();
    static int GetOPConveyorData();

    /********************************** 设置内部或外部扫描模块***** *********************************/
    static void setScanModel(SCAN_MODEL_E model);
};

#endif // XIOCTL_H
