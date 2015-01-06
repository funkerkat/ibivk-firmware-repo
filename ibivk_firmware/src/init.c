/*
 * init.c
 *
 *  Created on: 23.12.2014
 *      Author: Ludmila
 */

// библиотеки для работы с периферией
#include "xMil1553BC.h"
#include "xIrqmp.h"
#include "xUart.h"
#include "xTimer.h"
#include "xGrgpio.h"
#include "xBshvtransceiver.h"

// прототипы функций
#include "list_bshv.h"
#include "list_transmit.h"
#include "bshv.h"
#include "tmi.h"
#include "interrupts.h"

static void DisableCache()
{
	asm("set 0, %g0");
	asm("lda [%g0]2, %g1");
	asm("and 0xFFFFFFF0, %g1, %g2");
	asm("set 0x400000, %g4");
	asm("or %g4, %g2, %g3");
	asm("sta %g3, [%g0]2");
}

void InitIbivk()
{
	// reset ядра 1553
	CORE1553_SOFTWARE_RESET();

	// очистить регистры прерываний
	IRQMP_CLEAR();

	// отключение кэша
	DisableCache();

	// инициализация обработчиков прерываний
	InitInterruptHandlers();

	// иницилизация начального значения системного БШВ
	InitSystemBshv();

	// инициализация ядра 1553
	CORE1553_INIT();

	// инициализация таймера
	TIMER_INIT();

	// инициализация UART
	UART_INIT(115200);

	// инициализация GPIO
	GRGPIO_INIT();

	// инициализация ПЛИС
	BSHV_TRANSCEIVER_INIT();

	// инициализация связного списка, содержащего секунды (сторожевые элементы)
	InitListBshv();

	// инициализация очереди сообщений на выдачу в УАРТ
	InitListTransmit();

	// инициализация переменных
	InitTimerType();

	// инициализировать начальные значения телеметрии ИБИВК
	InitTmi();
}
