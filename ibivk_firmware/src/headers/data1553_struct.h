/*
 * data1553_struct.h
 *
 *  Created on: 17.12.2014
 *      Author: Ludmila
 */
/*
#ifndef DATA1553_STRUCT_H_
#define DATA1553_STRUCT_H_

#include "bshv_struct.h"

typedef struct _Data1553
{
	BshvExtention myBshvExtenion;
	unsigned short command_word;
	unsigned short status_word;
	unsigned short data_words[32];
}Data1553;
/*
#endif /* DATA1553_STRUCT_H_ */

