/*********************************************************************/
/*                            ϵͳ���߳�                             */
/*********************************************************************/
#include <Windows.h>
#include <iostream>
#include "SystemInit.h"
#include "Global.h"
#include "USleep.h"
using namespace std;


int main()
{
	//ϵͳ��ʼ��
	SystemInit();

	printf("PjHeatRunManagerVer: %s\n", strHrMgrVersion);

	while(1)
	{
		usleep(1);
	}

	return 0;
}


