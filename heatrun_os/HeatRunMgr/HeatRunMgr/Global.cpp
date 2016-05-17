#include <Windows.h>
#include "Global.h"
#include "HeatRunDataFileMgr.h"


//定义全局线程句柄
HANDLE g_hThreadSystemTimer = INVALID_HANDLE_VALUE;
//定义读写老化数据文件线程句柄
HANDLE g_hThreadRWFileData = INVALID_HANDLE_VALUE;
//定义Socktet线程句柄
HANDLE g_hThreadReceData = INVALID_HANDLE_VALUE;
HANDLE g_hThreadSendData = INVALID_HANDLE_VALUE;
HANDLE g_hThreadConnect = INVALID_HANDLE_VALUE;

//刷新线程句柄
HANDLE g_hThreadRefreshData = INVALID_HANDLE_VALUE;

//共享资源互斥全局变量区，留作备用
//定义临界区句柄
//CRITICAL_SECTION g_csCriticalSection;
//定义线程锁句柄
//HANDLE g_hMutex = INVALID_HANDLE_VALUE;

//定义全局老化数据管理链表头
pSmpNode g_pSmpDataMgrListHead = NULL;

//定义服务器版本信息
char *strHrMgrVersion = "11.12.05";   //Socket : OS : FileRW

//定义需要发送给UI的历史数据管理结构体指针
PT_HISTORICAL_DATA g_pHisData = NULL;

bool Refresh_Flag = NULL;

//PT_PJDATA_MGR PjGet[100];//暂时定一个全局变量，以后从链表里读取

PT_PJDATA_MGR PjGet;

fd_set clientfdset; //定义一个全局的socket 嵌套字组