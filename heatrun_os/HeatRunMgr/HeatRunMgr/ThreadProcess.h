#ifndef _THREADPROCESS_H_
#define _THREADPROCESS_H_

/**********************************************************
功	  能: 
		管理软定时器线程函数
参数:		 
		无
创 建 人:				创建时间: 
		alian					2015.10.28
修 改 人:				修改时间: 
		无					    无
返回值:
		 成功返回0  失败返回?
***********************************************************/
DWORD WINAPI ThreadSystemTimerProcess(LPVOID lpParmeter);


/**********************************************************
功	  能: 
		老化平台文件读写的线程函数
参数:		 
		无
创 建 人:				创建时间: 
		alian					2015.10.28
修 改 人:				修改时间: 
		无					    无
返回值:
		 成功返回0  失败返回?
***********************************************************/
DWORD WINAPI ThreadRWMgrFileDataProcess(LPVOID lpParmeter);


DWORD WINAPI ThreadHrDataProcess(LPVOID lpParmeter);


#endif // _THREADPROCESS_H_
