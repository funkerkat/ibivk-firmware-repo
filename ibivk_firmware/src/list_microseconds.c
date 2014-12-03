/*
 * list_microseconds.c
 *
 *  Created on: 03.12.2014
 *      Author: Ludmila
 */

#include <stdlib.h>
#include "list_microseconds.h"

typedef enum {NotUsed, NewItemIsLess, Equal, NewItemIsGreater} result_type;

static result_type Compare(signed int old_mcs, signed int new_mcs)
{
	// �������� ������������
	if ( new_mcs > old_mcs ) { return NewItemIsGreater; }
	if ( new_mcs < old_mcs ) { return NewItemIsLess; }

	// ���� �� ��������� ����� �� ������� �� ������ �� ������� ����, ��� �������� �����:
	return Equal;
}

static void InsertNodeMicrosecond(NodeMicrosecond *this_item, NodeMicrosecond *new_item)
{
    // ������������� ��������� � ������ �������� �� ��������� � �������������� ��������
	new_item->prev = this_item->prev;

	NodeMicrosecond* prev_item = this_item->prev;
	new_item->next = prev_item->next;

    // ������������� ��������� � ��������������� �������� �� ����� �������
	prev_item->next = new_item;

    // ������������� ��������� � ���������� �������� �� ����� �������
	this_item->prev = new_item;
}

static NodeMicrosecond* CreateNodeMicrosecond(signed int mcs, EntryCore1553* entry)
{
	NodeMicrosecond* new_item = (NodeMicrosecond*) malloc(sizeof(NodeMicrosecond));
	new_item->microsecond = mcs;
	new_item->core1553_entry = entry;
	// new_item->timer_value	// ����������� �������� ��� �������
	new_item->next = NULL;
	new_item->prev = NULL;
	return new_item;
}

void RemoveItemFromListMicrosecond(NodeMicrosecond* this_item)
{
	// ����� �������������� � ��������� ������� ������
	NodeMicrosecond* prev_item = this_item->prev;
	NodeMicrosecond* next_item = this_item->next;

	// ��������� ��������� ��������������� ��������
	prev_item->next = next_item;

	// ��������� ��������� ���������� ��������
	next_item->prev = prev_item;

	// ������� ������ ����, ���������� � ���� ��������
	EntryCore1553* entry = this_item->core1553_entry;
	free(entry);

	// ������� ������� �������
	free(this_item);
	this_item = NULL;
}

int AddNodeMicrosecondItem(NodeMicrosecond** p_start, EntryCore1553* entry, signed int mcs)
{
	NodeMicrosecond *start = *p_start;	// ��������� �� ������ ������� ������
	NodeMicrosecond *this = start;		// ��������� �� ������� ������� ������
	result_type res = NotUsed;

	if (start == NULL) {return NULL;}	// ������! ������ �� ������

	// ����� �� ���� ��������� ������, ����� ����������
	while(this)
	{
		res = Compare(this->microsecond, mcs);

		switch(res)
		{
			case NewItemIsGreater:
			{
				;	// continue searching
				break;
			}

			case Equal:
			{
				// �������� ������
				free(this->core1553_entry);
				this->core1553_entry = entry;
				return EXIT_SUCCESS;
			}

			case NewItemIsLess:
			{
				NodeMicrosecond* new_item = CreateNodeMicrosecond(mcs, entry);
				InsertNodeMicrosecond(this, new_item);		// ��������� ������� � �������� ������
				return EXIT_SUCCESS;
			}

			default: return EXIT_FAILURE;	// ��������� �����
		}
		// ����� ��� ������� ���� �� �������, ���� ������:
		this = this->next;
	}
	return EXIT_FAILURE;
}

void DeleteListMicrosecond(NodeMicrosecond** p_start)
{
	NodeMicrosecond *start = *p_start;	// ��������� �� ������ ������� ������
	NodeMicrosecond *this = start;		// ��������� �� ������� ������� ������
	NodeMicrosecond* next_item = NULL;	// ���������� ��� ���������� �������� ��������� �� ����. �������

	while(this)
	{
		// ��������� ��������� �� ��������� �������
		next_item = this->next;
		// ������� ������� �������
		free(this);
		this = NULL;
		// ������������� �� ��������� �������
		this = next_item;
	}
    *p_start = NULL;
}

int CountItemsInListMicrosecond(NodeMicrosecond** p_start)
{
	NodeMicrosecond *start = *p_start;	// ��������� �� ������ ������� ������
	NodeMicrosecond *this = start;		// ��������� �� ������� ������� ������
	int n = 0;
	while(this)
	{
		n++;
		this = this->next;
	}
	if (n<2) { return -1; }	// ������! � ������ ������ ���� ������� ��� ���������� ��������.
	return (n-2);			// n - ���-�� ��������� � ������, �� ������ ���������� ��-���
}

void CreateListMicrosecond(NodeMicrosecond** p_start)
{
	// ��������� ���������� �������� �������� ������: ������ � ���������.
	// (����� �������� ����� ����������� � ������� ������ ����� ����������� ����������.)

	// 1. �������� ������ (first) ������� ������:
	// �������� ������ � ��������� ��������
	NodeMicrosecond* first_item = (NodeMicrosecond*) malloc(sizeof(NodeMicrosecond));
    first_item->microsecond = NODE_MICROSECOND_MIN;

    // ���������� ��������� �� ����. ������� � NULL, ����� ����������� �� last_item
    first_item->next = NULL;
    first_item->prev = NULL;
    // ������������� ��������� ������� �������� ������ �� ��������� �������
    *p_start = first_item;

	// 2. �������� ��������� (last) ������� ������:
	// �������� ������ � ��������� ��������
    NodeMicrosecond* last_item = (NodeMicrosecond*) malloc(sizeof(NodeMicrosecond));
    last_item->microsecond = NODE_MICROSECOND_MAX;

    // ���������� ��������� �� ����. ������� � NULL, �. �. ������� ��������� � ������
    last_item->next = NULL;
    last_item->prev = first_item;

    // 3. ��������� �������� first � last
    first_item->next = last_item;
}
