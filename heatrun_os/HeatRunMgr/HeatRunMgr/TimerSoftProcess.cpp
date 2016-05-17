#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "Global.h"
#include "HeatRunData.h"
#include "TimerSoftProcess.h"
#include "SimpleList.h"
#include "SystemInit.h"
#include "PJOperation.h"


void PJ_TSProcess(int iTimerId)
{
	pSmpNode NodeTemp = NULL;	
	PT_PJDATA_MGR PjDataMgrTemp = NULL; 
	NodeTemp = SMPLIST_getnode (iTimerId, g_pSmpDataMgrListHead); //获取iTimerId的节点
	PjDataMgrTemp = (PT_PJDATA_MGR)NodeTemp->data;
	if (PjDataMgrTemp->m_TimerMgr.iNowTimeTs == 0)
	{
		HeatRunOperation(PjDataMgrTemp);
		//ImagePatternOperation(1);
		PjDataMgrTemp->m_TimerMgr.iNowTimeTs = PjDataMgrTemp->m_TimerMgr.iSetTimeTs;
	}
}
