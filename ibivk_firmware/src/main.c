/*
 ============================================================================
 Name        : ibivk_firmware.c
 Author      : Ludmila
 Version     : 1
 Copyright   : 
 Description : IBIVK project in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "xIrqmp.h"
#include "xUart.h"
#include "xTimer.h"
#include "xMil1553BC.h"

void InitInterruptHandlers(void);
void LinkedListDemo(void);
void InitSystemBshv(void);
void UartSettings(void);

#include "node_bshv.h"

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
	IRQMP_ENABLE();


	InitListBshv();

	int n = CountItemsInListBshv(&node_bshv_start);

	while(1)
	{


	}


	return EXIT_SUCCESS;
}
