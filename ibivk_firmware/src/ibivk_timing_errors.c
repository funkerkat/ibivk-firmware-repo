/*
 ============================================================================
 Name        : ibivk_firmware.c
 Author      : Ludmila
 Version     : 1
 Copyright   :
 Description : IBIVK project in C, Ansi-style
 ============================================================================
 */
/*
#include <stdio.h>
#include <stdlib.h>

#include "xTimer.h"
#include "xIrqmp.h"
#include "xGrgpio.h"

#include "node.h"



#define DELTA 12


int main_timer_errors(void)
{

	TIMER_INIT();

    InitInterruptHandlers();

    GRGPIO_INIT();
    IRQMP_INIT();

//	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_FORCE)) =  1 << 12;


	TIMER1_START(150 - DELTA);
	GRGPIO_TOGGLE(0);

//	TIMER2_START(20 - DELTA);
//	GpioToggle(1);
//
	TIMER2_START(75 - DELTA);
	GRGPIO_TOGGLE(1);

	while(1)
	{
		GRGPIO_TOGGLE(2);
	}


	return EXIT_SUCCESS;

}
*/


