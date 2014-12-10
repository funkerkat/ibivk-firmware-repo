/*
 * list_microseconds.c
 *
 *  Created on: 03.12.2014
 *      Author: Ludmila
 */

// ������������ ���������� ��
#include <stdlib.h>

// ���������� �������� �������
#include "nodes.h"
#include "watch_node_values.h"

// ��������� �������
#include "list_bshv.h"

NodeMicrosecond* global_microsecond;

static result_type Compare(signed int old_mcs, signed int new_mcs)
{
	// �������� ������������
	if ( new_mcs > old_mcs ) { return FirstValueIsLess; }
	if ( new_mcs < old_mcs ) { return FirstValueIsGreater; }

	// ���� �� ��������� ����� �� ������� �� ������ �� ������� ����, ��� �������� �����:
	return Equal;
}


static void SetTimerValues(NodeMicrosecond *this_item)
{
	NodeMicrosecond* prev_item = this_item->prev;
	NodeMicrosecond* next_item = this_item->next;


	// ���� ������� ������� -- ������ � ������ (����� �����������):
	if(prev_item->prev == NULL)
	{
		prev_item->timer_value = this_item->microsecond;
	}
	else
	{
		prev_item->timer_value = this_item->microsecond - prev_item->microsecond;
	}


	// ���� ������� ������� -- ��������� � ������ (�� ������ �����������):
	if (next_item->next == NULL)
	{
		this_item->timer_value = 0;
	}
	else
	{
		this_item->timer_value = next_item->microsecond - this_item->microsecond;
	}


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

	// ����������� �������� �������
	SetTimerValues(new_item);

}

static NodeMicrosecond* CreateNodeMicrosecond(signed int mcs, EntryCore1553* entry, NodeBshv* thisNodeBshv)
{
	NodeMicrosecond* new_item = (NodeMicrosecond*) malloc(sizeof(NodeMicrosecond));
	new_item->base_node_bshv = thisNodeBshv;
	new_item->microsecond = mcs;
	new_item->core1553_entry = entry;
	new_item->next = NULL;
	new_item->prev = NULL;
	return new_item;
}

int CountItemsInListMicrosecondNew(NodeMicrosecond* p_start)
{
	NodeMicrosecond *this = p_start;		// ��������� �� ������� ������� ������
	int n = 0;
	while(this)
	{
		n++;
		this = this->next;
	}
	if (n<2) { return -1; }	// ������! � ������ ������ ���� ������� ��� (����������) ��������.
	return (n-2);			// n - ���-�� ��������� � ������, �� ������ ���������� ��-���
}

/*
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
*/
void RemoveItemFromListMicrosecond(NodeMicrosecond* this_item)
{
	// �������� �� ������� ������ �� ������� ������� ��� � ����� ��������� � ������ �����������
	NodeBshv* base_bshv = this_item->base_node_bshv;
	NodeMicrosecond* p_enter = base_bshv->ptr;

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

	// ��������� ���������� ��������� � ������ �� ������� �������
	unsigned int n = CountItemsInListMicrosecondNew(p_enter);

	// ���� �� ������� ������� ��� �������, ������� ������� ������� ��� �� ������ ������
	if (n == 0)
	{
		RemoveItemFromNodeBshvList(base_bshv);
	}
}

int AddNodeMicrosecondItem(NodeMicrosecond** p_start, EntryCore1553* entry, signed int mcs, NodeBshv* thisNodeBshv)
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
			case FirstValueIsLess:
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

			case FirstValueIsGreater:
			{
				NodeMicrosecond* new_item = CreateNodeMicrosecond(mcs, entry, thisNodeBshv);
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

/*
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
	if (n<2) { return -1; }	// ������! � ������ ������ ���� ������� ��� (����������) ��������.
	return (n-2);			// n - ���-�� ��������� � ������, �� ������ ���������� ��-���
}
*/

void CreateListMicrosecond(NodeMicrosecond** p_start)
{
	// ��������� ���������� �������� �������� ������: ������ � ���������.
	// (����� �������� ����� ����������� � ������� ������ ����� ����������� ����������.)

	// 1. �������� ������ (first) ������� ������:
	// �������� ������ � ��������� ��������
	NodeMicrosecond* first_item = (NodeMicrosecond*) malloc(sizeof(NodeMicrosecond));
    first_item->microsecond = WATCH_NODE_VALUE_MIN;
    first_item->timer_value = 0;

    // ���������� ��������� �� ����. ������� � NULL, ����� ����������� �� last_item
    first_item->next = NULL;
    first_item->prev = NULL;
    // ������������� ��������� ������� �������� ������ �� ��������� �������
    *p_start = first_item;

	// 2. �������� ��������� (last) ������� ������:
	// �������� ������ � ��������� ��������
    NodeMicrosecond* last_item = (NodeMicrosecond*) malloc(sizeof(NodeMicrosecond));
    last_item->microsecond = WATCH_NODE_VALUE_MAX;
    last_item->timer_value = 0;

    // ���������� ��������� �� ����. ������� � NULL, �. �. ������� ��������� � ������
    last_item->next = NULL;
    last_item->prev = first_item;

    // 3. ��������� �������� first � last
    first_item->next = last_item;
}

