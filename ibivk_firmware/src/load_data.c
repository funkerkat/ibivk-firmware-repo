/*
 * put_into_linkedlist.c
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */

// страндартная библиотека Си
#include <stdlib.h>

// библиотеки для работы с периферией
#include "xSystem.h"
#include "xMil1553BC.h"

// библиотеки текущего проекта
#include "bus_controller_struct.h"
#include "bshv_struct.h"
#include "nodes.h"

// прототипы функций
#include "list_microseconds.h"

int LoadPacketF1(BshvExtention* b, unsigned int cw, unsigned short dw[])
{
	HertzHandler();
	/*
	// выделить память в куче под данные МКИО
	EntryCore1553* p_entry = (EntryCore1553*) malloc(sizeof(EntryCore1553));
	Load1553MessageF1(p_entry, cw, dw);

	// Поиск соответствующей записи БШВ или созданиe новой
	NodeBshv* thisNodeBshv = AddNodeBshvItem(&node_bshv_start, &(b->myBshv));

	// Если не существует списка микросекунд, привязанного к даннному значению БШВ, то создать его:
	if (thisNodeBshv->ptr == NULL) { CreateListMicrosecond(&(thisNodeBshv->ptr)); }

	// Добавить текущую микросекунду в список микросекунд
	AddNodeMicrosecondItem(&(thisNodeBshv->ptr), p_entry, b->microsecond, thisNodeBshv);
*/
	return EXIT_SUCCESS;
}



int LoadPacketF2(BshvExtention* b, unsigned int cw)
{
	//HertzHandler();

	// выделить память в куче под данные МКИО
	EntryCore1553* p_entry = (EntryCore1553*) malloc(sizeof(EntryCore1553));
	Load1553MessageF2(p_entry, cw);

	// Поиск соответствующей записи БШВ или созданиe новой
	NodeBshv* thisNodeBshv = AddNodeBshvItem(&node_bshv_start, &(b->myBshv));

	// Если не существует списка микросекунд, привязанного к даннному значению БШВ, то создать его:
	if (thisNodeBshv->ptr == NULL) { CreateListMicrosecond(&(thisNodeBshv->ptr)); }

	// Добавить текущую микросекунду в список микросекунд
	AddNodeMicrosecondItem(&(thisNodeBshv->ptr), p_entry, b->microsecond, thisNodeBshv);

	return EXIT_SUCCESS;
}

