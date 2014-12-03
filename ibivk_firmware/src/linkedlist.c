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
		return FirstValueIsLess; // пропускаем... ищем дальше
	}
	else if( (old_item->value) == (new_item->value) )
	{
		return Equal; // пока неизместно что делаем, пока пропускаем
	}
	else
	{
		return FirstValueIsGreater; // найдет первый старший элемент, новый узел должен быть вставлен в список перед этим элементом.
	}
}

static int CountItemsInLinkedList(Node** p_start)
{
	Node *start = *p_start;	// указатель на первый элемент списка
	Node *this = start;		// указатель на текущий элемент списка
	int n = 0;
	while(this)
	{
		n++;
		this = (Node*)this->next;
	}
	if (n<2) { return -1; }	// Ошибка! В списке должно быть минимум два сторожевых элемента.
	return (n-2);			// n - кол-во элементов в списке, не считая сторожевых эл-тов
}

static void CreateLinkedList(Node** p_start)
{
	// Создаются сторожевые элементы связного списка: первый и последний.
	// (Новые элементы будут вставляться в связный список между сторожевыми элементами.)

	// 1. Создадим первый (first) элемент списка:
	// выделить память и присвоить значения
    Node* first_item = (Node*) malloc(sizeof(Node));
    first_item->value = -1;
    // установить указатель на след. элемент в NULL, потом переставить на last_item
    first_item->next = NULL;
    // устанавливаем указатель первого элемента списка на созданный элемент
    *p_start = first_item;

	// 2. Создадим последний (last) элемент списка:
	// выделить память и присвоить значения
    Node* last_item = (Node*) malloc(sizeof(Node));
    last_item->value = 0x7FFFFFFF;
    // установить указатель на след. элемент в NULL, т. к. элемент последний в списке
    last_item->next = NULL;

    // 3. связываем элементы first и last
    first_item->next = last_item;
}

static void InsertNode(Node *prev_item, Node *this_item)
{
    // устанавливаем указатель у нового элемента на след. элемент
    this_item->next = prev_item->next;

    // устанавливаем указатель предыдущего элемента на новый (текущий) элемент
    prev_item->next = this_item;
}

static int AddItemToLinkedList(Node** p_start, Node* new_item)
{
	Node *start = *p_start;	// указатель на первый элемент списка
	Node *prev = NULL;		// указатель на элемент списка, предшествующий элементу, по которому срабатывает условие сравнения
	result_type res = NotUsed;
	Node *this = start;		// указатель на текущий элемент списка

	if (start == NULL) {return EXIT_FAILURE;}	// Ошибка! Список не создан

	// поиск по всем элементам списка, кроме сторожевых
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
				InsertNode(prev, new_item);		// Вставляем элемент в середину списка
				return EXIT_SUCCESS;

			default: return EXIT_FAILURE;
		}

		// место для вставки пока не нашлось, ищем дальше:
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
