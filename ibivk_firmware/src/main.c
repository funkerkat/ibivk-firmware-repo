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
#include "uart_tx_queue_struct.h"
#include "list_bshv.h"


void Fuck()
{

}


int main(void)
{
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

	QUEUE_CLEAN_POINTERS();

	InitListBshv();

	//TestFpga();

	while(1)
	{
		if (uart_tx_queue.store != 0)
		{
			AnalyzeQueue();
		}


	}


	return EXIT_SUCCESS;
}
