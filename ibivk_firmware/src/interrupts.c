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

// прототипы функций
#include "timer.h"
#include "uart_rx.h"

extern void *catch_interrupt(void func(), int irq);

// ----- Internal Interrupts  -----
void Handler_irq_1()
{
	// default
}

void Handler_irq_2()
{
	// UART 1
}

void Handler_irq_3()
{
	// UART 2
	IRQMP_DISABLE();
	unsigned int value = (*UART2_READ_BYTE) & 0xFF;
	AddByteToBuffer(value);
	IRQMP_ENABLE();
}

void Handler_irq_4()
{
	// IRQMP_320MS

	//ReadSystemBshv();

	int t = 0;
	t++;
}

void Handler_irq_5()
{
	// default
}

void Handler_irq_6()
{

}

void Handler_irq_7()
{
	IRQMP_DISABLE();
	ExecuteCore1553();
	IRQMP_ENABLE();
}

void Handler_irq_8()
{

}

void Handler_irq_9()
{

}

void Handler_irq_10()
{

}

void Handler_irq_11()
{

}

void Handler_irq_12()
{

}

void Handler_irq_13()
{

}


void Handler_irq_14()
{
	unsigned short core1553_pending = *((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG04_PENDING_INTERRUPT));;
	Core1553_Selftest(core1553_pending);
}

void Handler_irq_15()
{

	int t = 0;
	t++;
}

// ----- External Interrupts (GPIO) -----


// ----- Default (not used) -----
void Handler_default()
{

}

void InitInterruptHandlers()
{
	catch_interrupt(Handler_default, 1);
	catch_interrupt(Handler_irq_2, 2);
	catch_interrupt(Handler_irq_3, 3);
	catch_interrupt(Handler_irq_4, 4);
	catch_interrupt(Handler_default, 5);
	catch_interrupt(Handler_irq_6, 6);
	catch_interrupt(Handler_irq_7, 7);
	catch_interrupt(Handler_irq_8, 8);
	catch_interrupt(Handler_default, 9);
	catch_interrupt(Handler_default, 10);
	catch_interrupt(Handler_default, 11);
	catch_interrupt(Handler_default, 12);
	catch_interrupt(Handler_default, 13);
	catch_interrupt(Handler_irq_14, 14);
	catch_interrupt(Handler_irq_15, 15);
}

