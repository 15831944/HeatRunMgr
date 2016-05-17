/**********************************************************
		list.c���������������ʼ������ɾ�ƸĲ鼰�ͷ�����ռ䣩
***********************************************************/
#include <stdlib.h>
#include <string.h>
#include "SimpleList.h"

/* ��ʼ������,��������һ���ڵ�*/
pSmpNode SMPLIST_init(void *data)
{
	/* ����һ������ڵ�ṹ���ַ */
	pSmpNode node=(pSmpNode)malloc(sizeof(SmpNode));
	/* ��սڵ�ṹ������ */ 
	memset(node,0,sizeof(SmpNode));
	/*���������Ƿ�NULL��˵��Ҫ�����пռ�����ͷ*/
	if (NULL!=data)
	{
		/* �ڵ��������Ŵ������ݵ�ָ���ַ */
		node->data = data;
	}
	/* �ڵ�(ʵ���Ͼ���һ���ڵ������)βָ��NULL, */
	node->next = NULL;
    return node;
}


/* �������ݵ������� */
short int SMPLIST_addnode(void *data,void *Head)
{
	/* ������ͷָ�븳�����ɱ��� */
	pSmpNode node = (pSmpNode)Head;
	pSmpNode NewNode = NULL;

	/* �����������ݺ�����ͷָ���κ�һ��Ϊ�վͷ���ʧ�� */	  
    if( NULL==data || NULL==Head )
	{
		return 0;
	}

	/* �����ҵ������β�� */
    while(NULL!=node->next)
	{
		node = node->next; 
	}

	/* �����������data��ŵ�������Ľڵ���������� */
    NewNode = (pSmpNode)malloc(sizeof(SmpNode));
	memset(NewNode,0,sizeof(SmpNode));
	NewNode->data = data;
	NewNode->next = NULL;

	/* ������β��nextָ���½ڵ�,������������һ���ڵ�ɹ� */
	node->next = NewNode;
	return 1;
}



/* ��ȡ�������±�Ϊindex�Ľڵ� */
pSmpNode SMPLIST_getnode(int index,void *head)
{
	pSmpNode ListTmp = (pSmpNode)head;    //��ʱ����
	pSmpNode NodeTmp = NULL;           //��ʱ�ڵ�
	int node_num = 0;               //��������ڵ�������

	/* �жϴ�����±�С��0�����������ܽڵ���������NULL */
	if(index<0 || NULL==head || index>SMPLIST_nodesum(head))
	{
		return NULL;
	}
	else
	{
		/* ѭ������ ��� iTemp==index ��ʱ�� �򷵻����Ľڵ� */
		while(NULL!=ListTmp)
		{
			node_num++;
			/* ���������±����봫���Ҫ����ҵ��±������ʱ���������ص�ǰ�ڵ� */
			if(node_num==index)
			{
				NodeTmp = ListTmp;
				break;
			}
			/* ���������±����봫���Ҫ����ҵ��±��������ʱ�����¸��ڵ�ָ�븴�Ƶ���ʱ�������� */
			else
			{
				ListTmp = ListTmp->next;
			}
		}
	}
	return NodeTmp;
}



/* ͳ������Ľڵ���� */
int SMPLIST_nodesum(void *head)
{
	pSmpNode ListTmp = (pSmpNode)head;     //��ʱ����
	int node_sum = 0;                //��������
	/* ����ͷָ��Ϊ���򷵻�0���ڵ� */
	if(NULL==head)
	{
		return node_sum;
	}
	else
	{
		/* ������ʱ����ͬʱ�ۼӼ�������� */
		while(NULL!=ListTmp)
		{
			ListTmp = ListTmp->next;
			node_sum++;
		}
	}
	return node_sum;
}



/* ��������ɾ���±�δindex�Ľڵ� */
pSmpNode SMPLIST_nodedel(int index,pSmpNode head)
{
	int node_num;
	pSmpNode ListTmp = (pSmpNode)head;     //��ʱ����
	pSmpNode NodeTmp = (pSmpNode)head;     //��ʱ�ڵ�
	
	/* �жϴ�����±�С��0�����������ܽڵ���������NULL */
	if(index<=0 || NULL==head || index>SMPLIST_nodesum(head))
	{
		return NULL;
	}
	else
	{
		/* ���ɾ���Ľڵ��±��ǵ�һ���Ͱ�ͷָ��ڶ����ڵ� */
		if(1==index)
		{
			head = ((pSmpNode)head)->next;
#if 1 == LIST_FREEDATA
			free(NodeTmp->data);
#endif
			free(NodeTmp);
		}
		/* ������ǵ�һ���ڵ� */
		else
		{
			/* ���ҵ�Ҫɾ���Ľڵ��ǰһ���ڵ� */
			node_num = index-1;
			NodeTmp = SMPLIST_getnode(node_num,head);
			/* ���ҵ�Ҫɾ���Ľڵ� */
			ListTmp = SMPLIST_getnode(index,head);
			/* ɾ���ڵ��ǰһ���ڵ��nextָ����ɾ���ڵ��next */
			NodeTmp->next = ListTmp->next;
#if 1 == LIST_FREEDATA
			free(ListTmp->data);  
#endif
			free(ListTmp);			
		}		
	}
	return head;
}



/* �ͷ�����ڵ�ռ� */
short int SMPLIST_free(void *head)
{
	pSmpNode ListTmp = NULL;        //��ʱ����
	pSmpNode NodeTmp = NULL;        //��ʱ�ڵ�
	/* ����ͷΪ���򷵻�0 */
	if(head == NULL)
		return 0;
	/* �����������ͷ��ֵ����ʱ���� */
	ListTmp =(pSmpNode)head;
	/* �����������ڶ����ڵ㸳ֵ����ʱ�ڵ� */
	NodeTmp = ListTmp->next;
	//�����ͷţ�ֱ���ͷ������һ���ڵ�
	while(NodeTmp!=NULL)
	{
#if 1 == LIST_FREEDATA
		/* �����ǰ��ʱ�����ڵ����ݲ�Ϊ�գ����ͷŽڵ����ݼ��ڵ�Ŀռ� */
		if(ListTmp->data!=NULL) 
		{
			free(ListTmp->data);
		}
#endif
		free(ListTmp);
		/* ��ԭ����β�������Ƶ���ʱ������ */
		ListTmp = NodeTmp;
		/* ԭ����β�����������һ���ڵ� */
		NodeTmp = NodeTmp->next;
	}

#if 1 == LIST_FREEDATA
	/* ɾ�������ʣ��Ψһһ���ڵ���ͷ�����ڵ� */
	if(ListTmp->data!=NULL) 
	{
		free(ListTmp->data);
	}
#endif
	free(ListTmp);
	return 1;
}

