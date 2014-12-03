/*
 * linkedlist.c
 *
 *  Created on: 22.10.2014
 *      Author: Ludmila
 */

#include <stdlib.h>
#include "node.h"

typedef enum {NotUsed, FirstValueIsGreater, Equal, FirstValueIsLess} result_type;

static result_type Compare(Node* old_item, Node* new_item)
{
	if ( (old_item->value) < (new_item->value) )
	{
		return FirstValueIsLess; // ����������... ���� ������
	}
	else if( (old_item->value) == (new_item->value) )
	{
		return Equal; // ���� ���������� ��� ������, ���� ����������
	}
	else
	{
		return FirstValueIsGreater; // ������ ������ ������� �������, ����� ���� ������ ���� �������� � ������ ����� ���� ���������.
	}
}

static int CountItemsInLinkedList(Node** p_start)
{
	Node *start = *p_start;	// ��������� �� ������ ������� ������
	Node *this = start;		// ��������� �� ������� ������� ������
	int n = 0;
	while(this)
	{
		n++;
		this = (Node*)this->next;
	}
	if (n<2) { return -1; }	// ������! � ������ ������ ���� ������� ��� ���������� ��������.
	return (n-2);			// n - ���-�� ��������� � ������, �� ������ ���������� ��-���
}

static void CreateLinkedList(Node** p_start)
{
	// ��������� ���������� �������� �������� ������: ������ � ���������.
	// (����� �������� ����� ����������� � ������� ������ ����� ����������� ����������.)

	// 1. �������� ������ (first) ������� ������:
	// �������� ������ � ��������� ��������
    Node* first_item = (Node*) malloc(sizeof(Node));
    first_item->value = -1;
    // ���������� ��������� �� ����. ������� � NULL, ����� ����������� �� last_item
    first_item->next = NULL;
    // ������������� ��������� ������� �������� ������ �� ��������� �������
    *p_start = first_item;

	// 2. �������� ��������� (last) ������� ������:
	// �������� ������ � ��������� ��������
    Node* last_item = (Node*) malloc(sizeof(Node));
    last_item->value = 0x7FFFFFFF;
    // ���������� ��������� �� ����. ������� � NULL, �. �. ������� ��������� � ������
    last_item->next = NULL;

    // 3. ��������� �������� first � last
    first_item->next = last_item;
}

static void InsertNode(Node *prev_item, Node *this_item)
{
    // ������������� ��������� � ������ �������� �� ����. �������
    this_item->next = prev_item->next;

    // ������������� ��������� ����������� �������� �� ����� (�������) �������
    prev_item->next = this_item;
}

static int AddItemToLinkedList(Node** p_start, Node* new_item)
{
	Node *start = *p_start;	// ��������� �� ������ ������� ������
	Node *prev = NULL;		// ��������� �� ������� ������, �������������� ��������, �� �������� ����������� ������� ���������
	result_type res = NotUsed;
	Node *this = start;		// ��������� �� ������� ������� ������

	if (start == NULL) {return EXIT_FAILURE;}	// ������! ������ �� ������

	// ����� �� ���� ��������� ������, ����� ����������
	while(this)
	{
		res = Compare(this, new_item);

		switch(res)
		{
			case FirstValueIsLess:
				;	// continue searching
				break;

			case Equal:
				;	// replace
				break;

			case FirstValueIsGreater:
				InsertNode(prev, new_item);		// ��������� ������� � �������� ������
				return EXIT_SUCCESS;

			default: return EXIT_FAILURE;
		}

		// ����� ��� ������� ���� �� �������, ���� ������:
		prev = this;
		this = (Node*)this->next;
	}

	return EXIT_FAILURE;
}

static Node* CreateItem(int data)
{
    Node* item = (Node*) malloc(sizeof(Node));		// if malloc error, return -1
    item->value = data;
    return item;
}

void LinkedListDemo()
{
	Node* start = NULL;
	Node** p_start = &start;
	CreateLinkedList(p_start);

	Node* new_item1 = CreateItem(0x25);
	AddItemToLinkedList(p_start, new_item1);

	Node* new_item2 = CreateItem(0x96);
	AddItemToLinkedList(p_start, new_item2);

	Node* new_item3 = CreateItem(0x48);
	AddItemToLinkedList(p_start, new_item3);


	int n = CountItemsInLinkedList(p_start);


}
