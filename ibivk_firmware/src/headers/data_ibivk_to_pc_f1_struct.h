/*
 * data_ibivk_to_pc_f1_struct.h
 *
 *  Created on: 18.12.2014
 *      Author: Ludmila
 */

#ifndef DATA_IBIVK_TO_PC_F1_STRUCT_H_
#define DATA_IBIVK_TO_PC_F1_STRUCT_H_

#include "bshv_struct.h"

typedef struct _Ibivk_to_pc_f1
{
	BshvExtention myBshvExtenion;
	unsigned short command_word;
	unsigned short status_word;
}Ibivk_to_pc_f1;

#endif /* DATA_IBIVK_TO_PC_F1_STRUCT_H_ */
