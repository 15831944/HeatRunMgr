#ifndef _THREADPROCESS_H_
#define _THREADPROCESS_H_

/**********************************************************
��	  ��: 
		������ʱ���̺߳���
����:		 
		��
�� �� ��:				����ʱ��: 
		alian					2015.10.28
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0  ʧ�ܷ���?
***********************************************************/
DWORD WINAPI ThreadSystemTimerProcess(LPVOID lpParmeter);


/**********************************************************
��	  ��: 
		�ϻ�ƽ̨�ļ���д���̺߳���
����:		 
		��
�� �� ��:				����ʱ��: 
		alian					2015.10.28
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0  ʧ�ܷ���?
***********************************************************/
DWORD WINAPI ThreadRWMgrFileDataProcess(LPVOID lpParmeter);


DWORD WINAPI ThreadHrDataProcess(LPVOID lpParmeter);


#endif // _THREADPROCESS_H_
