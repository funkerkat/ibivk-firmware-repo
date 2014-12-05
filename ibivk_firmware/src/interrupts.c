/*
 * interrupts.c
 *
 *  Created on: 17.10.2014
 *      Author: Ludmila
 */

#include "xGrgpio.h"
#include "xTimer.h"
#include "xUart.h"

void handler_timer_1(void);
void handler_1Hz(void);
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

void AddByteToBuffer(unsigned int val);
void Handler_irq_3()
{
	// UART 2
	unsigned int value = (*UART2_READ_BYTE) & 0xFF;
////	AddByteToShiftReg(value);
	AddByteToBuffer(value);

}

void Handler_irq_4()
{
	// default
}

void Handler_irq_5()
{
	// default
}

void Handler_irq_6()
{
	// Clear pending bit
///	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER1_CONTROL)) 	= *((int*)(TIMER_BASE_ADDRESS + REG_TIMER1_CONTROL)) & (~(1 << 4));

	// TIMER 1
	GRGPIO_TOGGLE(0);

}

void Handler_irq_7()
{

	ExecuteCore1553();

	// Clear pending bit
///	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER2_CONTROL)) 	= *((int*)(TIMER_BASE_ADDRESS + REG_TIMER2_CONTROL)) & (~(1 << 4));

	// TIMER 2
	//GRGPIO_TOGGLE(1);
	//GRGPIO_TOGGLE(1);
}

void Handler_irq_8()
{
	// TIMER 3
	GRGPIO_TOGGLE(2);
	GRGPIO_TOGGLE(2);
	GRGPIO_TOGGLE(2);
}

void Handler_irq_9()
{
	// default
}

void Handler_irq_10()
{
	// default
}

void Handler_irq_11()
{
	// default
}

void Handler_irq_12()
{
	// 1 Hz
	//handler_1Hz();
}

void Handler_irq_13()
{
	// default
}


#include "xIrqmp.h"
#include "xMil1553BC.h"
unsigned int core1553_irqs[32];
unsigned int core1553_irqs_cnt;

void Handler_irq_14()
{
	IRQMP_DISABLE();

	// B1553BRM
	int t = 1;
	t++;

	core1553_irqs[core1553_irqs_cnt] = *((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG04_PENDING_INTERRUPT));
	core1553_irqs_cnt++;

	IRQMP_ENABLE();
}

void Handler_irq_15()
{
	// default
}

// ----- External Interrupts (GPIO) -----


// ----- Default (not used) -----
void Handler_default()
{
	int t = 1;
	t++;
}

void InitInterruptHandlers()
{


	catch_interrupt(Handler_default, 1);
	catch_interrupt(Handler_irq_2, 2);
	catch_interrupt(Handler_irq_3, 3);
	catch_interrupt(Handler_default, 4);
	catch_interrupt(Handler_default, 5);
	catch_interrupt(Handler_irq_6, 6);
	catch_interrupt(Handler_irq_7, 7);
	catch_interrupt(Handler_irq_8, 8);
	catch_interrupt(Handler_default, 9);
	catch_interrupt(Handler_default, 10);
	catch_interrupt(Handler_default, 11);
	catch_interrupt(Handler_irq_12, 12);
	catch_interrupt(Handler_default, 13);
	catch_interrupt(Handler_irq_14, 14);
	catch_interrupt(Handler_default, 15);


	/*
	catch_interrupt(Handler_irq_1, 1);
	catch_interrupt(Handler_irq_2, 2);
	catch_interrupt(Handler_irq_3, 3);
	catch_interrupt(Handler_irq_4, 4);
	catch_interrupt(Handler_irq_5, 5);
	catch_interrupt(Handler_irq_6, 6);
	catch_interrupt(Handler_irq_7, 7);
	catch_interrupt(Handler_irq_8, 8);
	catch_interrupt(Handler_irq_9, 9);
	catch_interrupt(Handler_irq_10, 10);
	catch_interrupt(Handler_irq_11, 11);
	catch_interrupt(Handler_irq_12, 12);
	catch_interrupt(Handler_irq_13, 13);
	catch_interrupt(Handler_irq_14, 14);
	catch_interrupt(Handler_irq_15, 15);
	*/

}

