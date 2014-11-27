/*
 * linked_list.c
 *
 *  Created on: 10.10.2014
 *      Author: Ludmila
 */
#include <stdio.h>
#include <stdlib.h>
#include "node.h"

typedef enum {NotUsed, NewItemIsLess, Equal, NewItemIsGreater} result_type;

result_type Compare(unsigned int data, unsigned int new_data)
{
	if (data < new_data)
	{
		return NewItemIsGreater; // ����������... ���� ������
	}
	else if(data == new_data)
	{
		return Equal; // ���� ���������� ��� ������, ���� ����������
	}
	else
	{
		return NewItemIsLess; // ������ ������ ������� �������, ����� ���� ������ ���� �������� � ������ ����� ���� ���������.
	}
}


void CreateFirstNode(Node** p_start, int new_data)
{
	// �������� ������ (� ������������) ������� �������� ������
    Node* new_item = (Node*) malloc(sizeof(Node));
    new_item->value = new_data;

    // ������������� ��������� �� ����. ������� � ������ �������� �� NULL
    new_item->next = NULL;

    // ������������� ��������� ������� �������� ������ �� ��������� �������
    *p_start = new_item;
}

void InsertNodeFirstPosition(Node** p_start, Node* next_node, int new_data)
{
	// �������� ������ (�� �������) ������� �������� ������
    Node* new_item = (Node*) malloc(sizeof(Node));
    new_item->value = new_data;

    // ������������� ��������� �� ����. ������� ������
    new_item->next = next_node;

    // ������������� ��������� ������� �������� ������ �� ��������� �������
    *p_start = new_item;
}

void InsertNodeLastPosition(Node *prev_item, int new_data)
{
	// �������� ������ (�� �������) ������� �������� ������
    Node* new_item = (Node*) malloc(sizeof(Node));
    new_item->value = new_data;

    // ������������� ��������� � ������ �������� �� NULL
    new_item->next = NULL;

    // ������������� ��������� ����������� �������� �� ����� (�������) �������
    prev_item->next = new_item;
}


void InsertNodeMiddlePosition(Node *prev_item, int new_data)
{
	// �������� ����� �������
    Node *this_item = (Node*) malloc(sizeof(Node));
    this_item->value = new_data;

    // ������������� ��������� � ������ �������� �� ����. �������
    this_item->next = prev_item->next;

    // ������������� ��������� ����������� �������� �� ����� (�������) �������
    prev_item->next = this_item;
}


int CountItems(Node** p_start)
{
	Node *start = *p_start;	// ��������� �� ������ ������� ������
	Node *this = start;		// ��������� �� ������� ������� ������
	int n = 0;
	while(this)
	{
		n++;
		this = (Node*)this->next;
	}
	return n;
}

void AddItem(Node** p_start, int new_item)
{
	Node *start = *p_start;	// ��������� �� ������ ������� ������
	Node *prev = NULL;		// ��������� �� ������� ������, �������������� ��������, �� �������� ����������� ������� ���������
	int item;
	result_type res = NotUsed;


	Node *this = start;		// ��������� �� ������� ������� ������

  // ������ ����, �������� ������ �������:
  if(start == NULL)
  {
	  CreateFirstNode(p_start, new_item);
	  return;
  }

  // ������� ������� � ������������ ������:
  while(this) {
	item = this->value;

	res = Compare(item, new_item);
	switch(res)
	{
		case NewItemIsGreater:
			;	// continue searching
			break;

		case Equal:
			;	// replace
			break;

		case NewItemIsLess:
			if (prev != NULL) {
				// ��������� ������� � �������� ������
				InsertNodeMiddlePosition(prev, new_item);
				return;
			}
			else {
				// ��������� ������� � ������ ������
				InsertNodeFirstPosition(p_start, this, new_item);
				return;
			}
			break;

		default:
			;
			break;
	}

	// ����� ��� ������� ���� �� �������, ���� ������:
	prev = this;
	this = (Node*)this->next;
  }

  // ��������� ������� � ����� ������:
  InsertNodeLastPosition(prev, new_item);

}

