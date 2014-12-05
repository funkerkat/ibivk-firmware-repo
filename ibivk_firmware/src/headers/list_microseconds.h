/*
 * list_microseconds.h
 *
 *  Created on: 03.12.2014
 *      Author: Ludmila
 */

#include "bus_controller.h"

#ifndef LIST_MICROSECONDS_H_
#define LIST_MICROSECONDS_H_

typedef struct NodeMicrosecond
{
	struct NodeMicrosecond* prev;
	signed int microsecond;
	signed int timer_value;
	EntryCore1553* core1553_entry;
	struct NodeMicrosecond* next;
}NodeMicrosecond;

enum WatchdogNodeMicrosecondsValues
 {
	 NODE_MICROSECOND_MIN 	= -1,
	 NODE_MICROSECOND_MAX 	= 0x7FFFFFFF,
 };

extern NodeMicrosecond* global_microsecond;

void CreateListMicrosecond(NodeMicrosecond** p_start);
void DeleteListMicrosecond(NodeMicrosecond** p_start);
int CountItemsInListMicrosecond(NodeMicrosecond** p_start);
int AddNodeMicrosecondItem(NodeMicrosecond** p_start, EntryCore1553* entry, signed int mcs);
void RemoveItemFromListMicrosecond(NodeMicrosecond* this_item);

#endif /* LIST_MICROSECONDS_H_ */
