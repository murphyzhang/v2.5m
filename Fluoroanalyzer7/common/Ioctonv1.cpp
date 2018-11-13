#include "Ioctonv1.h"

CIoctonv1::CIoctonv1()
{
}


void CIoctonv1::Gpb5Up()
{
#ifdef  VER_LINUX
    int nFd = open(DEV_PATH, O_WRONLY);
    unsigned int anBuff[4] = {0};

    anBuff[0] = S3C2410_GPBCON;				//那个寄存器
    anBuff[1] = 3<<10;    	// 那个位
    anBuff[2] = 1<<10;    	//修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);	// GPB5 out

    anBuff[0] = S3C2410_GPBDAT;	 //那个寄存器
    anBuff[1] = 1<<5;    // 那个位
    anBuff[2] = 1<<5;    //修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);// GPB5 gao
    close(nFd);
#endif
}

void CIoctonv1::Gpb5Down()
{
#ifdef  VER_LINUX
    int nFd = open(DEV_PATH, O_WRONLY);
    unsigned int anBuff[4] = {0};

    anBuff[0] = S3C2410_GPBCON;				//那个寄存器
    anBuff[1] = 3<<10;    	// 那个位
    anBuff[2] = 1<<10;    	//修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);	// GPB5 out

    anBuff[0] = S3C2410_GPBDAT;	 //那个寄存器
    anBuff[1] = 1<<5;    // 那个位
    anBuff[2] = 0<<5;    //修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);// GPB5 di
    close(nFd);
#endif
}

unsigned  int CIoctonv1::ReadGpb5Data()
{

#ifdef  VER_LINUX
    int nFd = open(DEV_PATH, O_WRONLY);
    unsigned int anBuff[4] = {0};
    unsigned int nReadData = 0;

    anBuff[0] = S3C2410_GPGCON;	 //那个寄存器
    anBuff[1] = 3<<10;    	// 那个位
    anBuff[2] = 0<<10;    	//修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);	// GPG5 input

    anBuff[0] = S3C2410_GPGDAT;	 //那个寄存器
    anBuff[1] = 1<<5;    // 那个位
    anBuff[2] = 0<<5;    //修改到值
    nReadData = ioctl(nFd, IOCTL_RED_GPIO_REG, S3C2410_GPGDAT);// 读取数据
    nReadData &= 1<<5;
    nReadData >>=5;
    close(nFd);
    return nReadData;
#endif
}


unsigned  int CIoctonv1::ReadGpg4Data()
{
#ifdef  VER_LINUX
    int nFd = open(DEV_PATH, O_WRONLY);
    unsigned int anBuff[4] = {0};
    unsigned int nReadData = 0;

    anBuff[0] = S3C2410_GPGCON;				//那个寄存器
    anBuff[1] = 3<<8;    	// 那个位
    anBuff[2] = 0<<8;    	//修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);	// GPG4 input

    anBuff[0] = S3C2410_GPGDAT;	 //那个寄存器
    anBuff[1] = 1<<4;    // 那个位
    anBuff[2] = 0<<4;    //修改到值
    nReadData = ioctl(nFd, IOCTL_RED_GPIO_REG, S3C2410_GPGDAT);// 读取数据
    nReadData &= 1<<4;
    nReadData >>=4;
    close(nFd);
    return nReadData;
#endif
}

void CIoctonv1::Gpb6Up()
{
#ifdef  VER_LINUX
    int nFd = open(DEV_PATH, O_WRONLY);
    unsigned int anBuff[4] = {0};

    anBuff[0] = S3C2410_GPBCON;				//那个寄存器
    anBuff[1] = 3<<12;    	// 那个位
    anBuff[2] = 1<<12;    	//修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);	// GPB6 out

    anBuff[0] = S3C2410_GPBDAT;	 //那个寄存器
    anBuff[1] = 1<<6;    // 那个位
    anBuff[2] = 1<<6;    //修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);// GPB6 gao
    close(nFd);
#endif
}

void CIoctonv1::Gpb6Down()
{
#ifdef  VER_LINUX

    int nFd = open(DEV_PATH, O_WRONLY);
    unsigned int anBuff[4] = {0};

    anBuff[0] = S3C2410_GPBCON;				//那个寄存器
    anBuff[1] = 3<<12;    	// 那个位
    anBuff[2] = 1<<12;    	//修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);	// GPB6 out

    anBuff[0] = S3C2410_GPBDAT;	 //那个寄存器
    anBuff[1] = 1<<6;    // 那个位
    anBuff[2] = 0<<6;    //修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);// GPB6 di
    close(nFd);
#endif
}
