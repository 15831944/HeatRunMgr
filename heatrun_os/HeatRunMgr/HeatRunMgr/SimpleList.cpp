/**********************************************************
		list.c链表操作函数（初始化、增删计改查及释放链表空间）
***********************************************************/
#include <stdlib.h>
#include <string.h>
#include "SimpleList.h"

/* 初始化链表,并建立第一个节点*/
pSmpNode SMPLIST_init(void *data)
{
	/* 申请一个链表节点结构体地址 */
	pSmpNode node=(pSmpNode)malloc(sizeof(SmpNode));
	/* 清空节点结构体内容 */ 
	memset(node,0,sizeof(SmpNode));
	/*如果传入的是非NULL，说明要申请有空间链表头*/
	if (NULL!=data)
	{
		/* 节点数据域存放传入数据的指针地址 */
		node->data = data;
	}
	/* 节点(实际上就是一个节点的链表)尾指向NULL, */
	node->next = NULL;
    return node;
}


/* 增加数据到链表中 */
short int SMPLIST_addnode(void *data,void *Head)
{
	/* 把链表头指针赋给过渡变量 */
	pSmpNode node = (pSmpNode)Head;
	pSmpNode NewNode = NULL;

	/* 如果传入的数据和链表头指针任何一个为空就返回失败 */	  
    if( NULL==data || NULL==Head )
	{
		return 0;
	}

	/* 遍历找到链表的尾巴 */
    while(NULL!=node->next)
	{
		node = node->next; 
	}

	/* 将传入的数据data存放到新申请的节点的数据域中 */
    NewNode = (pSmpNode)malloc(sizeof(SmpNode));
	memset(NewNode,0,sizeof(SmpNode));
	NewNode->data = data;
	NewNode->next = NULL;

	/* 将链表尾的next指向新节点,这样链表就添加一个节点成功 */
	node->next = NewNode;
	return 1;
}



/* 获取链表中下标为index的节点 */
pSmpNode SMPLIST_getnode(int index,void *head)
{
	pSmpNode ListTmp = (pSmpNode)head;    //临时链表
	pSmpNode NodeTmp = NULL;           //临时节点
	int node_num = 0;               //计算链表节点数变量

	/* 判断传入的下标小于0或大于链表的总节点数，返回NULL */
	if(index<0 || NULL==head || index>SMPLIST_nodesum(head))
	{
		return NULL;
	}
	else
	{
		/* 循环编历 如果 iTemp==index 的时候 则返回他的节点 */
		while(NULL!=ListTmp)
		{
			node_num++;
			/* 遍历到得下标数与传入的要求查找的下标数相等时，跳出返回当前节点 */
			if(node_num==index)
			{
				NodeTmp = ListTmp;
				break;
			}
			/* 遍历到得下标数与传入的要求查找的下标数不相等时，将下个节点指针复制到临时链表上来 */
			else
			{
				ListTmp = ListTmp->next;
			}
		}
	}
	return NodeTmp;
}



/* 统计链表的节点个数 */
int SMPLIST_nodesum(void *head)
{
	pSmpNode ListTmp = (pSmpNode)head;     //临时链表
	int node_sum = 0;                //链表总数
	/* 链表头指针为空则返回0个节点 */
	if(NULL==head)
	{
		return node_sum;
	}
	else
	{
		/* 遍历临时链表，同时累加计算结点个数 */
		while(NULL!=ListTmp)
		{
			ListTmp = ListTmp->next;
			node_sum++;
		}
	}
	return node_sum;
}



/* 从链表中删除下标未index的节点 */
pSmpNode SMPLIST_nodedel(int index,pSmpNode head)
{
	int node_num;
	pSmpNode ListTmp = (pSmpNode)head;     //临时链表
	pSmpNode NodeTmp = (pSmpNode)head;     //临时节点
	
	/* 判断传入的下标小于0或大于链表的总节点数，返回NULL */
	if(index<=0 || NULL==head || index>SMPLIST_nodesum(head))
	{
		return NULL;
	}
	else
	{
		/* 如果删除的节点下标是第一个就把头指向第二个节点 */
		if(1==index)
		{
			head = ((pSmpNode)head)->next;
#if 1 == LIST_FREEDATA
			free(NodeTmp->data);
#endif
			free(NodeTmp);
		}
		/* 如果不是第一个节点 */
		else
		{
			/* 先找到要删除的节点的前一个节点 */
			node_num = index-1;
			NodeTmp = SMPLIST_getnode(node_num,head);
			/* 再找到要删除的节点 */
			ListTmp = SMPLIST_getnode(index,head);
			/* 删除节点的前一个节点的next指向了删除节点的next */
			NodeTmp->next = ListTmp->next;
#if 1 == LIST_FREEDATA
			free(ListTmp->data);  
#endif
			free(ListTmp);			
		}		
	}
	return head;
}



/* 释放链表节点空间 */
short int SMPLIST_free(void *head)
{
	pSmpNode ListTmp = NULL;        //临时链表
	pSmpNode NodeTmp = NULL;        //临时节点
	/* 链表头为空则返回0 */
	if(head == NULL)
		return 0;
	/* 将传入的链表头赋值给临时链表 */
	ListTmp =(pSmpNode)head;
	/* 将传入的链表第二个节点赋值给临时节点 */
	NodeTmp = ListTmp->next;
	//遍历释放，直到释放完最后一个节点
	while(NodeTmp!=NULL)
	{
#if 1 == LIST_FREEDATA
		/* 如果当前临时链表内的数据不为空，则释放节点内容及节点的空间 */
		if(ListTmp->data!=NULL) 
		{
			free(ListTmp->data);
		}
#endif
		free(ListTmp);
		/* 将原来的尾部链表复制到临时链表上 */
		ListTmp = NodeTmp;
		/* 原来的尾部链表向后退一个节点 */
		NodeTmp = NodeTmp->next;
	}

#if 1 == LIST_FREEDATA
	/* 删除链表后剩下唯一一个节点后，释放这个节点 */
	if(ListTmp->data!=NULL) 
	{
		free(ListTmp->data);
	}
#endif
	free(ListTmp);
	return 1;
}

