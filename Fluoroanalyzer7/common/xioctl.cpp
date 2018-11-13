#include "xioctl.h"

XIoctl::XIoctl()
{
}
bool XIoctl::get_group(char group, GPIO_TYPE_E *con, GPIO_TYPE_E *dat)
{
    switch(group)
    {
    case 'a': case 'A':{*con = S3C2410_GPACON; *dat = S3C2410_GPADAT;}break;
    case 'b': case 'B':{*con = S3C2410_GPBCON; *dat = S3C2410_GPBDAT;}break;
    case 'c': case 'C':{*con = S3C2410_GPCCON; *dat = S3C2410_GPCDAT;}break;
    case 'd': case 'D':{*con = S3C2410_GPDCON; *dat = S3C2410_GPDDAT;}break;
    case 'e': case 'E':{*con = S3C2410_GPECON; *dat = S3C2410_GPEDAT;}break;
    case 'f': case 'F':{*con = S3C2410_GPFCON; *dat = S3C2410_GPFDAT;}break;
    case 'g': case 'G':{*con = S3C2410_GPGCON; *dat = S3C2410_GPGDAT;}break;
    default: return false; break;
    }
    return true;
}

int XIoctl::get_gpdat(char group, int io)
{
    GPIO_TYPE_E editcon, editdat;

    if(!get_group(group, &editcon, &editdat))
    {
        return 0;
    }

    int nFd = open(DEV_PATH, O_WRONLY);
    unsigned int anBuff[4] = {0};
    unsigned int nReadData = 0;

    anBuff[0] = editcon;	//那个寄存器
    anBuff[1] = 3<<(2*io);    	//那个位
    anBuff[2] = 0<<(2*io);    	//修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);// in

    anBuff[0] = editdat;	 //那个寄存器
    anBuff[1] = 1<<io;    // 那个位
    anBuff[2] = 0<<io;    //修改到值
    nReadData = ioctl(nFd, IOCTL_RED_GPIO_REG, editdat);// 读取数据
    nReadData &= 1<<io;
    nReadData >>=io;
    close(nFd);
    return nReadData;
}

void XIoctl::set_gpdat(char group, int io, int data)
{
    GPIO_TYPE_E editcon,editdat;
    if(!get_group(group,&editcon,&editdat))
    {
        return ;
    }
    int nFd = open(DEV_PATH, O_WRONLY);
    unsigned int anBuff[4] = {0};

    anBuff[0] = editcon;				//那个寄存器
    anBuff[1] = 3<<(2*io);    	// 那个位
    anBuff[2] = 1<<(2*io);    	//修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);	// out

    anBuff[0] = editdat;	 //那个寄存器
    anBuff[1] = 1<<io;    // 那个位
    anBuff[2] = (data&0x01)<<io;    //修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);//
    close(nFd);
}

unsigned  int  XIoctl::TimeState()
{
#ifdef  VER_LINUX
    unsigned int nReadData = 0;
    int nFd = open(DEV_PATH, O_WRONLY);
    nReadData = ioctl(nFd, IOCTL_TIMER_STAUS, 0);//
    close(nFd);
    return nReadData;
#endif
}

void XIoctl::StopTime()  // 停止定时器
{
#ifdef  VER_LINUX

    int nFd = open(DEV_PATH, O_WRONLY);
    ioctl(nFd, IOCTL_TIMER_STOP, 0);//
    close(nFd);

#endif

}



void XIoctl::InitMotor1()
{

}

void XIoctl::Motor1RunN(STEP_RUN_INFO_T &run)
{
#ifdef VER_LINUX
    int nFd = open(DEV_PATH, O_WRONLY);
    ioctl(nFd, IOCTL_GPIO_ON, 0);
    ioctl(nFd, IOCTL_GPIO_ON, 2);
    ioctl(nFd, IOCTL_GPIO_ON, 3);

    ioctl(nFd, IOCTL_STEP_RUN, &run);
    ioctl(nFd, IOCTL_GPIO_OFF, 0);
    close(nFd);
#endif
}

unsigned int XIoctl::Motor1IsRun()
{
#ifdef VER_LINUX
    unsigned int nState = 0;
    int nFd = open(DEV_PATH, O_WRONLY);
    nState = ioctl(nFd, IOCTL_TIMER_STAUS, 0);     //
    close(nFd);
    return nState;
#endif
}

void XIoctl::Motor1Stop()
{
#ifdef VER_LINUX
    int nFd = open(DEV_PATH, O_WRONLY);
    ioctl(nFd, IOCTL_TIMER_STOP, 0);     //
    close(nFd);
#endif
}

void XIoctl::InitDcMotor()
{
    set_gpdat('b',4,0);
    set_gpdat('b',10,0);
}

void XIoctl::SetDcMotorRun()
{
    set_gpdat('b',4,0);
    set_gpdat('b',10,1);
}

void XIoctl::SetDcMotorStop()
{
    set_gpdat('b',4,0);
    set_gpdat('b',10,0);
}

void XIoctl::InitLED()
{
    set_gpdat('f',5,1);
    set_gpdat('f',6,1);
    //set_gpdat('f',7,1);
    //set_gpdat('g',0,1);
}

void XIoctl::SetLEDAllow()
{
    set_gpdat('f',5,1);
    set_gpdat('f',6,0);
}

void XIoctl::SetLEDUNAllow()
{
    set_gpdat('f',5,0);
    set_gpdat('f',6,1);
}

void XIoctl::InitOPCard()
{
    set_gpdat('b',5,1);
}

int XIoctl::GetOPCardData()
{
#if 0
    int data = get_gpdat('g',4);
    qDebug() << "in card transducer =" << data;
    return data;
#else
    return get_gpdat('g',4);
#endif
}

void XIoctl::InitOPConveyor()
{
    set_gpdat('b',6,1);
}

//卡槽运行步数检测光耦
int XIoctl::GetOPConveyorData()
{
#if 0
    int data = get_gpdat('g',5);
    qDebug() << "car slot transducer =" << data;
    return data;
#else
    return get_gpdat('g',5);
#endif
}

void XIoctl::setScanModel(SCAN_MODEL_E model)
{
#if 1
//    set_gpdat('e', 13, (int)model);
#else
    switch (model)
    {
    case INSIDE_SCANMODEL:
        set_gpdat('e', 13, 0);
        break;
    case OUTSIDE_SCANMODEL:
        set_gpdat('e', 13, 1);
        break;
    default:
        break;
    }
#endif
}

void XIoctl::InitMotor2()
{
    set_gpdat('b',9,0);
    set_gpdat('b',1,0);
}

void XIoctl::Motor2RunN(SPEED_STEP_T &run)
{
//    qDebug() << "card slot motor run. info:" << run.nDirect << run.nSpeed << run.nStepCount;

#ifdef  VER_LINUX
    int nFd = open(DEV_PATH, O_WRONLY);
    unsigned int anBuff[4];// gpbcon GPB9 脉冲控制信号PWM

    anBuff[0] = S3C2410_GPBDAT;	 //那个寄存器
    anBuff[1] = 1<<1;    // 那个位
    anBuff[2] = run.nDirect<<1;    //修改到值
    ioctl(nFd, IOCTL_WRIT_GPIO_REG, anBuff);//

    ioctl(nFd, IOCTL_X_STEP_RUN, &run);

    close(nFd);
#endif
}
