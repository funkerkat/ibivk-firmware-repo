/*
 * interrupts.c
 *
 *  Created on: 17.10.2014
 *      Author: Ludmila
 */

// библиотеки для работы с периферией
#include "xUart.h"
#include "xIrqmp.h"
#include "xMil1553BC.h"

#include "xGrgpio.h"

// библиотеки текущего проекта
#include "bshv_struct.h"
#include "programm_errors.h"

// прототипы функций
#include "timer.h"
#include "uart_rx.h"
#include "selftest.h"

extern void *catch_interrupt(void func(), int irq);

// -- UART 1 --
void Handler_irq_2()
{

}

// -- UART 2 --
void Handler_irq_3()
{
	IRQMP_DISABLE();
	unsigned int value = (*UART2_READ_BYTE) & 0xFF;
	AddByteToBuffer(value);
	IRQMP_ENABLE();
}

// -- IRQMP_320MS --
void Handler_irq_4()
{
	ReadBshvFromFPGA();
	PredictNextBshvValue(&bshv_prev);
}

// -- TIMER 1 --
void Handler_irq_6()
{

}

// -- TIMER 2 --
void Handler_irq_7()
{
	IRQMP_DISABLE();
	ExecuteCore1553();
	IRQMP_ENABLE();
}

// -- TIMER 3 --
void Handler_irq_8()
{

}

// -- CORE 1553 --
void Handler_irq_14()
{
	unsigned short core1553_pending = *((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG04_PENDING_INTERRUPT));
	if (core1553_pending != 0)
	{
		Core1553_Selftest(core1553_pending);
	}
}

// -- IRQMP_1Hz --
void Handler_irq_15()
{
	// установить новое системное время
	CopyBshv(&bshv_prev, &system_bshv);

	// перейти к обработке сообщений
	HertzHandler();

	//SendTmi();
}

// -- Default (not used) --
void Handler_default()
{
	PmoSelftest(ALGORITHM_ERROR_CODE_1);
}

void InitInterruptHandlers()
{
	catch_interrupt(Handler_default, 	1);
	catch_interrupt(Handler_irq_2, 		2);
	catch_interrupt(Handler_irq_3, 		3);
	catch_interrupt(Handler_irq_4, 		4);
	catch_interrupt(Handler_default, 	5);
	catch_interrupt(Handler_irq_6, 		6);
	catch_interrupt(Handler_irq_7, 		7);
	catch_interrupt(Handler_irq_8, 		8);
	catch_interrupt(Handler_default, 	9);
	catch_interrupt(Handler_default, 	10);
	catch_interrupt(Handler_default, 	11);
	catch_interrupt(Handler_default, 	12);
	catch_interrupt(Handler_default, 	13);
	catch_interrupt(Handler_irq_14, 	14);
	catch_interrupt(Handler_irq_15, 	15);
}

