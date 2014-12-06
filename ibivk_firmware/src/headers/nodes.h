/*
 * nodes.h
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#ifndef NODES_H_
#define NODES_H_

#include "bshv_struct.h"
#include "bus_controller.h"

struct _NodeMicrosecond
{
	struct _NodeMicrosecond* prev;
	struct _NodeBshv *base_node_bshv;
	signed int microsecond;
	signed int timer_value;
	EntryCore1553* core1553_entry;
	struct _NodeMicrosecond* next;
};

struct _NodeBshv
{
	struct _NodeBshv *prev;
	Bshv myBshv;
	struct _NodeMicrosecond* ptr;
	struct _NodeBshv *next;
};

typedef struct _NodeMicrosecond NodeMicrosecond;
typedef struct _NodeBshv NodeBshv;

// глобальная переменная -- текущий элемент в списке микросекунд (планируется к выдаче в МКИО в ближайшее время)
extern NodeMicrosecond* global_microsecond;

// глобальная переменная -- точка входа в список секунд БШВ
extern NodeBshv* node_bshv_start;

#endif /* NODES_H_ */



