/*
 * linkedlist.c
 *
 *  Created on: 02.12.2014
 *      Author: Ludmila
 */

#include <stdlib.h>
#include "Node_bshv.h"

typedef enum {NotUsed, NewItemIsLess, Equal, NewItemIsGreater} result_type;

NodeBshv* node_bshv_start;

static result_type Compare(Bshv* old_bshv, Bshv* new_bshv)
{
	// Сравнить четырехлетия
	if ( (new_bshv->fouryears) > (old_bshv->fouryears) ) { return NewItemIsGreater; }
	if ( (new_bshv->fouryears) < (old_bshv->fouryears) ) { return NewItemIsLess; }

	// Сравнить сутки
	if ( (new_bshv->day) > (old_bshv->day) ) { return NewItemIsGreater; }
	if ( (new_bshv->day) < (old_bshv->day) ) { return NewItemIsLess; }

	// Сравнить час
	if ( (new_bshv->hour) > (old_bshv->hour) ) { return NewItemIsGreater; }
	if ( (new_bshv->hour) < (old_bshv->hour) ) { return NewItemIsLess; }

	// Сравнить минуту
	if ( (new_bshv->minute) > (old_bshv->minute) ) { return NewItemIsGreater; }
	if ( (new_bshv->minute) < (old_bshv->minute) ) { return NewItemIsLess; }

	// Сравнить секунду
	if ( (new_bshv->second) > (old_bshv->second) ) { return NewItemIsGreater; }
	if ( (new_bshv->second) < (old_bshv->second) ) { return NewItemIsLess; }

	// Если не произошел выход из функции по любому из условий выше, оба значения равны:
	return Equal;
}

static void CreateLinkedList(NodeBshv** p_start)
{
	// Создаются сторожевые элементы связного списка: первый и последний.
	// (Новые элементы будут вставляться в связный список между сторожевыми элементами.)

	// 1. Создадим первый (first) элемент списка:
	// выделить память и присвоить значения
    NodeBshv* first_item = (NodeBshv*) malloc(sizeof(NodeBshv));
    first_item->myBshv.fouryears = -1;
    first_item->myBshv.day = -1;
    first_item->myBshv.hour = -1;
    first_item->myBshv.minute = -1;
    first_item->myBshv.second = -1;
    // установить указатель на след. элемент в NULL, потом переставить на last_item
    first_item->next = NULL;
    // устанавливаем указатель первого элемента списка на созданный элемент
    *p_start = first_item;

	// 2. Создадим последний (last) элемент списка:
	// выделить память и присвоить значения
    NodeBshv* last_item = (NodeBshv*) malloc(sizeof(NodeBshv));
    last_item->myBshv.fouryears = 0x7FFFFFFF;
    last_item->myBshv.day = 0x7FFFFFFF;
    last_item->myBshv.hour = 0x7FFFFFFF;
    last_item->myBshv.minute = 0x7FFFFFFF;
    last_item->myBshv.second = 0x7FFFFFFF;
    // установить указатель на след. элемент в NULL, т. к. элемент последний в списке
    last_item->next = NULL;

    // 3. связываем элементы first и last
    first_item->next = last_item;
}

static void InsertNodeBshv(NodeBshv *prev_item, NodeBshv *this_item)
{
    // устанавливаем указатель у нового элемента на след. элемент
    this_item->next = prev_item->next;

    // устанавливаем указатель предыдущего элемента на новый (текущий) элемент
    prev_item->next = this_item;
}

static NodeBshv* CreateNodeBshv(Bshv* b)
{
	NodeBshv* new_item = (NodeBshv*) malloc(sizeof(NodeBshv));
	new_item->myBshv = *b;
	new_item->ptr = NULL;
	new_item->next = NULL;
	return new_item;
}

NodeBshv* AddNodeBshvItem(NodeBshv** p_start, Bshv* b)
{
	NodeBshv *start = *p_start;	// указатель на первый элемент списка
	NodeBshv *prev = NULL;		// указатель на элемент списка, предшествующий элементу, по которому срабатывает условие сравнения
	NodeBshv *this = start;		// указатель на текущий элемент списка
	result_type res = NotUsed;

	if (start == NULL) {return NULL;}	// Ошибка! Список не создан

	// поиск по всем элементам списка, кроме сторожевых
	while(this)
	{
		res = Compare(&(this->myBshv), b);

		switch(res)
		{
			case NewItemIsGreater:
			{
				;	// continue searching
				break;
			}

			case Equal:
			{
				return this;
			}

			case NewItemIsLess:
			{
				NodeBshv* new_item = CreateNodeBshv(b);
				InsertNodeBshv(prev, new_item);		// Вставляем элемент в середину списка
				return new_item;
			}

			default: return NULL;	// Аварийный выход
		}

		// место для вставки пока не нашлось, ищем дальше:
		prev = this;
		this = (NodeBshv*)this->next;
	}

	return NULL;
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
/*
void ListBshvDemo()
{
	NodeBshv* start = NULL;
	CreateLinkedList(&start);

	NodeBshv* new_item1 = CreateItem(0x25);
	AddNodeBshvItem(&start, new_item1);

	NodeBshv* new_item2 = CreateItem(0x96);
	AddNodeBshvItem(&start, new_item2);

	NodeBshv* new_item3 = CreateItem(0x48);
	AddNodeBshvItem(&start, new_item3);


	int n = CountItemsInLinkedList(&start);


}
*/
