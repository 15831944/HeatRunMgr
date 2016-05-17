#ifndef _HAETRUNRDATAFILEMGR_H_
#define _HAETRUNRDATAFILEMGR_H_
#include "ThreadProcess.h"
#include "HeatRunData.h"

//老化各个数据的长度
#define LENTH_MAC	          (LEN_MAC-1)
#define LENTH_IP	          (LEN_IP-1)
#define LENTH_TYPE	          (LEN_TYPE-1)
#define LENTH_VER	          (LEN_VER-1)
#define LENTH_AUTOPOWEROFF	  2
#define LENTH_LAMPONOFF	      12
#define LENTH_ECOMODE 	      (sizeof(EcoModeSW[0])-1)
#define LENTH_STANDBYMODE	  (sizeof(StandbyModeSW[0])-1)
#define LENTH_ONLINESTATUS	  (sizeof(OnlineStatusSW[0])-1)
#define LENTH_INSTALL         (sizeof(InstallSW[0])-1)
#define LENTH_HEATRUNCYCLE    10
#define LENTH_IMAGEPATTERN    (sizeof(Pattern[0])-1)
#define LENTH_LAMP1TTIME      7
#define LENTH_LAMP2TTIME      7
#define LENTH_RECORDTIME      17

//个老化条件状态数量
#define SUM_PATTERNTYPE       (sizeof(Pattern)/sizeof(Pattern[0]))
#define SUM_ECOMODE           (sizeof(EcoModeSW)/sizeof(EcoModeSW[0]))
#define SUM_STANDBYMODE       (sizeof(StandbyModeSW)/sizeof(StandbyModeSW[0]))
#define SUM_ONLINESTATUS      (sizeof(OnlineStatusSW)/sizeof(OnlineStatusSW[0]))
#define SUM_INSTALLSTATUS     (sizeof(InstallSW)/sizeof(InstallSW[0]))

//Pattern类型数量总数

//数据TXT文件中各个数据的长度
//< MAC | IP | TYPE	| VER |APOFF|LAMPONOFF|LAMPONOFF| ECOMODE |STANDBYMODE|ONLINE| INSTALL |HEATRUNCYCLE|IMAGEPATTERN|LAMP1TTIME|LAMP2TTIME|RECORDTIME>\n
//1+12 +1+15+1+ 15 +1+17 +1+ 2 +1+  12   +1+   12  +1+   6   +1+   6     +1+ 7  +1+   6   +1+   10     +1+  10*10    +1+   7    +1+   7    +1+   17   +1+1
//定义TXT文件中一行数据的长度(一行一共258个字符)，但因申请空间是会影响堆栈，此宏定义方法使用有问题
// #define LENTH_ROWDATA     LENTH_MAC+LENTH_IP+LENTH_TYPE+LENTH_VER+LENTH_AUTOPOWEROFF \
// 	                      +(2*LENTH_LAMPONOFF)+LENTH_ECOMODE+LENTH_STANDBYMODE+LENTH_ONLINESTATUS \
// 						  +LENTH_INSTALL+LENTH_HEATRUNCYCLE+(LENTH_IMAGEPATTERN*SUM_IMAGEPATTERN) \
// 						  +LENTH_LAMP1TTIME+LENTH_LAMP2TTIME+LENTH_RECORDTIME+26  //每一个数据前后都有分割符 所以要加上 1'<'+11'|'+10*1'|'+3'|'+1'>'+1\n
#define LENTH_ROWDATA     268    //每加一个Pattern就是加 “Nothing__|”10位 原258->258+1*10位

#define FLAG_INTERVAL(buf,len,flag)		{ buf[len] = flag;  \
	                                      len += 1;}
#define PSOFFSET(ps,bit)	    (ps + bit)	

//历史文件名二维数组
#define SUM_FILENAME 10            // 和UI沟通好，最多提取7天数据
#define LENTH_FILECHAR 25
//发送给UI的一包数据要包括几台PJ数据
#define SUM_PJ_PER_PACKET    10
//需要发送给UI的缓冲区数据
#define SUM_HISDATA          1024
#define LENTH_DATA_PER_PJ    113            //一条历史数据长度
//2BYTE 历史数据总包数 + 10包*一条历史数据长度(113char) + 预留数据(10char)
#define LENTH_UIFRAMEDATA    (2+SUM_PJ_PER_PACKET*LENTH_DATA_PER_PJ+10) //预留十位数据，以免溢出

//UI查询的历史数据管理结构体
typedef struct
{
	char pQueryStart[LENTH_FILECHAR];                     //查询的开始时间  1y
	char pQueryEnd[LENTH_FILECHAR];                       //查询的结束时间  ly
	char pQueryMac[LEN_MAC];                              //要查询PJ的MAC地址  ly
	char pFileName[SUM_FILENAME][LENTH_FILECHAR];         //需要查找的文件名数组,和UI沟通好，最多提取7天数据
	char pHrData[SUM_HISDATA][LENTH_UIFRAMEDATA];         //需要发送给UI显示的历史老化数据缓存区
	bool bUIQueryHisData;                                 //是否处于UI请求历史数据期间 true处于UI获取历史数据期间 false处于空闲期  ly
	bool bSendFinish;									  //false发送完成/未发送,true正在发送
	int  nSendToUIIndex;                                  //发送给UI的二维数组下标
	int  nSumFileTxt;                                     //查找的文件数量
	int  nSumHrData;                                      //需要发送给UI显示的历史老化数据的包数
	int  nLenCommonData;                                  //数据长度分两种 1.除最后一包数据的数据长度
	int  nLenFinalData;                                  //数据长度分两种 2.最后一包数据的数据长度
}HISTORICAL_DATA,*PT_HISTORICAL_DATA;

extern char Pattern[][10];


/**********************************************************
功	  能: 
		按照当前日期打开或创建文件，同时将链表数据写入文件
参数:		 
		ListHead     — 数据管理链表头
创 建 人:				创建时间: 
		alian					2015.10.28
修 改 人:				修改时间: 
		无					    无
返回值:
		 成功返回0  失败返回?
***********************************************************/
int SaveHrDataToFile( void *ListHead);


/**********************************************************
功	  能: 
		从文件中读取符合要求的老化数据条数
参数:		 
		pHrHisData — 老化历史数据管理结构体
创 建 人:				创建时间: 
		alian					2013.05.30
修 改 人:				修改时间: 
		无					    无
返回值:
		 成功返回0
***********************************************************/
int GetHRDataFromFile( PT_HISTORICAL_DATA pHrHisData );


/**********************************************************
功	  能: 
		根据给出的时间范围读取文件名字到数组中
参数:		 
		pHrHisData — 老化历史数据管理结构体
创 建 人:				创建时间: 
		alian					2013.05.30
修 改 人:				修改时间: 
		无					    无
返回值:
		 成功放回符合要求文件的数量
***********************************************************/
int GetFileNameFromDir( PT_HISTORICAL_DATA pHrHisData );


/**********************************************************
功	  能: 
		从数据中提取IP转化成4位BYTE类型
参数:		 
		pSrcIP   — IP字符串数组指针"192.168.1.101"
		pDstIP   — 转化成char类型
创 建 人:				创建时间: 
		alian					2013.05.30
修 改 人:				修改时间: 
		无					    无
返回值:
		 返回转化成BYTE的位数
***********************************************************/
int GetIPFromData( char *pSrcIP, char *pDstIP );


/**********************************************************
功	  能: 
		从数据中提取VER转化成12位BYTE类型
参数:		 
		pSrcVer   — IP字符串数组指针"20150912 12:36:23"
		pDstVer   — 转化成char类型
创 建 人:				创建时间: 
		alian					2013.05.30
修 改 人:				修改时间: 
		无					    无
返回值:
		 返回转化成BYTE的位数
***********************************************************/
int GetVerFromData( char *pSrcVer, char *pDstVer );


#endif // _HAETRUNRDATAFILEMGR_H_

