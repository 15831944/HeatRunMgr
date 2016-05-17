#include <stdio.h>
#include "TimerSoft.h"
#include "SimpleList.h"
#include "HeatRunData.h"


//数据管理链表节点数量
static int stnSumListNode = 0;

extern int  HeatRunTime;

//注册一个软定时器
int TimerSoft_Regedit( void *ListHead, int iTimerId, unsigned char ucAutoReload, int iSetTimeTs, FUN_TIMERSOFT_CALLBACK funCallBack )
{
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return 0;
	}

	//变量定义区
	int iResRegedit = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;

	//获取链表节点个数，同时这是本模块全局的
	stnSumListNode = SMPLIST_nodesum( Head );

	// 找到对应的节点，即最后一个
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//提取节点数据
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);
	
	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		// 给节点的定时器赋值
		PjDataMgr->m_TimerMgr.ucInStall          = FLAG_OK;
		PjDataMgr->m_TimerMgr.ucStart            = FLAG_NG;
		PjDataMgr->m_TimerMgr.ucAutoReload       = ucAutoReload;
		PjDataMgr->m_TimerMgr.iSetTimeTs         = iSetTimeTs;
		PjDataMgr->m_TimerMgr.iNowTimeTs         = iSetTimeTs;
		PjDataMgr->m_TimerMgr.fTimerSoftCallBack = funCallBack;
		iResRegedit = iTimerId;
	}

	return iResRegedit; //成功返回注册定时器链表节点下表，失败返回-1
}


//销毁iTimerId软定时器
int TimerSoft_Destory( void *ListHead, int iTimerId )
{
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return 0;
	}

	//变量定义区
	int iResDestory = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// 找到对应的节点，即最后一个
	NodeTemp = SMPLIST_getnode( stnSumListNode, Head );

	//提取节点数据
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		//iTimerId软定时器已经销毁，返回-2
		if( FLAG_NG == PjDataMgr->m_TimerMgr.ucInStall )
		{
			iResDestory = -2;
		}
		else
		{
			PjDataMgr->m_TimerMgr.ucInStall			= FLAG_NG;
			PjDataMgr->m_TimerMgr.ucStart				= FLAG_NG;
			PjDataMgr->m_TimerMgr.ucAutoReload		= FLAG_NG;
			PjDataMgr->m_TimerMgr.iSetTimeTs			= 0;
			PjDataMgr->m_TimerMgr.iNowTimeTs			= 0;
			PjDataMgr->m_TimerMgr.fTimerSoftCallBack	= NULL;
			iResDestory = iTimerId;
		}
	}
	return iResDestory;  //销毁成功返回软定时器编号，失败返回-1
}


//启动iTimerId软定时器
int TimerSoft_Start( void *ListHead, int iTimerId )
{  	
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return 0;
	}

	//变量定义区
	int iResStart = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// 找到对应定时器的节点
	NodeTemp = SMPLIST_getnode( iTimerId, Head );
	//printf("%d", (PT_PJDATA_MGR)NodeTemp->data )

	//提取节点数据
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		//如果iTimerId软定时器已经启动，返回-2
		if( FLAG_OK == PjDataMgr->m_TimerMgr.ucStart )
		{
			iResStart = -2;
		}
		else
		{
			PjDataMgr->m_TimerMgr.ucStart	= FLAG_OK;
			iResStart = iTimerId;
		}
	}

	return iResStart; //启动成功返回软定时器编号，失败返回-1
}


//暂停iTimerId软定时器
int TimerSoft_Stop( void *ListHead, int iTimerId )
{
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return 0;
	}

	//变量定义区
	int iResStop = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// 找到对应定时器的节点
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//提取节点数据
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		//如果已经暂停iTimerId软定时器，返回-2
		if( FLAG_NG == PjDataMgr->m_TimerMgr.ucStart )
		{
			iResStop = -2;
		}
		else
		{
			PjDataMgr->m_TimerMgr.ucStart = FLAG_NG;
			iResStop = iTimerId;
		}
	}

	return iResStop;		//暂停成功返回暂停软定时器编号，失败返回-1
}


//重启iTimerId软定时器
int TimerSoft_ReStart( void *ListHead, int iTimerId )
{
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return 0;
	}

	//变量定义区
	int iResReStart = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// 找到对应定时器的节点
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//提取节点数据
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		PjDataMgr->m_TimerMgr.iNowTimeTs = PjDataMgr->m_TimerMgr.iSetTimeTs;
		iResReStart = iTimerId;
	}

	return iResReStart;		//重启成功返回软定时器编号，失败返回-1
}


//修改软定时器nTimerId计数值
int TimerSoft_ResetTsTime( void *ListHead, int iTimerId, int iSetTimeTs )
{
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return 0;
	}

	//变量定义区
	int iResResetTime = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// 找到对应定时器的节点
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//提取节点数据
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		PjDataMgr->m_TimerMgr.iSetTimeTs = iSetTimeTs;
		iResResetTime = iTimerId;
	}

	return iResResetTime;	//成功返回重新设置的软定时器编号，失败返回-1
}

//关闭或开启iTimerId软定时器自动重载功能
int TimerSoft_SetAutoReLoad( void *ListHead, int iTimerId, unsigned char ucFreeOrSet )
{
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return 0;
	}

	//变量定义区
	int iResSetAutoReLoad = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// 找到对应定时器的节点
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//提取节点数据
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		if( 0x00 == ucFreeOrSet )	//取消自动重载设置
		{
			//如果已经取消了就返回-2
			if( FLAG_NG == PjDataMgr->m_TimerMgr.ucAutoReload )
			{
				iResSetAutoReLoad = -2;
			}
			else
			{
				PjDataMgr->m_TimerMgr.ucAutoReload = FLAG_NG;
				iResSetAutoReLoad = iTimerId;
			}
		}
		else						//设置自动重载设置
		{
			//如果已经设置了就返回-2
			if( FLAG_OK == PjDataMgr->m_TimerMgr.ucAutoReload )
			{
				iResSetAutoReLoad = -2;
			}
			else
			{
				PjDataMgr->m_TimerMgr.ucAutoReload = FLAG_OK;
				iResSetAutoReLoad = iTimerId;
			}
		}
	}
	return iResSetAutoReLoad;	//成功返回重新设置的软定时器编号，失败返回-1
}


//软定时器心跳函数
void TimerSoft_Tick( void *ListHead )
{
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return;
	}

	//变量定义区
 	int iTimerSoftNo = 0;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;

	//遍历链表，运行到时的软定时器处理函数
   	for( iTimerSoftNo=1; iTimerSoftNo<=stnSumListNode; iTimerSoftNo++ )
	{
		// 找到对应定时器的节点
		NodeTemp = SMPLIST_getnode( iTimerSoftNo, Head );

		//提取节点数据
		PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

		//软定时器已注册并启动
		if( FLAG_OK == PjDataMgr->m_TimerMgr.ucInStall && FLAG_OK == PjDataMgr->m_TimerMgr.ucStart )
		{
			//同时本软定时器倒计时结束
			if( 0 == PjDataMgr->m_TimerMgr.iNowTimeTs )
			{
				//调用软定时器中断函数
				PjDataMgr->m_TimerMgr.fTimerSoftCallBack(iTimerSoftNo);	

				//要重载就继续，不要就停止
				if( FLAG_OK == PjDataMgr->m_TimerMgr.ucAutoReload )
				{
					PjDataMgr->m_TimerMgr.iNowTimeTs = PjDataMgr->m_TimerMgr.iSetTimeTs;	
				}
				else
				{
					PjDataMgr->m_TimerMgr.ucStart = 0;			//本软定时器暂时停止，可以重新启动
				}
			}

			//心跳节奏
			PjDataMgr->m_TimerMgr.iNowTimeTs--;
		}
	}
}