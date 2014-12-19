/*
 ============================================================================
 Name        : ibivk_firmware.c
 Author      : Ludmila
 Version     : 1
 Copyright   : 
 Description : IBIVK project in C, Ansi-style
 ============================================================================
 */

// библиотеки для работы с периферией
#include "xMil1553BC.h"
#include "xIrqmp.h"
#include "xTimer.h"
#include "xUart.h"
#include "xGrgpio.h"

// библиотеки текущего проекта
#include "list_transmit_struct.h"
#include "list_bshv.h"


void DisableCache()
{
	asm("set 0, %g0");
	asm("lda [%g0]2, %g1");
	asm("and 0xFFFFFFF0, %g1, %g2");
	asm("set 0x400000, %g4");
	asm("or %g4, %g2, %g3");
	asm("sta %g3, [%g0]2");
}

int main(void)
{
	DisableCache();
	CORE1553_SOFTWARE_RESET();
	IRQMP_CLEAR();
	InitInterruptHandlers();
	// ------------------------
	InitSystemBshv();

	CORE1553_INIT();
	TIMER_INIT();
	UART_INIT(115200);
	GRGPIO_INIT();
	IRQMP_ENABLE();

	//QUEUE_CLEAN_POINTERS();

	InitListBshv();


	//AnalyzeNewQueue();

	//TestFpga();

	//DemoListTransmit();

	while(1)
	{

		if (head_list_transmit != NULL)
		{
			SendItemToUart();
		}


	}

	return EXIT_SUCCESS;
}
