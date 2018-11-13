#ifndef XPRINTF_H
#define XPRINTF_H
#include "SerialPort/SerialPort.h"
#include "global_define.h"
#include "argdecode.h"



class CXPrintf
{
public:
    CXPrintf();
    void Send_printf_com(char arg1);
    void Send_printf_com(char arg1,char arg2);
    void Send_printf_com(char arg1,char arg2,char arg3);
    void Send_printf_com(char arg1,char arg2,char arg3,char arg4);
    void Send_printf_com(char arg1,char arg2,char arg3,char arg4,char arg5);

    unsigned char Get_printf_status();
    void Printf_string(QString str);//打印字符串
    void Printf_ENTRN(); //换行

    QString convert_3str(QString str1,QString str2,QString str3);//三个字符串转成一行 居中对齐
    QString convert_res(int res,PF_DATA *proinfo); //将结果转换成要显示的
    void Printf_res(QString name, QString res, QString unit);

    void Set_printfdata(RES_DATA *data);
    void Printf_topage();
    void Printf_topage(RES_DATA *data);
    bool Get_bprintf();
    void set_bpfrefer(bool pf);
private:
    bool m_bprintf;
    bool m_bpfrefer;
    QextSerialPort *m_pUartprintf; // 串口指针
    RES_DATA m_data;

};

#endif // XPRINTF_H
