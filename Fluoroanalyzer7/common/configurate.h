#ifndef CONFIGURATE_H
#define CONFIGURATE_H

//#define DEVICE_TEST //老化版
#if defined(DEVICE_TEST)
#define DT "-老化版"
#else
#define DT
#endif
#define CURR_SYS_VERSIONS "2.5m" DT

#define LONG_END_RESULT //小数点后3位

//#define USE_NEW_FONT

#if defined(USE_NEW_FONT)
#define FONT "msvista"
#define FONTFAMILY  QString("font: " FONT ";")
#define FONT_SIZE_1 "font-size:38px;"
#define FONT_SIZE_2 "font-size:30px;"
#define FONT_SIZE_3 "font-size:28px;"
#define FONT_SIZE_4 "font-size:26px;"
#define FONT_SIZE_5 "font-size:22px;"
#define FONT_SIZE_6 "font-size:24px;"
#define FONT_SIZE_7 "font-size:20px;"
#else
#define FONT "wenquanyi"
#define FONTFAMILY  QString("font: " FONT ";")
#define FONT_SIZE_1 "font-size:36pt;"
#define FONT_SIZE_2 "font-size:24pt;"
#define FONT_SIZE_3 "font-size:16pt;"
#define FONT_SIZE_4 "font-size:14pt;"
#define FONT_SIZE_5 "font-size:10pt;"
#define FONT_SIZE_6 "font-size:12pt;"
#define FONT_SIZE_7 "font-size:8pt;"
#endif

#define DEV_PATH        ("/dev/step")

#define X_BGPIC_PATH    ("/BioTime/picture/")
#define X_PROINFO_PATH  ("/BioTime/proinfo.ini")
#define X_BTDB_PATH     ("/BioTime/biotime.db")
#define X_HISTORY_BATCH ("/BioTime/history.ini")

//#define PET_HOSPITAL    //宠物医院用


/*********************************************
  以下二维码专用
**********************************************/
//#define RUNNINT_PATH    ("/BioTime/running.ini")   // 流水号存放的文件
//#define CONFIG_PATH     ("/BioTime/config.ini")    // 配置信息存放的文件
//#define CF_PATH         ("/BioTime/cf.ini")        // C线荧光阀值
//#define CHECK_PATH      ("/BioTime/check.ini")     // 保存校验值
//#define BASE_PATH       ("/BioTime/base.ini")      // 保存:范围、面积、基底这3个参数
//#define TEST_TIME_PATH  ("/BioTime/time.ini")      // 工程测试时候的时间
//#define MIN_MAX         ("/BioTime/min_max.ini")   // 仪器最终出浓度值的时候，要增加一个最低阀值和最高阀值比较
//#define REFER           ("/BioTime/refer.ini")     // 各个项目参考值
//#define SPRINT_PATH     ("/BioTime/bsprint.ini")   // 保存校验值
//#define SAVE_PROINFO    ("/BioTime/beforpro.ini")  // 保存之前测试过的项目
//#define SN_NUM_PATH     ("/BioTime/snnum.ini")     // 保存sn号


#endif // CONFIGURATE_H
