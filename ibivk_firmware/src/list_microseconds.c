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
	// Сравнить микросекунды
	if ( new_mcs > old_mcs ) { return NewItemIsGreater; }
	if ( new_mcs < old_mcs ) { return NewItemIsLess; }

	// Если не произошел выход из функции по любому из условий выше, оба значения равны:
	return Equal;
}

static void InsertNodeMicrosecond(NodeMicrosecond *this_item, NodeMicrosecond *new_item)
{
    // устанавливаем указатели у нового элемента на следующий и предшествующий элементы
	new_item->prev = this_item->prev;

	NodeMicrosecond* prev_item = this_item->prev;
	new_item->next = prev_item->next;

    // устанавливаем указатель у предшествующего элемента на новый элемент
	prev_item->next = new_item;

    // устанавливаем указатель у следующего элемента на новый элемент
	this_item->prev = new_item;
}

static NodeMicrosecond* CreateNodeMicrosecond(signed int mcs, EntryCore1553* entry)
{
	NodeMicrosecond* new_item = (NodeMicrosecond*) malloc(sizeof(NodeMicrosecond));
	new_item->microsecond = mcs;
	new_item->core1553_entry = entry;
	// new_item->timer_value	// пересчитать значение для таймера
	new_item->next = NULL;
	new_item->prev = NULL;
	return new_item;
}

void RemoveItemFromListMicrosecond(NodeMicrosecond* this_item)
{
	// найти предшествующий и следующий элемент списка
	NodeMicrosecond* prev_item = this_item->prev;
	NodeMicrosecond* next_item = this_item->next;

	// исправить указатели предшествующего элемента
	prev_item->next = next_item;

	// исправить указатели следующего элемента
	next_item->prev = prev_item;

	// удалить данные МКИО, хранящиеся в этом элементе
	EntryCore1553* entry = this_item->core1553_entry;
	free(entry);

	// удалить текущий элемент
	free(this_item);
	this_item = NULL;
}

int AddNodeMicrosecondItem(NodeMicrosecond** p_start, EntryCore1553* entry, signed int mcs)
{
	NodeMicrosecond *start = *p_start;	// указатель на первый элемент списка
	NodeMicrosecond *this = start;		// указатель на текущий элемент списка
	result_type res = NotUsed;

	if (start == NULL) {return NULL;}	// Ошибка! Список не создан

	// поиск по всем элементам списка, кроме сторожевых
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
				// заменить запись
				free(this->core1553_entry);
				this->core1553_entry = entry;
				return EXIT_SUCCESS;
			}

			case NewItemIsLess:
			{
				NodeMicrosecond* new_item = CreateNodeMicrosecond(mcs, entry);
				InsertNodeMicrosecond(this, new_item);		// Вставляем элемент в середину списка
				return EXIT_SUCCESS;
			}

			default: return EXIT_FAILURE;	// Аварийный выход
		}
		// место для вставки пока не нашлось, ищем дальше:
		this = this->next;
	}
	return EXIT_FAILURE;
}

void DeleteListMicrosecond(NodeMicrosecond** p_start)
{
	NodeMicrosecond *start = *p_start;	// указатель на первый элемент списка
	NodeMicrosecond *this = start;		// указатель на текущий элемент списка
	NodeMicrosecond* next_item = NULL;	// переменная для временного хранения указателя на след. элемент

	while(this)
	{
		// сохранить указатель на следующий элемент
		next_item = this->next;
		// удалить текущий элемент
		free(this);
		this = NULL;
		// переключиться на следующий элемент
		this = next_item;
	}
    *p_start = NULL;
}

int CountItemsInListMicrosecond(NodeMicrosecond** p_start)
{
	NodeMicrosecond *start = *p_start;	// указатель на первый элемент списка
	NodeMicrosecond *this = start;		// указатель на текущий элемент списка
	int n = 0;
	while(this)
	{
		n++;
		this = this->next;
	}
	if (n<2) { return -1; }	// Ошибка! В списке должно быть минимум два сторожевых элемента.
	return (n-2);			// n - кол-во элементов в списке, не считая сторожевых эл-тов
}

void CreateListMicrosecond(NodeMicrosecond** p_start)
{
	// Создаются сторожевые элементы связного списка: первый и последний.
	// (Новые элементы будут вставляться в связный список между сторожевыми элементами.)

	// 1. Создадим первый (first) элемент списка:
	// выделить память и присвоить значения
	NodeMicrosecond* first_item = (NodeMicrosecond*) malloc(sizeof(NodeMicrosecond));
    first_item->microsecond = NODE_MICROSECOND_MIN;

    // установить указатель на след. элемент в NULL, потом переставить на last_item
    first_item->next = NULL;
    first_item->prev = NULL;
    // устанавливаем указатель первого элемента списка на созданный элемент
    *p_start = first_item;

	// 2. Создадим последний (last) элемент списка:
	// выделить память и присвоить значения
    NodeMicrosecond* last_item = (NodeMicrosecond*) malloc(sizeof(NodeMicrosecond));
    last_item->microsecond = NODE_MICROSECOND_MAX;

    // установить указатель на след. элемент в NULL, т. к. элемент последний в списке
    last_item->next = NULL;
    last_item->prev = first_item;

    // 3. связываем элементы first и last
    first_item->next = last_item;
}

