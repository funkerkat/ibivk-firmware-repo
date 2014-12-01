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


int LoadPacketF1(Bshv* b, unsigned int cw, unsigned int* dw, unsigned int n)
{

	EntryCore1553 entry;
	Load1553MessageF1(&entry, cw, dw);


	return EXIT_SUCCESS;
}

int LoadPacketF2(Bshv* b, unsigned int cw)
{


	return EXIT_SUCCESS;
}

