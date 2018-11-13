#ifndef PACKET_DEF_H
#define PACKET_DEF_H
#include <QtGui>
#include <QtCore>

#define DATA_MAX_SIZE   (600) // 数据的组数

#pragma pack (4)
typedef enum tagPCK_TYPE_E
{
    DAFAULT,
    RECORD_UP,      // ARM 记录上传
    DATA_350,       // ARM 把350组数据上传
    CONFIG_DOWM,    // PC 把参数下载到ARM
    CONFIG_UP,      // PC 获取ARM参数
    DOWNLOAD,       // PC 通知下位机上传记录(对于PC下载功能)
    SCANNING,       // 扫描
    SCAN_TIME,       // 定时扫描
    UART_CONNECT,    // PC发送给ARM表示机请求连接 ARM发送给PC表示连接成功
    UART_DISCONNECT,  // PC发送给ARM表示机断开连接 ARM发送给PC表示断开成功
    SUCCESS,          // 接收成功 可以发送下一帧数据
    FAIL              // 接收失败 重发数据 在帧体中的nIdx表示接收失败的帧编号
}PCK_TYPE_E;

typedef struct tagPCK_HEAD_T
{
    unsigned int    nIdx;   //
    unsigned int    nSum;
    unsigned int    nCheck;
    PCK_TYPE_E      eType;  // 帧类型
}PCK_HEAD_T, *P_PCK_HEAD_T;


// ARM 记录上传
typedef struct tagPCK_TO_PC_T
{
    int    nRunning;
    char   acDate[12];
    char   acTime[12];
    int    nResult;
    char   acUnit[8];
    char   acProject[4];
    int    nTEnergy; // T线能量值的100倍
    int    nCEnergy; // C线能量值的100倍
}PCK_TO_PC_T, *P_PCK_TO_PC_T;

#define     ONE_PCK_RECORD  (200)
// ARM 把350组数据上传200组一次
typedef struct tagTEN_RECORD_T
{
    unsigned int anRecord[ONE_PCK_RECORD];
    long long nC1;
    long long nC2;
    long long nT1;
    long long nT2;
}TEN_RECORD_T, *P_TEN_RECORD_T;



//  参数配置 PC发给ARM 或者PC机自己管理参数用
typedef struct tagCONFIG_T
{
    int nC_1;
    int nC_2;
    int nC_3;
    int nT1_1;
    int nT1_2;
    int nT1_3;
    int nT2_2;
    int nT2_3;
    int nT3_3;
    int nLimit;  // 范围
    int nArea;   // 面积
    int nBase;   // 基底
    int nOffset; // 偏差
    int nStdVar; // 标准值
    int nHCG;    //
    int nAFP;
    int nTSH;
    int nFSH;
    int nCRP;
    int nLH;

    int nBaseState; // 本地的计算方式 1 2 3
    int nApos;
    int nBpos;
}CONFIG_T, *P_CONFIG_T;

// 定时扫描时间
typedef struct tagSCAN_TIME_T
{
    int nTime;
}SCAN_TIME_T, *P_SCAN_TIME_T;

// 流水号范围(pc 要求arm上传流水号范围)
typedef struct tagRUNNING_LIMITS_T
{
    int nStatr;
    int nEnd;
}RUNNING_LIMITS_T, *P_RUNNING_LIMITS_T;


typedef union tagPCK_BODY_U
{
    PCK_TO_PC_T      stToPC;
    CONFIG_T         stConfig;
    SCAN_TIME_T      stTime;
    TEN_RECORD_T     stTenRecord;
    RUNNING_LIMITS_T stRunLimits;
}PCK_BODY_U, *P_PCK_BODY_U;

// char       cStart; // 0x23
typedef struct tagPACKET_T
{
    PCK_HEAD_T stHead;
    PCK_BODY_U unBody;
}PACKET_T, *P_PACKET_T;
// char       cEnd; // 0x24




////////////////////////////////////////////////////////////


#endif // PACKET_DEF_H
