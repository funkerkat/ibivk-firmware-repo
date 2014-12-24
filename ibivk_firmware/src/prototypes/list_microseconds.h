/*
 * list_microseconds.h
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#ifndef LIST_MICROSECONDS_H_
#define LIST_MICROSECONDS_H_

void CreateListMicrosecond(NodeMicrosecond** p_start);
int AddNodeMicrosecondItem(NodeMicrosecond** p_start, EntryCore1553* entry, signed int mcs, NodeBshv* thisNodeBshv);
void RemoveItemFromListMicrosecond(NodeMicrosecond* this_item);
int CountItemsInListMicrosecond(NodeMicrosecond* p_start);

#endif /* LIST_MICROSECONDS_H_ */
