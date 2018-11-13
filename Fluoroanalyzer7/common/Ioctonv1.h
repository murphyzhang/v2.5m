#ifndef IOCTONV1_H
#define IOCTONV1_H

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

#define DEV_PATH    ("/dev/step")


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



#define MOTOR2_PATH     ("/BioTime/motor2.ini")

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


//  1代底层增加蜂鸣器

class CIoctonv1
{
public:
    CIoctonv1();

    static unsigned  int  ReadGpg4Data();


    static void Gpb5Up();
    static void Gpb5Down();    
    static unsigned  int  ReadGpb5Data();

    static void Gpb6Up();
    static void Gpb6Down();

};

#endif // IOCTONV1_H
