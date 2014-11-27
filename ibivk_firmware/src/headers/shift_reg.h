/*
 * shift_reg.h
 *
 *  Created on: 06.11.2014
 *      Author: Ludmila
 */

#ifndef SHIFT_REG_H_
#define SHIFT_REG_H_

#define SHIFTREG_MAX_LENGTH	(255+4+1)
//#define SHIFTREG_MAX_LENGTH	(8)
//#define SHIFTREG_MAX_LENGTH	(16)


typedef struct ShiftReg
{
	unsigned int   		value;
	struct ShiftReg* 	next;
}Byte;

typedef struct ShiftRegPointers
{
	Byte* adrHead;
	Byte* adrThis;
	Byte* adrPrev;
	unsigned int reg_pointer;
}ShiftRegPointers;


#endif /* SHIFT_REG_H_ */
