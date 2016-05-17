#include <stdio.h>
#include "TimerSoft.h"
#include "SimpleList.h"
#include "HeatRunData.h"


//���ݹ�������ڵ�����
static int stnSumListNode = 0;

extern int  HeatRunTime;

//ע��һ����ʱ��
int TimerSoft_Regedit( void *ListHead, int iTimerId, unsigned char ucAutoReload, int iSetTimeTs, FUN_TIMERSOFT_CALLBACK funCallBack )
{
	// �������ͷָ��Ϊ�վͷ���ʧ��  
	if( NULL==ListHead )
	{
		return 0;
	}

	//����������
	int iResRegedit = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;

	//��ȡ����ڵ������ͬʱ���Ǳ�ģ��ȫ�ֵ�
	stnSumListNode = SMPLIST_nodesum( Head );

	// �ҵ���Ӧ�Ľڵ㣬�����һ��
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//��ȡ�ڵ�����
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);
	
	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		// ���ڵ�Ķ�ʱ����ֵ
		PjDataMgr->m_TimerMgr.ucInStall          = FLAG_OK;
		PjDataMgr->m_TimerMgr.ucStart            = FLAG_NG;
		PjDataMgr->m_TimerMgr.ucAutoReload       = ucAutoReload;
		PjDataMgr->m_TimerMgr.iSetTimeTs         = iSetTimeTs;
		PjDataMgr->m_TimerMgr.iNowTimeTs         = iSetTimeTs;
		PjDataMgr->m_TimerMgr.fTimerSoftCallBack = funCallBack;
		iResRegedit = iTimerId;
	}

	return iResRegedit; //�ɹ�����ע�ᶨʱ������ڵ��±�ʧ�ܷ���-1
}


//����iTimerId��ʱ��
int TimerSoft_Destory( void *ListHead, int iTimerId )
{
	// �������ͷָ��Ϊ�վͷ���ʧ��  
	if( NULL==ListHead )
	{
		return 0;
	}

	//����������
	int iResDestory = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// �ҵ���Ӧ�Ľڵ㣬�����һ��
	NodeTemp = SMPLIST_getnode( stnSumListNode, Head );

	//��ȡ�ڵ�����
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		//iTimerId��ʱ���Ѿ����٣�����-2
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
	return iResDestory;  //���ٳɹ�������ʱ����ţ�ʧ�ܷ���-1
}


//����iTimerId��ʱ��
int TimerSoft_Start( void *ListHead, int iTimerId )
{  	
	// �������ͷָ��Ϊ�վͷ���ʧ��  
	if( NULL==ListHead )
	{
		return 0;
	}

	//����������
	int iResStart = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// �ҵ���Ӧ��ʱ���Ľڵ�
	NodeTemp = SMPLIST_getnode( iTimerId, Head );
	//printf("%d", (PT_PJDATA_MGR)NodeTemp->data )

	//��ȡ�ڵ�����
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		//���iTimerId��ʱ���Ѿ�����������-2
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

	return iResStart; //�����ɹ�������ʱ����ţ�ʧ�ܷ���-1
}


//��ͣiTimerId��ʱ��
int TimerSoft_Stop( void *ListHead, int iTimerId )
{
	// �������ͷָ��Ϊ�վͷ���ʧ��  
	if( NULL==ListHead )
	{
		return 0;
	}

	//����������
	int iResStop = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// �ҵ���Ӧ��ʱ���Ľڵ�
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//��ȡ�ڵ�����
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		//����Ѿ���ͣiTimerId��ʱ��������-2
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

	return iResStop;		//��ͣ�ɹ�������ͣ��ʱ����ţ�ʧ�ܷ���-1
}


//����iTimerId��ʱ��
int TimerSoft_ReStart( void *ListHead, int iTimerId )
{
	// �������ͷָ��Ϊ�վͷ���ʧ��  
	if( NULL==ListHead )
	{
		return 0;
	}

	//����������
	int iResReStart = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// �ҵ���Ӧ��ʱ���Ľڵ�
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//��ȡ�ڵ�����
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		PjDataMgr->m_TimerMgr.iNowTimeTs = PjDataMgr->m_TimerMgr.iSetTimeTs;
		iResReStart = iTimerId;
	}

	return iResReStart;		//�����ɹ�������ʱ����ţ�ʧ�ܷ���-1
}


//�޸���ʱ��nTimerId����ֵ
int TimerSoft_ResetTsTime( void *ListHead, int iTimerId, int iSetTimeTs )
{
	// �������ͷָ��Ϊ�վͷ���ʧ��  
	if( NULL==ListHead )
	{
		return 0;
	}

	//����������
	int iResResetTime = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// �ҵ���Ӧ��ʱ���Ľڵ�
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//��ȡ�ڵ�����
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		PjDataMgr->m_TimerMgr.iSetTimeTs = iSetTimeTs;
		iResResetTime = iTimerId;
	}

	return iResResetTime;	//�ɹ������������õ���ʱ����ţ�ʧ�ܷ���-1
}

//�رջ���iTimerId��ʱ���Զ����ع���
int TimerSoft_SetAutoReLoad( void *ListHead, int iTimerId, unsigned char ucFreeOrSet )
{
	// �������ͷָ��Ϊ�վͷ���ʧ��  
	if( NULL==ListHead )
	{
		return 0;
	}

	//����������
	int iResSetAutoReLoad = -1;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//int stnSumListNode = SMPLIST_nodesum( Head );

	// �ҵ���Ӧ��ʱ���Ľڵ�
	NodeTemp = SMPLIST_getnode( iTimerId, Head );

	//��ȡ�ڵ�����
	PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

	if( stnSumListNode>=iTimerId && iTimerId>=0 )
	{
		if( 0x00 == ucFreeOrSet )	//ȡ���Զ���������
		{
			//����Ѿ�ȡ���˾ͷ���-2
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
		else						//�����Զ���������
		{
			//����Ѿ������˾ͷ���-2
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
	return iResSetAutoReLoad;	//�ɹ������������õ���ʱ����ţ�ʧ�ܷ���-1
}


//��ʱ����������
void TimerSoft_Tick( void *ListHead )
{
	// �������ͷָ��Ϊ�վͷ���ʧ��  
	if( NULL==ListHead )
	{
		return;
	}

	//����������
 	int iTimerSoftNo = 0;
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;

	//�����������е�ʱ����ʱ��������
   	for( iTimerSoftNo=1; iTimerSoftNo<=stnSumListNode; iTimerSoftNo++ )
	{
		// �ҵ���Ӧ��ʱ���Ľڵ�
		NodeTemp = SMPLIST_getnode( iTimerSoftNo, Head );

		//��ȡ�ڵ�����
		PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

		//��ʱ����ע�Ტ����
		if( FLAG_OK == PjDataMgr->m_TimerMgr.ucInStall && FLAG_OK == PjDataMgr->m_TimerMgr.ucStart )
		{
			//ͬʱ����ʱ������ʱ����
			if( 0 == PjDataMgr->m_TimerMgr.iNowTimeTs )
			{
				//������ʱ���жϺ���
				PjDataMgr->m_TimerMgr.fTimerSoftCallBack(iTimerSoftNo);	

				//Ҫ���ؾͼ�������Ҫ��ֹͣ
				if( FLAG_OK == PjDataMgr->m_TimerMgr.ucAutoReload )
				{
					PjDataMgr->m_TimerMgr.iNowTimeTs = PjDataMgr->m_TimerMgr.iSetTimeTs;	
				}
				else
				{
					PjDataMgr->m_TimerMgr.ucStart = 0;			//����ʱ����ʱֹͣ��������������
				}
			}

			//��������
			PjDataMgr->m_TimerMgr.iNowTimeTs--;
		}
	}
}