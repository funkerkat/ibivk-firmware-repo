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
#include "xSystem.h"
#include "xIrqmp.h"

// библиотеки текущего проекта
#include "list_transmit_struct.h"

// прототипы функций
#include "list_transmit.h"
#include "init.h"
#include "selftest.h"

int main(void)
{
	// инициализация микроконтроллера
	InitIbivk();

	// первичное самотестирование
	InitSelftest();

	// разрешение прерываний
	IRQMP_ENABLE();

	while(1)
	{
		// Если в очереди на выдачу в УАРТ есть элементы, осуществить выдачу
		if (head_list_transmit != NULL) { SendItemToUart(); }
	}

	return EXIT_SUCCESS;
}



/*
DisableCache();
CORE1553_SOFTWARE_RESET();
IRQMP_CLEAR();
InitInterruptHandlers();
// ------------------------
InitSystemBshv();		// ?

CORE1553_INIT();
TIMER_INIT();
UART_INIT(115200);
GRGPIO_INIT();
IRQMP_ENABLE();


InitListBshv();

// инициализация очереди сообщений на выдачу в УАРТ
InitListTransmit();

// инициализировать начальные значения телеметрии ИБИВК
InitTmi();

//TestFpga();
*/
