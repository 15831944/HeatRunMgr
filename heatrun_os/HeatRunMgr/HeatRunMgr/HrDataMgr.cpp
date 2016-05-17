#include <stdio.h>
#include <string.h>
#include "HeatRunData.h"

static PT_PJDATA_MGR g_ptPjDataMgrHead;

int RegisterPjDataMgr(PT_PJDATA_MGR ptPjDataMgr)  //添加PJ数据链表
{
	PT_PJDATA_MGR ptTmp;
	if (!g_ptPjDataMgrHead)
	{
		g_ptPjDataMgrHead   = ptPjDataMgr;
		ptPjDataMgr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptPjDataMgrHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext = ptPjDataMgr;
		ptPjDataMgr->ptNext = NULL;
	}
	return 0;

}