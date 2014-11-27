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
		return NewItemIsGreater; // пропускаем... ищем дальше
	}
	else if(data == new_data)
	{
		return Equal; // пока неизместно что делаем, пока пропускаем
	}
	else
	{
		return NewItemIsLess; // найдет первый старший элемент, новый узел должен быть вставлен в список перед этим элементом.
	}
}


void CreateFirstNode(Node** p_start, int new_data)
{
	// создадим первый (и единственный) элемент связного списка
    Node* new_item = (Node*) malloc(sizeof(Node));
    new_item->value = new_data;

    // устанавливаем указатель на след. элемент у нового элемента на NULL
    new_item->next = NULL;

    // устанавливаем указатель первого элемента списка на созданный элемент
    *p_start = new_item;
}

void InsertNodeFirstPosition(Node** p_start, Node* next_node, int new_data)
{
	// создадим первый (по порядку) элемент связного списка
    Node* new_item = (Node*) malloc(sizeof(Node));
    new_item->value = new_data;

    // устанавливаем указатель на след. элемент списка
    new_item->next = next_node;

    // устанавливаем указатель первого элемента списка на созданный элемент
    *p_start = new_item;
}

void InsertNodeLastPosition(Node *prev_item, int new_data)
{
	// создадим первый (по порядку) элемент связного списка
    Node* new_item = (Node*) malloc(sizeof(Node));
    new_item->value = new_data;

    // устанавливаем указатель у нового элемента на NULL
    new_item->next = NULL;

    // устанавливаем указатель предыдущего элемента на новый (текущий) элемент
    prev_item->next = new_item;
}


void InsertNodeMiddlePosition(Node *prev_item, int new_data)
{
	// создадим новый элемент
    Node *this_item = (Node*) malloc(sizeof(Node));
    this_item->value = new_data;

    // устанавливаем указатель у нового элемента на след. элемент
    this_item->next = prev_item->next;

    // устанавливаем указатель предыдущего элемента на новый (текущий) элемент
    prev_item->next = this_item;
}


int CountItems(Node** p_start)
{
	Node *start = *p_start;	// указатель на первый элемент списка
	Node *this = start;		// указатель на текущий элемент списка
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
	Node *start = *p_start;	// указатель на первый элемент списка
	Node *prev = NULL;		// указатель на элемент списка, предшествующий элементу, по которому срабатывает условие сравнения
	int item;
	result_type res = NotUsed;


	Node *this = start;		// указатель на текущий элемент списка

  // Список пуст, создадим первый элемент:
  if(start == NULL)
  {
	  CreateFirstNode(p_start, new_item);
	  return;
  }

  // Добавим элемент в существующий список:
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
				// Вставляем элемент в середину списка
				InsertNodeMiddlePosition(prev, new_item);
				return;
			}
			else {
				// Вставляем элемент в начало списка
				InsertNodeFirstPosition(p_start, this, new_item);
				return;
			}
			break;

		default:
			;
			break;
	}

	// место для вставки пока не нашлось, ищем дальше:
	prev = this;
	this = (Node*)this->next;
  }

  // Вставляем элемент в конец списка:
  InsertNodeLastPosition(prev, new_item);

}

