/*
 * node_bshv.h
 *
 *  Created on: 02.12.2014
 *      Author: Ludmila
 */
#include "bshv.h"
#include "list_microseconds.h"

#ifndef NODE_BSHV_H_
#define NODE_BSHV_H_

typedef struct NodeBshv
{
	struct NodeBshv* prev;
	Bshv myBshv;
	NodeMicrosecond* ptr;
	struct NodeBshv* next;
}NodeBshv;

extern NodeBshv* node_bshv_start;

#endif /* NODE_BSHV_H_ */
