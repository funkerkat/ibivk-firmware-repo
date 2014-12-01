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


int LoadPacketF1(Bshv* b, unsigned int cw, unsigned short dw[])
{

	//unsigned short t[32];


	EntryCore1553* p_entry = (EntryCore1553*) malloc(sizeof(EntryCore1553));

	//EntryCore1553 entry;
//	int x = &entry;

	Load1553MessageF1(p_entry, cw, dw);

	START_EXECUTION();

	return EXIT_SUCCESS;
}

int LoadPacketF2(Bshv* b, unsigned int cw)
{


	return EXIT_SUCCESS;
}

