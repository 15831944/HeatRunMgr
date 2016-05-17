#ifndef _SYSTEMINIT_H_
#define _SYSTEMINIT_H_
#include "HeatRunData.h"

/**********************************************************
��	  ��: 
		ϵͳ��ʼ������
����:		 
		��
�� �� ��:				����ʱ��: 
		alian					2015.10.28
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0  ʧ�ܷ���?
***********************************************************/
int SystemInit();


/**********************************************************
��	  ��: 
		ϵͳ���в�����ʼ������
����:		 
		��
�� �� ��:				����ʱ��: 
		alian					2015.10.28
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0
***********************************************************/
int SystemParamInit();


/**********************************************************
��	  ��: 
		�߳�����ʼ������
����:		 
		��
�� �� ��:				����ʱ��: 
		alian					2015.10.28
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0  ʧ�ܷ���?
***********************************************************/
//������Դ����ȫ�ֱ���������������
//int MutexInit();


/**********************************************************
��	  ��: 
		ϵͳ�ٽ�����ʼ������
����:		 
		��
�� �� ��:				����ʱ��: 
		alian					2015.10.28
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0  ʧ�ܷ���?
***********************************************************/
//������Դ����ȫ�ֱ���������������
//int CriticalSectionInit();


/**********************************************************
��	  ��: 
		ϵͳ�̳߳�ʼ������
����:		 
		��
�� �� ��:				����ʱ��: 
		alian					2015.10.28
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0  ʧ�ܷ���?
***********************************************************/
int ThreadInit();


/**********************************************************
��	  ��: 
		ͨ���ֶ������������ݺ�����Ϊ���ݴ洢����ʹ��
����:		 
		��
�� �� ��:				����ʱ��: 
		alian					2015.10.28
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0
***********************************************************/
void HeatRunDataInit(void);


/**********************************************************
��	  ��: 
		��ʼ��һ̨ͶӰ���ϻ����ݺ���
����:
		pDataMgr      �� ��Ҫ��ʼ�����ϻ���������ָ��
		nIndex        �� ��Ҫ��ʼ���Ĺ��������±�
�� �� ��:				����ʱ��: 
		alian					2015.11.3
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 ��
***********************************************************/
int PJHeatRunParamInit( PT_PJDATA_MGR pDataMgr, int nIndex );


int PjGetBufferInit();


#endif // _SYSTEMINIT_H_