#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <Windows.h>
#include "TimerSoft.h"
#include "SimpleList.h"
#include "HeatRunDataFileMgr.h"

//打印到DOS窗口的调试数据口
//#define DEBUG_DOS

//打印到串口的调试数据口
//#define DEBUG_RS232

//系统心跳时间  1000代表1s  2000代表2s
#define NUM_SYSTEMTICK        1000 

//写老化管理数据到管理文件的周期时间
#define NUM_SAVEHRDATATICK    1000*60*10

//虚构链表数据调试口
//#define DEBUG_MANUAL_INIT

//文件数据提取调试口
//#define DEBUG_READ_HRFILE_DATA

//开启本宏开关表示停止记录老化数据
//#define DEBUG_CLOSE_SAVEDATA

//线程句柄全局化
//系统定时器线程句柄
extern HANDLE g_hThreadSystemTimer;
//读写老化数据文件线程句柄
extern HANDLE g_hThreadRWFileData;
//Socktet线程句柄
extern HANDLE g_hThreadReceData;
extern HANDLE g_hThreadSendData;
extern HANDLE g_hThreadConnect;

//刷新线程句柄
extern HANDLE g_hThreadRefreshData;

//共享资源互斥全局变量区，留作备用
//全局临界区句柄
//extern CRITICAL_SECTION g_csCriticalSection;
//全局线程锁句柄
//extern HANDLE g_hMutex;

//全局老化数据管理链表头
extern pSmpNode g_pSmpDataMgrListHead;

//全局服务器版本信息
extern char *strHrMgrVersion;

//全局需要发送给UI的历史数据管理结构体指针
extern PT_HISTORICAL_DATA g_pHisData;

//extern PT_PJDATA_MGR PjGet[100];
extern PT_PJDATA_MGR PjGet;

extern bool Refresh_Flag;

extern int NUM_PJ;

extern fd_set clientfdset; //定义一个全局的socket 嵌套字组
#endif // _GLOBAL_H_
