/*
 * linkedlist.c
 *
 *  Created on: 02.12.2014
 *      Author: Ludmila
 */

// ������������ ���������� ��
#include <stdlib.h>

// ���������� �������� �������
#include "nodes.h"
#include "watch_node_values.h"

// ��������� �������
#include "list_microseconds.h"

// ���������� ���������� -- ����� ����� � ������ ������ ���
NodeBshv* node_bshv_start;

static void CreateLinkedList(NodeBshv** p_start)
{
	// ��������� ���������� �������� �������� ������: ������ � ���������.
	// (����� �������� ����� ����������� � ������� ������ ����� ����������� ����������.)

	// 1. �������� ������ (first) ������� ������:
	// �������� ������ � ��������� ��������
    NodeBshv* first_item = (NodeBshv*) malloc(sizeof(NodeBshv));
    first_item->myBshv.fouryears = WATCH_NODE_VALUE_MIN;
    first_item->myBshv.day = WATCH_NODE_VALUE_MIN;
    first_item->myBshv.hour = WATCH_NODE_VALUE_MIN;
    first_item->myBshv.minute = WATCH_NODE_VALUE_MIN;
    first_item->myBshv.second = WATCH_NODE_VALUE_MIN;
    first_item->ptr = NULL;
    // ���������� ��������� �� ����. ������� � NULL, ����� ����������� �� last_item
    first_item->next = NULL;
    first_item->prev = NULL;
    // ������������� ��������� ������� �������� ������ �� ��������� �������
    *p_start = first_item;

	// 2. �������� ��������� (last) ������� ������:
	// �������� ������ � ��������� ��������
    NodeBshv* last_item = (NodeBshv*) malloc(sizeof(NodeBshv));
    last_item->myBshv.fouryears = WATCH_NODE_VALUE_MAX;
    last_item->myBshv.day = WATCH_NODE_VALUE_MAX;
    last_item->myBshv.hour = WATCH_NODE_VALUE_MAX;
    last_item->myBshv.minute = WATCH_NODE_VALUE_MAX;
    last_item->myBshv.second = WATCH_NODE_VALUE_MAX;
    last_item->ptr = NULL;
    // ���������� ��������� �� ����. ������� � NULL, �. �. ������� ��������� � ������
    last_item->next = NULL;
    last_item->prev = first_item;

    // 3. ��������� �������� first � last
    first_item->next = last_item;
}

static void InsertNodeBshv(NodeBshv *this_item, NodeBshv *new_item)
{
    // ������������� ��������� � ������ �������� �� ��������� � �������������� ��������
	new_item->prev = this_item->prev;

	NodeBshv* prev_item = this_item->prev;
	new_item->next = prev_item->next;

    // ������������� ��������� � ��������������� �������� �� ����� �������
	prev_item->next = new_item;

    // ������������� ��������� � ���������� �������� �� ����� �������
	this_item->prev = new_item;
}


static NodeBshv* CreateNodeBshv(Bshv* b)
{
	NodeBshv* new_item = (NodeBshv*) malloc(sizeof(NodeBshv));
	new_item->myBshv = *b;
	new_item->ptr = NULL;
	new_item->next = NULL;
	new_item->prev = NULL;
	return new_item;
}

void RemoveItemFromNodeBshvList(NodeBshv* this_item)
{
	// ����� �������������� � ��������� ������� ������
	NodeBshv* prev_item = this_item->prev;
	NodeBshv* next_item = this_item->next;

	// ��������� ��������� ��������������� ��������
	prev_item->next = next_item;

	// ��������� ��������� ���������� ��������
	next_item->prev = prev_item;

	// ������� ������� �������
	free(this_item);
	this_item = NULL;
}

NodeBshv* AddNodeBshvItem(NodeBshv** p_start, Bshv* b)
{
	NodeBshv *start = *p_start;	// ��������� �� ������ ������� ������
	NodeBshv *this = start;		// ��������� �� ������� ������� ������
	result_type res = NotUsed;

	if (start == NULL) {return NULL;}	// ������! ������ �� ������

	// ����� �� ���� ��������� ������, ����� ����������
	while(this)
	{
		res = CompareBshv(&(this->myBshv), b);

		switch(res)
		{
			case FirstValueIsLess:
			{
				;	// continue searching
				break;
			}

			case Equal:
			{
				return this;
			}

			case FirstValueIsGreater:
			{
				NodeBshv* new_item = CreateNodeBshv(b);
				InsertNodeBshv(this, new_item);		// ��������� ������� � �������� ������
				return new_item;
			}

			default: return NULL;	// ��������� �����
		}

		// ����� ��� ������� ���� �� �������, ���� ������:
		this = (NodeBshv*)this->next;
	}
	return NULL;
}

int CountTotalItems(NodeBshv* p_start)
{
	NodeBshv *start = p_start;	// ��������� �� ������ ������� ������
	NodeBshv *this = start;		// ��������� �� ������� ������� ������

	// ���������� ������ (����������) ������� ������
	this = this->next;

	// ���������� ���������� �������
	int n = 0;
	while(this->next)
	{
		// ���������� ��������� �� ������ �����������
		NodeMicrosecond* p_mcs = this->ptr;

		//��������� ���������� ��������� � ������� �������
		int k = CountItemsInListMicrosecond(p_mcs);
		n += k;

		// ������������� �� ��������� �������
		this = this->next;
	}

	return n;
}

int CountItemsInListBshv(NodeBshv** p_start)
{
	NodeBshv *start = *p_start;	// ��������� �� ������ ������� ������
	NodeBshv *this = start;		// ��������� �� ������� ������� ������
	int n = 0;
	while(this)
	{
		n++;
		this = (NodeBshv*)this->next;
	}
	if (n<2) { return -1; }	// ������! � ������ ������ ���� ������� ��� ���������� ��������.
	return (n-2);			// n - ���-�� ��������� � ������, �� ������ ���������� ��-���
}


void InitListBshv()
{
	node_bshv_start = NULL;
	CreateLinkedList(&node_bshv_start);
}
