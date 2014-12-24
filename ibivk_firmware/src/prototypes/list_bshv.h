/*
 * list_bshv.h
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#ifndef LIST_BSHV_H_
#define LIST_BSHV_H_

#include "nodes.h"

void InitListBshv();
void RemoveItemFromNodeBshvList(NodeBshv* this_item);
NodeBshv* AddNodeBshvItem(NodeBshv** p_start, Bshv* b);
int CountTotalItems(NodeBshv* p_start);

#endif /* LIST_BSHV_H_ */
