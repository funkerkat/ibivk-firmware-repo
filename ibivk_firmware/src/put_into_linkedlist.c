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
	Bshv thisBshv = b->myBshv;

	// Поиск соответствующей записи БШВ или созданиe новой
	NodeBshv* myNodeBshv = AddNodeBshvItem(&node_bshv_start, &thisBshv);

	int n = CountItemsInListBshv(&node_bshv_start);

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

