/*
 * data_ibivk_to_pc_f2_struct.h
 *
 *  Created on: 18.12.2014
 *      Author: Ludmila
 */

#ifndef DATA_IBIVK_TO_PC_F2_STRUCT_H_
#define DATA_IBIVK_TO_PC_F2_STRUCT_H_

#include "bshv_struct.h"

typedef struct _Ibivk_to_pc_f2
{
	BshvExtention myBshvExtenion;
	unsigned short command_word;
	unsigned short status_word;
	unsigned short data_words[32];
}Ibivk_to_pc_f2;

#endif /* DATA_IBIVK_TO_PC_F2_STRUCT_H_ */
