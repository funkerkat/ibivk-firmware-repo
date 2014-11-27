/*
 * irqmp.c
 *
 *  Created on: 16.10.2014
 *      Author: Ludmila
 */

#include "irqmp.h"

void InitIrqmp()
{
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_MASK)) = IRQMP_MASK;
//	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_MASK)) = 0x3FFF;
//	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_MASK)) = 0;



	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_PENDING)) = 0;

// force:
//	adr = (int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_FORCE);
//	*adr = 1 << 6;
//	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_FORCE)) =  1 << 6;

}
