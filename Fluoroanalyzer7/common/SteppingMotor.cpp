#include "SteppingMotor.h"
#include "SteppingMotor.h"
#include <QString>


STEP_RUN_INFO_T CSteppingMotor::m_stStepFirstRunInfo = {0};  // 电机一开始配置
STEP_RUN_INFO_T CSteppingMotor::m_stStepRunInfo      = {0};
int CSteppingMotor::m_nFd = 0;

//int CSteppingMotor::m_nDirect = 0;      // 方向
//int CSteppingMotor::m_nSpeed = 0;       // 代表速度，速度值越小，速度越快  给50是， 1.285KHZ， 200是213HZ
//int CSteppingMotor::m_nStepCount = 0;   // 代表段数

CSteppingMotor::CSteppingMotor()
{
}

//#define MOTOR_PATH     ("/BioTime/motor.ini") //采集电机一开机走的配置
//#define MOTOR_1_PATH    ("/BioTime/motor1.ini") //采集电机工作时走的配置
// 配置文件第一行为开机后要走的电机配置
// 第二行到最多第十行为电机后面工作到配置
int CSteppingMotor::SteppingMotorInit()
{
    /////////////// 配置采集电机一开始走的文件////////////////////
    QFile file;
    file.setFileName(MOTOR_PATH);
    bool bIsOpen = file.open(QIODevice::ReadOnly);
//    qDebug() << "open /motor.ini" << bIsOpen;

    if(bIsOpen)
    {
        // 第一行开始读 此时开始最多10行为步进电机扫描时的配置
        for(int nCount = 0;
            (nCount < 10)&&(file.atEnd() == false);
            nCount++)
        {
            QByteArray array = file.readLine();
            QString strFile(array);
            int nSize = strFile.split(',').size();

            if(nSize < 3) continue;
            m_stStepFirstRunInfo.nCount = nCount + 1;
            m_stStepFirstRunInfo.astSpeedSteps[nCount].nDirect
                    = strFile.section(',', 0, 0).toInt(); // 方向
            m_stStepFirstRunInfo.astSpeedSteps[nCount].nSpeed
                    = strFile.section(',', 1, 1).toInt();// 代表速度，速度值越小，速度越快  给50是， 1.285KHZ， 200是213HZ
            m_stStepFirstRunInfo.astSpeedSteps[nCount].nStepCount
                    = strFile.section(',', 2, 2).toInt();// 代表段数

//            qDebug() << "------- nCount---------" << nCount;
//            qDebug() << "m_stStepFirstRunInfo nDirect"
//                     << m_stStepFirstRunInfo.astSpeedSteps[nCount].nDirect;
//            qDebug() << "m_stStepFirstRunInfo nSpeed"
//                     << m_stStepFirstRunInfo.astSpeedSteps[nCount].nSpeed;
//            qDebug() << "m_stStepFirstRunInfo nStepCount"
//                     << m_stStepFirstRunInfo.astSpeedSteps[nCount].nStepCount;
        }
        file.close();
    }

    /////////////// 配置采集电机工作走的文件////////////////////
    file.setFileName(MOTOR_1_PATH);
    bIsOpen = file.open(QIODevice::ReadOnly);
//    qDebug() << "open /motor.ini" << bIsOpen;
    if(bIsOpen)
    {
        // 第一行开始读 此时开始最多10行为步进电机扫描时的配置
        for(int nCount = 0;
            (nCount < 10)&&(file.atEnd() == false);
            nCount++)
        {
            QByteArray array = file.readLine();
            QString strFile(array);
            int nSize = strFile.split(',').size();

            if(nSize < 3) continue;
            m_stStepRunInfo.nCount = nCount + 1;
            m_stStepRunInfo.astSpeedSteps[nCount].nDirect
                    = strFile.section(',', 0, 0).toInt(); // 方向
            m_stStepRunInfo.astSpeedSteps[nCount].nSpeed
                    = strFile.section(',', 1, 1).toInt();// 代表速度，速度值越小，速度越快  给50是， 1.285KHZ， 200是213HZ
            m_stStepRunInfo.astSpeedSteps[nCount].nStepCount
                    = strFile.section(',', 2, 2).toInt();// 代表段数

//            qDebug() << "------- nCount---------" << nCount;
//            qDebug() << "m_stStepRunInfo nDirect"
//                     << m_stStepRunInfo.astSpeedSteps[nCount].nDirect;
//            qDebug() << "m_stStepRunInfo nSpeed"
//                     << m_stStepRunInfo.astSpeedSteps[nCount].nSpeed;
//            qDebug() << "m_stStepRunInfo nStepCount"
//                     << m_stStepRunInfo.astSpeedSteps[nCount].nStepCount;
        }
        file.close();
    }

}


// 开机后要让电机走
int CSteppingMotor::SteppingMotorFirstRun()
{
//    STEP_RUN_INFO_T stStepRunInfo = {0};
//    stStepRunInfo.nCount = 1;
//    stStepRunInfo.astSpeedSteps[0].nDirect = m_nDirect;
//    stStepRunInfo.astSpeedSteps[0].nSpeed = m_nSpeed;
//    stStepRunInfo.astSpeedSteps[0].nStepCount = m_nStepCount;
    int nFd = open("/dev/step", O_WRONLY);
#ifdef VER_LINUX
    ioctl(nFd, 1, 0);
    ioctl(nFd, 1, 2);
    ioctl(nFd, 1, 3);

    ioctl(nFd, 3, &m_stStepFirstRunInfo);
    ioctl(nFd, 0, 0);
#endif
    close(nFd);
}

int CSteppingMotor::SteppingMotorRun()
{
    int nFd = open("/dev/step", O_WRONLY);
#ifdef VER_LINUX
    ioctl(nFd, 1, 0);
    ioctl(nFd, 1, 2);
    ioctl(nFd, 1, 3);

    ioctl(nFd, 3, &m_stStepRunInfo);
    ioctl(nFd, 0, 0);
#endif
    close(nFd);
}

int CSteppingMotor::SteppingIsRun() // IOCTL_TIMER_STAUS
{
 // static int SteppingIsRun(); // IOCTL_TIMER_STAUS

// #define IOCTL_TIMER_STAUS		15
// //定时器状态
// #define TIMER_IN_USED    0
// #define TIMER_UN_USED    1
#ifdef VER_LINUX
    int nState = 0;
    int nFd = open("/dev/step", O_WRONLY);
    nState = ioctl(nFd, 15, 0);     //
    close(nFd);
    return nState;
#endif

}

int CSteppingMotor::SteppingStop()
{
#ifdef VER_LINUX
    int nState = 0;
    int nFd = open("/dev/step", O_WRONLY);
    ioctl(nFd, 16, 0);     //
    close(nFd);
    return nState;
#endif
}

// 2#
int CSteppingMotor::Send2jin()
{
#ifdef VER_LINUX
    int nFd = open("/dev/step", O_WRONLY);
    ioctl(nFd, 20, 0);     //
    close(nFd);
    return 0;
#endif
}

void CSteppingMotor::Motor1RunN(STEP_RUN_INFO_T *run)
{
    int nFd = open("/dev/step", O_WRONLY);
#ifdef VER_LINUX
    ioctl(nFd, 1, 0);
    ioctl(nFd, 1, 2);
    ioctl(nFd, 1, 3);

    ioctl(nFd, 3, run);
    ioctl(nFd, 0, 0);
#endif
    close(nFd);
}
