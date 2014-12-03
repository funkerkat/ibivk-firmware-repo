/*
 * put_into_linkedlist.c
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */

#include <stdlib.h>
#include "xSystem.h"
#include "bshv.h"
#include "bus_controller.h"
#include "xMil1553BC.h"
#include "xMil1553BC.h"
#include "node_bshv.h"
#include "bshv.h"

int AddNodeBshvItem(NodeBshv** p_start, NodeBshv* new_item);
NodeBshv* GetBshvNode(NodeBshv** p_start, Bshv* b);


int LoadPacketF1(BshvExtention* b, unsigned int cw, unsigned short dw[])
{
	Bshv b1 = b->myBshv;
	b1.second = 15;

	Bshv b2 = b->myBshv;
	b2.second = 17;

	Bshv b3 = b->myBshv;
	b3.second = 16;

	/*
	int n;

	// Поиск соответствующей записи БШВ или созданиe новой
	NodeBshv* myNodeBshv1 = AddNodeBshvItem(&node_bshv_start, &b1);
	n = CountItemsInListBshv(&node_bshv_start);


	NodeBshv* myNodeBshv2 = AddNodeBshvItem(&node_bshv_start, &b2);
	n = CountItemsInListBshv(&node_bshv_start);


	NodeBshv* myNodeBshv3 = AddNodeBshvItem(&node_bshv_start, &b3);
	n = CountItemsInListBshv(&node_bshv_start);


	RemoveItemFromNodeBshvList(myNodeBshv3);
*/


	/*
	EntryCore1553* p_entry = (EntryCore1553*) malloc(sizeof(EntryCore1553));

	Load1553MessageF1(p_entry, cw, dw);

	CORE1553_RELOAD(p_entry->reg_leon, p_entry->reg_core1553);
	START_EXECUTION();
	*/

	return EXIT_SUCCESS;
}

int LoadPacketF2(BshvExtention* b, unsigned int cw)
{
	/*
	EntryCore1553* p_entry = (EntryCore1553*) malloc(sizeof(EntryCore1553));

	Load1553MessageF2(p_entry, cw);

	CORE1553_RELOAD(p_entry->reg_leon, p_entry->reg_core1553);
	START_EXECUTION();
	free(p_entry);
	*/

	return EXIT_SUCCESS;
}

