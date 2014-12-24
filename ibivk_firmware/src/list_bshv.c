/*
 * linkedlist.c
 *
 *  Created on: 02.12.2014
 *      Author: Ludmila
 */

// страндартная библиотека Си
#include <stdlib.h>

// библиотеки текущего проекта
#include "nodes.h"
#include "watch_node_values.h"

// прототипы функций
#include "list_microseconds.h"

// глобальная переменная -- точка входа в список секунд БШВ
NodeBshv* node_bshv_start;

static void CreateLinkedList(NodeBshv** p_start)
{
	// Создаются сторожевые элементы связного списка: первый и последний.
	// (Новые элементы будут вставляться в связный список между сторожевыми элементами.)

	// 1. Создадим первый (first) элемент списка:
	// выделить память и присвоить значения
    NodeBshv* first_item = (NodeBshv*) malloc(sizeof(NodeBshv));
    first_item->myBshv.fouryears = WATCH_NODE_VALUE_MIN;
    first_item->myBshv.day = WATCH_NODE_VALUE_MIN;
    first_item->myBshv.hour = WATCH_NODE_VALUE_MIN;
    first_item->myBshv.minute = WATCH_NODE_VALUE_MIN;
    first_item->myBshv.second = WATCH_NODE_VALUE_MIN;
    first_item->ptr = NULL;
    // установить указатель на след. элемент в NULL, потом переставить на last_item
    first_item->next = NULL;
    first_item->prev = NULL;
    // устанавливаем указатель первого элемента списка на созданный элемент
    *p_start = first_item;

	// 2. Создадим последний (last) элемент списка:
	// выделить память и присвоить значения
    NodeBshv* last_item = (NodeBshv*) malloc(sizeof(NodeBshv));
    last_item->myBshv.fouryears = WATCH_NODE_VALUE_MAX;
    last_item->myBshv.day = WATCH_NODE_VALUE_MAX;
    last_item->myBshv.hour = WATCH_NODE_VALUE_MAX;
    last_item->myBshv.minute = WATCH_NODE_VALUE_MAX;
    last_item->myBshv.second = WATCH_NODE_VALUE_MAX;
    last_item->ptr = NULL;
    // установить указатель на след. элемент в NULL, т. к. элемент последний в списке
    last_item->next = NULL;
    last_item->prev = first_item;

    // 3. связываем элементы first и last
    first_item->next = last_item;
}

static void InsertNodeBshv(NodeBshv *this_item, NodeBshv *new_item)
{
    // устанавливаем указатели у нового элемента на следующий и предшествующий элементы
	new_item->prev = this_item->prev;

	NodeBshv* prev_item = this_item->prev;
	new_item->next = prev_item->next;

    // устанавливаем указатель у предшествующего элемента на новый элемент
	prev_item->next = new_item;

    // устанавливаем указатель у следующего элемента на новый элемент
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
	// найти предшествующий и следующий элемент списка
	NodeBshv* prev_item = this_item->prev;
	NodeBshv* next_item = this_item->next;

	// исправить указатели предшествующего элемента
	prev_item->next = next_item;

	// исправить указатели следующего элемента
	next_item->prev = prev_item;

	// удалить текущий элемент
	free(this_item);
	this_item = NULL;
}

NodeBshv* AddNodeBshvItem(NodeBshv** p_start, Bshv* b)
{
	NodeBshv *start = *p_start;	// указатель на первый элемент списка
	NodeBshv *this = start;		// указатель на текущий элемент списка
	result_type res = NotUsed;

	if (start == NULL) {return NULL;}	// Ошибка! Список не создан

	// поиск по всем элементам списка, кроме сторожевых
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
				InsertNodeBshv(this, new_item);		// Вставляем элемент в середину списка
				return new_item;
			}

			default: return NULL;	// Аварийный выход
		}

		// место для вставки пока не нашлось, ищем дальше:
		this = (NodeBshv*)this->next;
	}
	return NULL;
}

int CountTotalItems(NodeBshv* p_start)
{
	NodeBshv *start = p_start;	// указатель на первый элемент списка
	NodeBshv *this = start;		// указатель на текущий элемент списка

	// пропустить первый (сторожевой) элемент списка
	this = this->next;

	// подсчитать количество записей
	int n = 0;
	while(this->next)
	{
		// определить указатель на список микросекунд
		NodeMicrosecond* p_mcs = this->ptr;

		//посчитать количество элементов в текущей секунде
		int k = CountItemsInListMicrosecond(p_mcs);
		n += k;

		// переключиться на следующую секунду
		this = this->next;
	}

	return n;
}

int CountItemsInListBshv(NodeBshv** p_start)
{
	NodeBshv *start = *p_start;	// указатель на первый элемент списка
	NodeBshv *this = start;		// указатель на текущий элемент списка
	int n = 0;
	while(this)
	{
		n++;
		this = (NodeBshv*)this->next;
	}
	if (n<2) { return -1; }	// Ошибка! В списке должно быть минимум два сторожевых элемента.
	return (n-2);			// n - кол-во элементов в списке, не считая сторожевых эл-тов
}


void InitListBshv()
{
	node_bshv_start = NULL;
	CreateLinkedList(&node_bshv_start);
}
