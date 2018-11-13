#include "SerialPort.h"
#include <QtGui>
#include <QString>

// 串口0 扫描枪
// 串口1 条形码扫描模块
// 串口2 打印机和PC
// 串口3 单片机430采集模块
#ifdef VER_WINDOWS
QextSerialPort * CSerialPort::s_pUart0 = new QextSerialPort("COM1");
QextSerialPort * CSerialPort::s_pUart1 = new QextSerialPort("COM3");
QextSerialPort * CSerialPort::s_pUart2 = new QextSerialPort("COM7"); //
QextSerialPort * CSerialPort::s_pUart3 = new QextSerialPort("COM4");

#else
QextSerialPort * CSerialPort::s_pUart0 = new QextSerialPort("/dev/ttySAC0");
QextSerialPort * CSerialPort::s_pUart1 = new QextSerialPort("/dev/ttySAC1");
QextSerialPort * CSerialPort::s_pUart2 = new QextSerialPort("/dev/ttySAC2");
QextSerialPort * CSerialPort::s_pUart3 = new QextSerialPort("/dev/ttySAC3");
#endif


CSerialPort::CSerialPort()
{
}


// 获得串口1指针
QextSerialPort * CSerialPort::GetSerialPort(UART_TYPE eType)
{
    switch(eType)
    {
    case UART_0:               return s_pUart0; break;
    case UART_1_PC_PRINTER:    return s_pUart2; break;
    case UART_2_SCAN:          return s_pUart1; break;
    case UART_3_430:           return s_pUart3; break;
    default:
        break;
    }

    return NULL;
}

bool CSerialPort::OpenSerialPort(UART_TYPE eType)
{
    bool bOpen = false;

    // 串口0扫描枪 串口1条形码扫描模块 串口2打印机和PC 串口3单片机430采集模块
    switch(eType)
    {
    case UART_0:
    {
        bOpen = initUART0();
    }
    case UART_1_PC_PRINTER:
    {
        bOpen = initUART2();
        break;
    }
    case UART_2_SCAN:
    {
        bOpen = initUART1();
        break;
    }
    case UART_3_430:
    {
        bOpen = initUART3();
        break;
    }
    default:
        break;
    }

   return bOpen;
}

bool CSerialPort::initUART0(BaudRateType baudRate)
{
    bool bOpen = false;
    QextSerialPort * pUart;
    pUart = s_pUart0;
    bOpen = pUart->open(QIODevice::ReadWrite);
    //以读写方式打开串口

    pUart->setBaudRate(baudRate);
    //波特率设置，我们设置为9600

    pUart->setDataBits(DATA_8);
    //数据位设置，我们设置为8位数据位

    pUart->setParity(PAR_NONE);
    //奇偶校验设置，我们设置为无校验

    pUart->setStopBits(STOP_1);
    //停止位设置，我们设置为1位停止位

    pUart->setFlowControl(FLOW_OFF);
    //数据流控制设置，我们设置为无数据流控制

    pUart->setTimeout(20);
    //延时设置，我们设置为延时20ms
    return bOpen;
}

bool CSerialPort::initUART1(BaudRateType baudRate)
{
    bool bOpen = false;
    QextSerialPort * pUart;
    pUart = s_pUart1;
    bOpen = pUart->open(QIODevice::ReadWrite);
    //以读写方式打开串口

    pUart->setBaudRate(baudRate);
    //波特率设置，我们设置为9600

    pUart->setDataBits(DATA_8);
    //数据位设置，我们设置为8位数据位

    pUart->setParity(PAR_NONE);
    //奇偶校验设置，我们设置为无校验

    pUart->setStopBits(STOP_1);
    //停止位设置，我们设置为1位停止位

    pUart->setFlowControl(FLOW_OFF);
    //数据流控制设置，我们设置为无数据流控制

    pUart->setTimeout(20);
    //延时设置，我们设置为延时20ms
    return bOpen;
}

bool CSerialPort::initUART2(BaudRateType baudRate)
{
    bool bOpen = false;
    QextSerialPort * pUart;
    pUart = s_pUart2;
    bOpen = pUart->open(QIODevice::ReadWrite);
    //以读写方式打开串口

    pUart->setBaudRate(BAUD115200);
    //波特率设置，我们设置为9600

    pUart->setDataBits(DATA_8);
    //数据位设置，我们设置为8位数据位

    pUart->setParity(PAR_NONE);
    //奇偶校验设置，我们设置为无校验

    pUart->setStopBits(STOP_1);
    //停止位设置，我们设置为1位停止位

    pUart->setFlowControl(FLOW_OFF);
    //数据流控制设置，我们设置为无数据流控制

    pUart->setTimeout(20);
    //延时设置，我们设置为延时20ms
    return bOpen;
}

bool CSerialPort::initUART3(BaudRateType baudRate)
{
    bool bOpen = false;
    QextSerialPort * pUart;
    pUart = s_pUart3;
    bOpen = pUart->open(QIODevice::ReadWrite);
    //以读写方式打开串口

    pUart->setBaudRate(baudRate);
    //波特率设置，我们设置为9600

    pUart->setDataBits(DATA_8);
    //数据位设置，我们设置为8位数据位

    pUart->setParity(PAR_NONE);
    //奇偶校验设置，我们设置为无校验

    pUart->setStopBits(STOP_1);
    //停止位设置，我们设置为1位停止位

    pUart->setFlowControl(FLOW_OFF);
    //数据流控制设置，我们设置为无数据流控制

    pUart->setTimeout(20);
    //延时设置，我们设置为延时20ms
    return bOpen;
}
