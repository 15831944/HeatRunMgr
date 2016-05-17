#ifndef _SAVEHRDATAT0FILE_H_
#define _SAVEHRDATAT0FILE_H_

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

#endif // _SAVEHRDATAT0FILE_H_
